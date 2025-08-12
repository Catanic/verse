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

#include "fs/virtfs/zipreader.h"

#include "fs/paths.h"

#include "fs/virtfs/zipentry.h"
#include "fs/virtfs/ziplocalheader.h"

#include "utils/cast.h"
#include "utils/checkutils.h"
#include "utils/delete2.h"
#include "utils/stringutils.h"

#include <zlib.h>
PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_endian.h>
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

#ifndef SDL_BIG_ENDIAN
#error missing SDL_endian.h
#endif  // SDL_BYTEORDER

// #define DEBUG_ZIP

extern const char *dirSeparator;

#define readVal(val, sz, msg) \
    cnt = fread(static_cast<void*>(val), 1, sz, arcFile); \
    if (cnt != (sz)) \
    { \
        reportAlways("Error reading " msg " in file %s", \
            archiveName.c_str()) \
        delete2(header) \
        delete [] buf; \
        fclose(arcFile); \
        return false; \
    }

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define swapVal16(val) val = __builtin_bswap16(val);
#define swapVal32(val) val = __builtin_bswap32(val);
#else  // SDL_BYTEORDER == SDL_BIG_ENDIAN
#define swapVal16(val)
#define swapVal32(val)
#endif  // SDL_BYTEORDER == SDL_BIG_ENDIAN

namespace VirtFs
{

namespace ZipReader
{
    bool readArchiveInfo(ZipEntry *const entry)
    {
        if (entry == nullptr)
        {
            reportAlways("Entry is null.")
            return false;
        }
        const std::string archiveName = entry->root;
        STD_VECTOR<ZipLocalHeader*> &restrict headers = entry->mHeaders;
        STD_VECTOR<std::string> &restrict dirs = entry->mDirs;
        FILE *restrict const arcFile = fopen(archiveName.c_str(),
            "rb");
        if (arcFile == nullptr)
        {
            reportAlways("Can't open zip file %s",
                archiveName.c_str())
            return false;
        }
        uint8_t *const buf = new uint8_t[65535 + 10];
        uint16_t val16 = 0U;
        uint16_t method = 0U;
        ZipLocalHeader *header = nullptr;

#ifdef DEBUG_ZIP
        logger->log("Read archive: %s", archiveName.c_str());
#endif  // DEBUG_ZIP

        // format source https://en.wikipedia.org/wiki/Zip_%28file_format%29
        while (feof(arcFile) == 0)
        {
            size_t cnt = 0U;
            // file header pointer on 0
            // read file header signature
            readVal(buf, 4, "zip file header")  // + 4
            // pointer on 4

            if (buf[0] == 0x50 &&
                buf[1] == 0x4B &&
                buf[2] == 0x03 &&
                buf[3] == 0x04)
            {   // local file header
                header = new ZipLocalHeader;
                header->zipEntry = entry;
                // skip useless fields
                fseek(arcFile, 4, SEEK_CUR);  // + 4
                // file header pointer on 8
                readVal(&method, 2, "compression method")  // + 2
                swapVal16(method)
                header->compressed = (method != 0);
                // file header pointer on 10
                fseek(arcFile, 8, SEEK_CUR);  // + 8
                // file header pointer on 18
                readVal(&header->compressSize, 4,
                    "zip compressed size")  // + 4
                swapVal32(header->compressSize)
                // file header pointer on 22
                readVal(&header->uncompressSize, 4,
                    "zip uncompressed size")  // + 4
                swapVal32(header->uncompressSize)
                // file header pointer on 26
                readVal(&val16, 2, "file name length")  // + 2
                swapVal16(val16)
                // file header pointer on 28
                const uint32_t fileNameLen = CAST_U32(val16);
                if (fileNameLen > 1000)
                {
                    reportAlways("Error too long file name in file %s",
                        archiveName.c_str())
                    delete header;
                    delete [] buf;
                    fclose(arcFile);
                    return false;
                }
                readVal(&val16, 2, "extra field length")  // + 2
                swapVal16(val16)
                // file header pointer on 30
                const uint32_t extraFieldLen = CAST_U32(val16);
                readVal(buf, fileNameLen, "file name")
                // file header pointer on 30 + fileNameLen
                buf[fileNameLen] = 0;
                header->fileName = std::string(
                    reinterpret_cast<char*>(buf));
                prepareFsPath(header->fileName);
                header->dataOffset = CAST_S32(ftell(arcFile) + extraFieldLen);
                fseek(arcFile, extraFieldLen + header->compressSize, SEEK_CUR);
                // pointer on 30 + fileNameLen + extraFieldLen + compressSize
                if (findLast(header->fileName, dirSeparator) == false)
                {
                    headers.push_back(header);
#ifdef DEBUG_ZIP
                    logger->log(" file name: %s",
                        header->fileName.c_str());
                    logger->log(" compression method: %u",
                        CAST_U32(method));
                    logger->log(" compressed size: %u",
                        header->compressSize);
                    logger->log(" uncompressed size: %u",
                        header->uncompressSize);
#endif  // DEBUG_ZIP
                }
                else
                {
#ifdef DEBUG_ZIP
                    logger->log(" dir name: %s",
                        header->fileName.c_str());
#endif  // DEBUG_ZIP
                    dirs.push_back(header->fileName);
                    delete2(header)
                }
            }
            else if (buf[0] == 0x50 &&
                     buf[1] == 0x4B &&
                     buf[2] == 0x01 &&
                     buf[3] == 0x02)
            {   // central directory file header
                // !!! This is quick way for read zip archives. !!!
                // !!! It ignore modified files in archive. !!!
                // ignoring central directory entries
                break;
            }
            else if (buf[0] == 0x50 &&
                     buf[1] == 0x4B &&
                     buf[2] == 0x05 &&
                     buf[3] == 0x06)
            {   // end of central directory
                // !!! This is quick way for read zip archives. !!!
                // !!! It ignore modified files in archive. !!!
                // ignoring end of central directory
                break;
            }
            else
            {
                reportAlways("Error in header signature (0x%02x%02x%02x%02x)"
                    " in file %s",
                    buf[0],
                    buf[1],
                    buf[2],
                    buf[3],
                    archiveName.c_str())
                delete [] buf;
                fclose(arcFile);
                return false;
            }
        }
        delete [] buf;
        fclose(arcFile);
        return true;
    }

