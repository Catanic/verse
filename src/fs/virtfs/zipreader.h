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

#ifndef UTILS_ZIP_H
#define UTILS_ZIP_H

#include "localconsts.h"

#include <string>

namespace VirtFs
{

struct ZipEntry;
struct ZipLocalHeader;

namespace ZipReader
{
    bool readArchiveInfo(ZipEntry *const entry);
    std::string getZlibError(const int err);
    void reportZlibError(const std::string &text,
                         const int err);
    uint8_t *readCompressedFile(const ZipLocalHeader *restrict const header);
    const uint8_t *readFile(const ZipLocalHeader *restrict const header);
}  // namespace ZipReader

}  // namespace VirtFs

#endif  // UTILS_ZIP_H
