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

#include "fs/virtfs/direntry.h"

#include "debug.h"

namespace VirtFs
{

DirEntry::DirEntry(const std::string &userDir0,
                   const std::string &rootDir,
                   const std::string &subDir0,
                   const std::string &rootSubDir0,
                   FsFuncs *restrict const funcs0) :
    FsEntry(FsEntryType::Dir, funcs0),
    userDir(userDir0),
    rootSubDir(rootSubDir0)
{
    root = rootDir;
    subDir = subDir0;
}

DirEntry::~DirEntry()
{
}

}  // namespace VirtFs