    void reportZlibError(const std::string &text,
                         const int err)
    {
        reportAlways("Zlib error: '%s' in %s",
            text.c_str(),
            getZlibError(err).c_str())
    }

    std::string getZlibError(const int err)
    {
        switch (err)
        {
            case Z_OK:
                return std::string();
            default:
                return "unknown zlib error";
        }
    }

    uint8_t *readCompressedFile(const ZipLocalHeader *restrict const header)
    {
        if (header == nullptr)
        {
            reportAlways("ZipReader::readCompressedFile: header is null")
            return nullptr;
        }
        FILE *restrict const arcFile = fopen(
            header->zipEntry->root.c_str(),
            "rb");
        if (arcFile == nullptr)
        {
            reportAlways("Can't open zip file %s",
                header->zipEntry->root.c_str())
            return nullptr;
        }

        fseek(arcFile, header->dataOffset, SEEK_SET);
        const uint32_t compressSize = header->compressSize;
        uint8_t *const buf = new uint8_t[compressSize];
        if (fread(static_cast<void*>(buf), 1, compressSize, arcFile) !=
            compressSize)
        {
            reportAlways("Read zip compressed file error from archive: %s",
                header->zipEntry->root.c_str())
            fclose(arcFile);
            delete [] buf;
            return nullptr;
        }
        fclose(arcFile);
        return buf;
    }

    const uint8_t *readFile(const ZipLocalHeader *restrict const header)
    {
        if (header == nullptr)
        {
            reportAlways("Open zip file error. header is null.")
            return nullptr;
        }
        uint8_t *restrict const in = readCompressedFile(header);
        if (in == nullptr)
            return nullptr;
        if (header->compressed == false)
            return in;  //  return as is if data not compressed
        const size_t outSize = header->uncompressSize;
        uint8_t *restrict const out = new uint8_t[outSize];
        if (outSize == 0)
        {
            delete [] in;
            return out;
        }

        z_stream strm;
        strm.zalloc = nullptr;
        strm.zfree = nullptr;
        strm.opaque = nullptr;
        strm.next_in = in;
        strm.avail_in = header->compressSize;
        strm.next_out = out;
        strm.avail_out = CAST_U32(outSize);

PRAGMACLANG6GCC(GCC diagnostic push)
PRAGMACLANG6GCC(GCC diagnostic ignored "-Wold-style-cast")
        int ret = inflateInit2(&strm, -MAX_WBITS);
PRAGMACLANG6GCC(GCC diagnostic pop)

        if (ret != Z_OK)
        {
            reportZlibError(header->zipEntry->root, ret);
            delete [] in;
            delete [] out;
            return nullptr;
        }
        ret = inflate(&strm, Z_FINISH);
//        ret = inflate(&strm, Z_SYNC_FLUSH);
        if (ret != Z_OK &&
            ret != Z_STREAM_END)
        {
            reportZlibError("file decompression error",
                ret);
            inflateEnd(&strm);
            delete [] in;
            delete [] out;
            return nullptr;
        }
        inflateEnd(&strm);
        delete [] in;
        return out;
    }
}  // namespace ZipReader

}  // namespace VirtFs
