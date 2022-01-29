#ifndef TTKVERSION_H
#define TTKVERSION_H
 
/***************************************************************************
 * This file is part of the TTK Downloader project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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

//update time 2022.01.29
#define DOWNLOAD_VERSION_STR    "2.4.0.0"
#define DOWNLOAD_VERSION_WSTR   L"2.4.0.0"
#define DOWNLOAD_VER_TIME_STR   "(2022/01/29)"

#define DOWNLOAD_MAJOR_VERSION 2
#define DOWNLOAD_MIDLE_VERSION 4
#define DOWNLOAD_MINOR_VERSION 0
#define DOWNLOAD_PATCH_VERSION 0

#define DOWNLOAD_VERSION 0x02400

#define TTK_VERSION_CHECK(major, middle, minor, patch) ((major<<12)|(middle<<8)|(minor<<4)|(patch))

#endif // TTKVERSION_H
