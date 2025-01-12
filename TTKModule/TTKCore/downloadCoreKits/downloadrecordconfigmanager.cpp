#include "downloadrecordconfigmanager.h"

DownloadRecordConfigManager::DownloadRecordConfigManager(QObject *parent)
    : TTKAbstractXml(parent)
{

}

void DownloadRecordConfigManager::writeDownloadConfig(const DownloadRecordList &records)
{
    if(!toFile(HISTORY_PATH_FULL))
    {
        return;
    }

    createProcessingInstruction();
    QDomElement rootDom = createRoot(APP_NAME);
    QDomElement recordDom = writeDomNode(rootDom, "history");

    for(const DownloadRecord &record : qAsConst(records))
    {
        writeDomMutilElement(recordDom, "value", TTKXmlAttributeList() <<
                                                 TTKXmlAttribute("name", record.m_path) <<
                                                 TTKXmlAttribute("size", record.m_size) <<
                                                 TTKXmlAttribute("time", record.m_time) <<
                                                 TTKXmlAttribute("url", record.m_url));
    }

    QTextStream out(m_file);
    m_document->save(out, 4);
}

void DownloadRecordConfigManager::readDownloadConfig(DownloadRecordList &records)
{
    const QDomNodeList &nodelist = m_document->elementsByTagName("value");
    for(int i = 0; i < nodelist.count(); ++i)
    {
        const QDomElement &element = nodelist.at(i).toElement();
        DownloadRecord record;
        record.m_path = element.attribute("name");
        record.m_time = element.attribute("time");
        record.m_size = element.attribute("size");
        record.m_url = element.attribute("url");
        records << record;
    }
}
