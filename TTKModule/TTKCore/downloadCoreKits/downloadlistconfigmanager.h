#ifndef DOWNLOADLISTCONFIGMANAGER_H
#define DOWNLOADLISTCONFIGMANAGER_H

/***************************************************************************
 * This file is part of the TTK Downloader project
 * Copyright (C) 2015 - 2023 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "downloadobject.h"
#include "ttkabstractxml.h"

/*! @brief The class of the local download list item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT DownloadItem
{
    QString m_name;
    QString m_url;
};
TTK_DECLARE_LIST(DownloadItem);

/*! @brief The class of the download list manager.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT DownloadListConfigManager : public TTKAbstractXml
{
    Q_OBJECT
    TTK_DECLARE_MODULE(DownloadListConfigManager)
public:
    /*!
     * Object contsructor.
     */
    explicit DownloadListConfigManager(QObject *parent = nullptr);

    /*!
     * Read list download datas from xml file by given name.
     */
    inline bool readListXMLConfig() { return fromFile(LIST_PATH_FULL); }

    /*!
     * Write list download datas into xml file.
     */
    void writeListConfig(const DownloadItemList &records);
    /*!
     * Read list download datas into xml file.
     */
    void readListConfig(DownloadItemList &records);

};

#endif // DOWNLOADLISTCONFIGMANAGER_H
