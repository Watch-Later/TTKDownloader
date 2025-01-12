#include "downloadbackgroundlistwidget.h"
#include "downloadextractwrapper.h"
#include "downloadmessagebox.h"

#include <QPainter>
#include <QMouseEvent>

#define ITEM_COUNT      4

DownloadBackgroundListItem::DownloadBackgroundListItem(QWidget *parent)
    : QLabel(parent),
      m_printMask(false),
      m_isSelected(false),
      m_selectedMask(true),
      m_closeMask(false),
      m_closeSet(false)
{
    setFixedSize(100, 70);
    setCursor(Qt::PointingHandCursor);
}

void DownloadBackgroundListItem::updatePixImage()
{
    if(!m_path.isEmpty())
    {
        DownloadBackgroundImage image;
        if(DownloadExtractWrapper::outputSkin(&image, m_path))
        {
            updatePixImage(image);
        }
    }
}

void DownloadBackgroundListItem::updatePixImage(const DownloadBackgroundImage &image)
{
    m_imageInfo = image.m_item;
    setPixmap(image.m_pix.scaled(size()));
}

bool DownloadBackgroundListItem::contains(const DownloadSkinConfigItem &item) const
{
    if(item.isValid() && m_imageInfo.isValid())
    {
        return item.m_name == m_imageInfo.m_name;
    }
    return false;
}

void DownloadBackgroundListItem::setSelect(bool s)
{
    m_isSelected = s;
    update();
}

void DownloadBackgroundListItem::setSelectEnable(bool s)
{
    m_selectedMask = s;
    update();
}

void DownloadBackgroundListItem::setCloseEnable(bool s)
{
    m_closeSet = s;
    update();
}

void DownloadBackgroundListItem::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    if(m_closeSet && QRect(width() - 18 - 4, 4, 18, 18).contains(event->pos()))
    {
        Q_EMIT closeClicked(this);
    }
    else
    {
        Q_EMIT itemClicked(this);
    }
}

void DownloadBackgroundListItem::leaveEvent(QEvent *event)
{
    QLabel::leaveEvent(event);
    m_printMask = false;
    m_closeMask = false;
    update();
}

void DownloadBackgroundListItem::enterEvent(QtEnterEvent *event)
{
    QLabel::enterEvent(event);
    m_printMask = true;
    m_closeMask = true;
    update();
}

void DownloadBackgroundListItem::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);

    if(m_isSelected && m_selectedMask)
    {
        QPainter painter(this);
        painter.drawPixmap(width() - 17, height() - 17, 17, 17, QPixmap(":/tiny/lb_selected"));
    }

    if(m_printMask)
    {
        QPainter painter(this);
        painter.setBrush(QColor(0, 0, 0, 155));
        painter.drawRect(rect());

        QFont font = painter.font();
        font.setPixelSize(13);
        painter.setFont(font);
        const QFontMetrics ftm(font);

        painter.setPen(Qt::white);
        QString v = QString::number(m_imageInfo.m_useCount);
        painter.drawText((width() - QtFontWidth(ftm, v)) / 2, 30, v);

        v = m_imageInfo.m_name;
        painter.drawText((width() - QtFontWidth(ftm, v)) / 2, 48, v);
    }

    if(m_closeSet && m_closeMask)
    {
        QPainter painter(this);
        painter.drawPixmap(width() - 18 - 4, 4, 18, 18, QPixmap(":/functions/btn_close_hover"));
    }
}


DownloadBackgroundListWidget::DownloadBackgroundListWidget(QWidget *parent)
    : QWidget(parent),
      m_currentItem(nullptr)
{
    m_layout = new QGridLayout(this);
    m_layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_layout->setContentsMargins(7, 7, 7, 7);
    m_layout->setSpacing(5);
    setLayout(m_layout);
}

DownloadBackgroundListWidget::~DownloadBackgroundListWidget()
{
    clearAllItems();
}

void DownloadBackgroundListWidget::setCurrentItemName(const QString &name)
{
    //Set the current theme index
    for(DownloadBackgroundListItem *item : qAsConst(m_items))
    {
        if(item->fileName() == name)
        {
            item->setSelect(true);
            m_currentItem = item;
            break;
        }
    }
}

void DownloadBackgroundListWidget::clearSelectState()
{
    for(DownloadBackgroundListItem *item : qAsConst(m_items))
    {
        item->setSelect(false);
    }
}

void DownloadBackgroundListWidget::clearAllItems()
{
    qDeleteAll(m_items);
    m_items.clear();
}

