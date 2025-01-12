#include "downloadsysconfigmanager.h"
#include "downloadsettingmanager.h"
#include "downloadcoreutils.h"

#include <QRect>

DownloadSysConfigManager::DownloadSysConfigManager(QObject *parent)
    : TTKAbstractXml(parent)
{

}

void DownloadSysConfigManager::writeXMLConfig()
{
    QPoint widgetPositionChoiced = G_SETTING_PTR->value(DownloadSettingManager::WidgetPosition).toPoint();
    QSize widgetSizeChoiced = G_SETTING_PTR->value(DownloadSettingManager::WidgetSize).toSize();
    int closeEventChoiced = G_SETTING_PTR->value(DownloadSettingManager::CloseEventChoiced).toInt();
    int closeNetWorkChoiced = G_SETTING_PTR->value(DownloadSettingManager::CloseNetWorkChoiced).toInt();
    int fileAssociationChoiced = G_SETTING_PTR->value(DownloadSettingManager::FileAssociationChoiced).toInt();
    int startupModeChoiced = G_SETTING_PTR->value(DownloadSettingManager::StartUpModeChoiced).toInt();
    int startupRunModeChoiced = G_SETTING_PTR->value(DownloadSettingManager::StartUpRunModeChoiced).toInt();
    int slienceRunModeChoiced = G_SETTING_PTR->value(DownloadSettingManager::SlienceRunModeChoiced).toInt();

    QString bgThemeChoiced = G_SETTING_PTR->value(DownloadSettingManager::BgThemeChoiced).toString();
    int bgTransparentChoiced = G_SETTING_PTR->value(DownloadSettingManager::BgTransparentChoiced).toInt();

    int downloadLimit = G_SETTING_PTR->value(DownloadSettingManager::DownloadLimitChoiced).toInt();
    int downloadModeChoiced = G_SETTING_PTR->value(DownloadSettingManager::DownloadModeChoiced).toInt();
    int downloadMaxCountChoiced = G_SETTING_PTR->value(DownloadSettingManager::DownloadMaxCountChoiced).toInt();
    QString downloadPathDir = G_SETTING_PTR->value(DownloadSettingManager::DownloadPathDirChoiced).toString();
    QString downloadDLoadLimit = G_SETTING_PTR->value(DownloadSettingManager::DownloadDLoadLimitChoiced).toString();
    QString downloadULoadLimit = G_SETTING_PTR->value(DownloadSettingManager::DownloadULoadLimitChoiced).toString();

    int skinEffectLevelChoiced = G_SETTING_PTR->value(DownloadSettingManager::SkinEffectLevelChoiced).toInt();
    int skinFontChoiced = G_SETTING_PTR->value(DownloadSettingManager::SkinFontChoiced).toInt();
    int skinSuspensionChoiced = G_SETTING_PTR->value(DownloadSettingManager::SkinSuspensionChoiced).toInt();
    int skinSuspensionPerChoiced = G_SETTING_PTR->value(DownloadSettingManager::SkinSuspensionPerChoiced).toInt();

    if(!toFile(COFIG_PATH_FULL))
    {
        return;
    }

    createProcessingInstruction();
    QDomElement rootDom = createRoot(APP_NAME);
    QDomElement plusSettingDom = writeDomNode(rootDom, "plusSetting");
    QDomElement backgroundSettingDom = writeDomNode(rootDom, "backgroundSetting");
    QDomElement downloadSettingDom = writeDomNode(rootDom, "downloadSetting");
    QDomElement skinSettingDom = writeDomNode(rootDom, "skinSetting");

    writeDomElement(plusSettingDom, "geometry", TTKXmlAttribute("value", QString("%1,%2,%3,%4").arg(widgetPositionChoiced.x())
                    .arg(widgetPositionChoiced.y()).arg(widgetSizeChoiced.width()).arg(widgetSizeChoiced.height())));
    writeDomElement(plusSettingDom, "closeEvent", TTKXmlAttribute("value", closeEventChoiced));
    writeDomElement(plusSettingDom, "closeNetwork", TTKXmlAttribute("value", closeNetWorkChoiced));
    writeDomElement(plusSettingDom, "fileAssociation", TTKXmlAttribute("value", fileAssociationChoiced));
    writeDomElement(plusSettingDom, "startUpMode", TTKXmlAttribute("value", startupModeChoiced));
    writeDomElement(plusSettingDom, "startUpRunMode", TTKXmlAttribute("value", startupRunModeChoiced));
    writeDomElement(plusSettingDom, "slienceRunMode", TTKXmlAttribute("value", slienceRunModeChoiced));

    writeDomElement(backgroundSettingDom, "bgTheme", TTKXmlAttribute("value", bgThemeChoiced));
    writeDomElement(backgroundSettingDom, "bgTransparent", TTKXmlAttribute("value", bgTransparentChoiced));

    writeDomElement(downloadSettingDom, "downloadLimit", TTKXmlAttribute("value", downloadLimit));
    writeDomElement(downloadSettingDom, "downloadMode", TTKXmlAttribute("value", downloadModeChoiced));
    writeDomElement(downloadSettingDom, "downloadMaxCount", TTKXmlAttribute("value", downloadMaxCountChoiced));
    writeDomElement(downloadSettingDom, "downloadPathDir", TTKXmlAttribute("value", downloadPathDir));
    writeDomElement(downloadSettingDom, "downloadDLoadLimit", TTKXmlAttribute("value", downloadDLoadLimit));
    writeDomElement(downloadSettingDom, "downloadULoadLimit", TTKXmlAttribute("value", downloadULoadLimit));

    writeDomElement(skinSettingDom, "skinEffectLevel", TTKXmlAttribute("value", skinEffectLevelChoiced));
    writeDomElement(skinSettingDom, "skinFont", TTKXmlAttribute("value", skinFontChoiced));
    writeDomElement(skinSettingDom, "skinSuspension", TTKXmlAttribute("value", skinSuspensionChoiced));
    writeDomElement(skinSettingDom, "skinSuspensionPer", TTKXmlAttribute("value", skinSuspensionPerChoiced));

    QTextStream out(m_file);
    m_document->save(out, 4);
}

