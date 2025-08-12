/*
 *  The ManaVerse Client
 *  Copyright (C) 2013-2020  The ManaPlus Developers
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

#ifndef FS_VIRTFS_H
#define FS_VIRTFS_H

#include "enums/fs/fsentrytype.h"

#include "enums/simpletypes/append.h"

#include "utils/stringvector.h"

#include "localconsts.h"

namespace VirtFs
{
    struct File;
    struct FsEntry;
    struct List;

    void init(const std::string &restrict name);
    void updateDirSeparator();
    const char *getDirSeparator();
    const char *getBaseDir();
    const char *getUserDir();
    bool exists(std::string name);
    List *enumerateFiles(std::string dir) RETURNS_NONNULL;
    bool isDirectory(std::string name);
    bool isSymbolicLink(const std::string &restrict name);
    void freeList(List *restrict const handle);
    File *openRead(std::string filename);
    File *openWrite(std::string filename);
    File *openAppend(std::string filename);
    bool setWriteDir(const std::string &restrict newDir);
    bool mountDir(std::string newDir,
                  const Append append);
    bool mountDir2(std::string newDir,
                   std::string subDir,
                   const Append append);
    bool mountDirSilent(std::string newDir,
                        const Append append);
    bool mountDirSilent2(std::string newDir,
                         std::string subDir,
                         const Append append);
    bool unmountDir(std::string oldDir);
    bool unmountDir2(std::string oldDir,
                     std::string subDir);
    bool unmountDirSilent(std::string oldDir);
    bool unmountDirSilent2(std::string oldDir,
                           std::string subDir);
    bool mountZip(std::string newDir,
                  const Append append);
    bool mountZip2(std::string newDir,
                   std::string subDir,
                   const Append append);
    bool unmountZip(std::string oldDir);
    bool unmountZip2(std::string oldDir,
                     std::string subDir);
    std::string getRealDir(std::string filename);
    bool mkdir(const std::string &restrict dirName);
    bool remove(const std::string &restrict filename);
    bool deinit();
    void permitLinks(const bool val);
    int64_t read(File *restrict const handle,
                 void *restrict const buffer,
                 const uint32_t objSize,
                 const uint32_t objCount);
    int64_t write(File *restrict const file,
                  const void *restrict const buffer,
                  const uint32_t objSize,
                  const uint32_t objCount);
    int close(File *restrict const file);
    int64_t fileLength(File *restrict const file);
    int64_t tell(File *restrict const file);
    int seek(File *restrict const file,
             const uint64_t pos);
    int eof(File *restrict const file);

    bool mountDirInternal(const std::string &restrict newDir,
                          std::string subDir,
                          const Append append);
    bool unmountDirInternal(std::string oldDir,
                            std::string subDir);
    STD_VECTOR<FsEntry*> &getEntries();
    FsEntry *searchByRootInternal(const std::string &restrict root,
                                  const std::string &restrict subDir);
    FsEntry *searchByTypeInternal(const std::string &restrict root,
                                  const FsEntryTypeT type);
    void addEntry(FsEntry *const entry,
                  const Append append);
#ifdef UNITTESTS
    bool mountDirSilentTest(std::string newDir,
                            const Append append);
    bool mountDirSilentTest2(std::string newDir,
                             std::string subDir,
                             const Append append);
#endif  // UNITTESTS
    const char *loadFile(std::string filename,
                         int &restrict fileSize);
    void getFiles(std::string dirName,
                  StringVect &list);
    void getFilesWithDir(std::string dirName,
                         StringVect &list);
    void getDirs(std::string dirName,
                 StringVect &list);
}  // namespace VirtFs

extern const char *dirSeparator;

#endif  // FS_VIRTFS_H
