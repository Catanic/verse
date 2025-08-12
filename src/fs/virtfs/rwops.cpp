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

/*
 * This code provides a glue layer between PhysicsFS and Simple Directmedia
 *  Layer's (SDL) RWops i/o abstraction.
 *
 * License: this code is public domain. I make no warranty that it is useful,
 *  correct, harmless, or environmentally safe.
 *
 * This particular file may be used however you like, including copying it
 *  verbatim into a closed-source project, exploiting it commercially, and
 *  removing any trace of my name from the source (although I hope you won't
 *  do that). I welcome enhancements and corrections to this file, but I do
 *  not require you to send me patches if you make changes. This code has
 *  NO WARRANTY.
 *
 * Unless otherwise stated, the rest of PhysicsFS falls under the zlib license.
 *  Please see LICENSE.txt in the root of the source tree.
 *
 * SDL falls under the LGPL license. You can get SDL at http://www.libsdl.org/
 *
 *  This file was written by Ryan C. Gordon. (icculus@icculus.org).
 *
 *  Copyright (C) 2012-2020  The ManaPlus Developers
 *  Copyright (C) 2020-2025  The ManaVerse Developers
 */

#include "fs/virtfs/rwops.h"

#include "fs/virtfs/file.h"
#include "fs/virtfs/fs.h"
#include "fs/virtfs/fsfuncs.h"

#include "utils/checkutils.h"
#include "utils/fuzzer.h"
#include "utils/performance.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_rwops.h>
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

namespace VirtFs
{

SDL_RWops *create_rwops(File *const file,
                        const std::string &restrict fname)
{
    SDL_RWops *retval = nullptr;

    if (file == nullptr)
    {
        reportAlways("VirtFs::rwops_seek: create rwops error: %s",
            fname.c_str())
    }
    else
    {
        retval = SDL_AllocRW();
        if (retval != nullptr)
        {
#ifdef USE_SDL2
            retval->size  = file->funcs->rwops_size;
#endif  // USE_SDL2

            retval->seek  = file->funcs->rwops_seek;
            retval->read  = file->funcs->rwops_read;
            retval->write = file->funcs->rwops_write;
            retval->close = file->funcs->rwops_close;
            retval->hidden.unknown.data1 = file;
        } /* if */
    } /* else */

    return retval;
} /* VirtFs::create_rwops */

#ifdef __APPLE__
static bool checkFilePath(const std::string &restrict fname)
{
    if (fname.empty())
        return false;
    if (!exists(fname) || isDirectory(fname))
        return false;
    return true;
}
#endif  // __APPLE__

SDL_RWops *rwopsOpenRead(const std::string &restrict fname)
{
    BLOCK_START("RWopsopenRead")
#ifdef __APPLE__
    if (!checkFilePath(fname))
        return nullptr;
#endif  // __APPLE__
#ifdef USE_FUZZER
    if (Fuzzer::conditionTerminate(fname))
        return nullptr;
#endif  // USE_FUZZER
#ifdef USE_PROFILER

    SDL_RWops *const ret = create_rwops(openRead(fname),
        fname);

    BLOCK_END("RWopsopenRead")
    return ret;
#else  // USE_PROFILER

    return create_rwops(openRead(fname),
        fname);
#endif  // USE_PROFILER
} /* RWopsopenRead */

SDL_RWops *rwopsOpenWrite(const std::string &restrict fname)
{
#ifdef __APPLE__
    if (!checkFilePath(fname))
        return nullptr;
#endif  // __APPLE__

    return create_rwops(openWrite(fname),
        fname);
} /* RWopsopenWrite */

SDL_RWops *rwopsOpenAppend(const std::string &restrict fname)
{
#ifdef __APPLE__
    if (!checkFilePath(fname))
        return nullptr;
#endif  // __APPLE__

    return create_rwops(openAppend(fname),
        fname);
} /* RWopsopenAppend */

}  // namespace VirtFs

/* end of virtfsrwops.c ... */