QRect DownloadSysConfigManager::readWindowGeometry() const
{
    const QDomNodeList &nodelist = m_document->elementsByTagName("geometry");
    if(nodelist.isEmpty())
    {
        return QRect(0, 0, WINDOW_WIDTH_MIN, WINDOW_HEIGHT_MIN);
    }

    const QDomElement &element = nodelist.at(0).toElement();
    const QStringList &list = element.attribute("value").split(",");
    if(list.count() == 4)
    {
        return QRect(list[0].toInt() < 0 ? 0 : list[0].toInt(),
                     list[1].toInt() < 0 ? 0 : list[1].toInt(),
                     list[2].toInt(), list[3].toInt());
    }
    else
    {
        return QRect(0, 0, WINDOW_WIDTH_MIN, WINDOW_HEIGHT_MIN);
    }
}

void DownloadSysConfigManager::readSysLoadConfig() const
{
    G_SETTING_PTR->setValue(DownloadSettingManager::CloseEventChoiced,
                     readXmlAttributeByTagNameValue("closeEvent").toInt());
    G_SETTING_PTR->setValue(DownloadSettingManager::CloseNetWorkChoiced,
                     readXmlAttributeByTagNameValue("closeNetwork").toInt());
    G_SETTING_PTR->setValue(DownloadSettingManager::FileAssociationChoiced,
                     readXmlAttributeByTagNameValue("fileAssociation").toInt());
    G_SETTING_PTR->setValue(DownloadSettingManager::StartUpModeChoiced,
                     readXmlAttributeByTagNameValue("startUpMode").toInt());
    G_SETTING_PTR->setValue(DownloadSettingManager::StartUpRunModeChoiced,
                     readXmlAttributeByTagNameValue("startUpRunMode").toInt());
    G_SETTING_PTR->setValue(DownloadSettingManager::SlienceRunModeChoiced,
                     readXmlAttributeByTagNameValue("slienceRunMode").toInt());


    G_SETTING_PTR->setValue(DownloadSettingManager::BgThemeChoiced,
                     readXmlAttributeByTagNameValue("bgTheme"));
    G_SETTING_PTR->setValue(DownloadSettingManager::BgTransparentChoiced,
                     readXmlAttributeByTagNameValue("bgTransparent").toInt());


    G_SETTING_PTR->setValue(DownloadSettingManager::DownloadLimitChoiced,
                     readXmlAttributeByTagNameValue("downloadLimit").toInt());
    G_SETTING_PTR->setValue(DownloadSettingManager::DownloadModeChoiced,
                     readXmlAttributeByTagNameValue("downloadMode").toInt());
    G_SETTING_PTR->setValue(DownloadSettingManager::DownloadMaxCountChoiced,
                     readXmlAttributeByTagNameValue("downloadMaxCount").toInt());
    const QString &path = readXmlAttributeByTagNameValue("downloadPathDir");
    G_SETTING_PTR->setValue(DownloadSettingManager::DownloadPathDirChoiced,
                     (path.isEmpty() || !QFile::exists(path)) ? DownloadUtils::Core::downloadPrefix() : path);
    G_SETTING_PTR->setValue(DownloadSettingManager::DownloadDLoadLimitChoiced,
                     readXmlAttributeByTagNameValue("downloadDLoadLimit"));
    G_SETTING_PTR->setValue(DownloadSettingManager::DownloadULoadLimitChoiced,
                     readXmlAttributeByTagNameValue("downloadULoadLimit"));


    G_SETTING_PTR->setValue(DownloadSettingManager::SkinEffectLevelChoiced,
                     readXmlAttributeByTagNameValue("skinEffectLevel"));
    G_SETTING_PTR->setValue(DownloadSettingManager::SkinFontChoiced,
                     readXmlAttributeByTagNameValue("skinFont").toInt());
    G_SETTING_PTR->setValue(DownloadSettingManager::SkinSuspensionChoiced,
                     readXmlAttributeByTagNameValue("skinSuspension"));
    G_SETTING_PTR->setValue(DownloadSettingManager::SkinSuspensionPerChoiced,
                     readXmlAttributeByTagNameValue("skinSuspensionPer").toInt());

}