void DownloadBackgroundListWidget::createItem(const QString &name, const QString &path, bool state)
{
    DownloadBackgroundListItem *item = new DownloadBackgroundListItem(this);
    item->setCloseEnable(state);
    item->setFileName(name);
    item->setFilePath(path);
    item->updatePixImage();
    connect(item, SIGNAL(itemClicked(DownloadBackgroundListItem*)), SLOT(itemHasClicked(DownloadBackgroundListItem*)));
    connect(item, SIGNAL(closeClicked(DownloadBackgroundListItem*)), SLOT(itemCloseClicked(DownloadBackgroundListItem*)));
    m_layout->addWidget(item, m_items.count()/ITEM_COUNT, m_items.count()%ITEM_COUNT, Qt::AlignLeft | Qt::AlignTop);
    m_items << item;
}

void DownloadBackgroundListWidget::createItem(const QString &icon, bool state)
{
    DownloadBackgroundListItem *item = new DownloadBackgroundListItem(this);
    item->setCloseEnable(state);
    item->setPixmap(QPixmap(icon).scaled(item->size()));
    connect(item, SIGNAL(itemClicked(DownloadBackgroundListItem*)), SLOT(itemHasClicked(DownloadBackgroundListItem*)));
    connect(item, SIGNAL(closeClicked(DownloadBackgroundListItem*)), SLOT(itemCloseClicked(DownloadBackgroundListItem*)));
    m_layout->addWidget(item, m_items.count()/ITEM_COUNT, m_items.count()%ITEM_COUNT, Qt::AlignLeft | Qt::AlignTop);
    m_items << item;
}

void DownloadBackgroundListWidget::updateItem(const DownloadBackgroundImage &image, const QString &path)
{
    for(DownloadBackgroundListItem *item : qAsConst(m_items))
    {
        if(item->fileName().isEmpty())
        {
            item->setSelectEnable(false);
            item->setFileName(path);
            item->updatePixImage(image);
            break;
        }
    }
}

bool DownloadBackgroundListWidget::contains(const QString &name) const
{
    for(DownloadBackgroundListItem *item : qAsConst(m_items))
    {
        if(item->fileName() == name)
        {
            return true;
        }
    }

    return false;
}

bool DownloadBackgroundListWidget::contains(const DownloadBackgroundImage &image) const
{
    for(DownloadBackgroundListItem *item : qAsConst(m_items))
    {
        if(item->contains(image.m_item))
        {
            return true;
        }
    }

    return false;
}

int DownloadBackgroundListWidget::find(DownloadBackgroundListItem *item) const
{
    for(int i = 0; i < m_items.count(); ++i)
    {
        if(m_items[i] == item)
        {
            return i;
        }
    }

    return -1;
}

DownloadBackgroundListItem* DownloadBackgroundListWidget::find(const QString &name) const
{
    for(DownloadBackgroundListItem *item : qAsConst(m_items))
    {
        if(item->fileName() == name)
        {
            return item;
        }
    }

    return nullptr;
}

DownloadBackgroundListItem* DownloadBackgroundListWidget::find(const DownloadBackgroundImage &image) const
{
    for(DownloadBackgroundListItem *item : qAsConst(m_items))
    {
        if(item->contains(image.m_item))
        {
            return item;
        }
    }

    return nullptr;
}

void DownloadBackgroundListWidget::updateLastedItem()
{
    if(!m_items.isEmpty())
    {
        itemHasClicked(m_items.back());
    }
}

void DownloadBackgroundListWidget::itemCloseClicked(DownloadBackgroundListItem *item)
{
    if(m_items.count() == 1)
    {
        DownloadMessageBox message;
        message.setText(tr("Last One Item Can not be deleted!"));
        message.exec();
        return;
    }

    m_layout->removeWidget(item);
    int index = find(item);
    int cIndex = find(m_currentItem);
    QFile::remove(item->filePath());
    m_items.takeAt(index)->deleteLater();

    if(index == cIndex)
    {
        m_currentItem = nullptr;
        if(!m_items.isEmpty())
        {
            itemHasClicked(m_items[index == 0 ? 0 : index - 1]);
        }
    }

    for(int i = index; i < m_items.count(); ++i)
    {
        m_layout->addWidget(m_items[i], i/ITEM_COUNT, i%ITEM_COUNT, Qt::AlignLeft | Qt::AlignTop);
    }
}

void DownloadBackgroundListWidget::itemHasClicked(DownloadBackgroundListItem *item)
{
    if(m_currentItem)
    {
        m_currentItem->setSelect(false);
    }

    m_currentItem = item;
    m_currentItem->setSelect(true);
    Q_EMIT itemClicked(item->fileName());
}
