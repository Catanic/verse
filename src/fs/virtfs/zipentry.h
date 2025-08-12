/*
 *  The ManaVerse Client
 *  Copyright (C) 2017-2020  The ManaPlus Developers
 *  Copyright (C) 2020-2025  The ManaVerse Developers
 *
 *  This file is part of The ManaVerse Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UTILS_VIRTZIPENTRY_H
#define UTILS_VIRTZIPENTRY_H

#include "fs/virtfs/fsentry.h"

#include "utils/vector.h"

#include "localconsts.h"

namespace VirtFs
{

struct ZipLocalHeader;

struct ZipEntry final : public FsEntry
{
    ZipEntry(const std::string &restrict archiveName,
             const std::string &subDir0,
             FsFuncs *restrict const funcs0);

    A_DELETE_COPY(ZipEntry)

    virtual ~ZipEntry();

    STD_VECTOR<ZipLocalHeader*> mHeaders;
    STD_VECTOR<std::string> mDirs;
};

}  // namespace VirtFs

#endif  // UTILS_VIRTZIPENTRY_H
