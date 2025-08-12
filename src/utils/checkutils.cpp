/*
 *  The ManaVerse Client
 *  Copyright (C) 2012-2020  The ManaPlus Developers
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

#ifdef ENABLE_ASSERTS

#include "utils/checkutils.h"

#include "logger.h"

#ifdef HAVE_EXECINFO
#include <execinfo.h>
#endif  // HAVE_EXECINFO

#include "debug.h"

void reportAssertStack(const char *const file,
                       const unsigned int line,
                       const char *const func,
                       const char *const name,
                       const char *const text)
{
    logger->log("--- Assert: %s --------------------------------------------",
        name);
    logger->assertLog("%s:%u: '%s' in function `%s'",
        file,
        line,
        text,
        func);
#ifdef HAVE_EXECINFO
    reportStack();
#endif  // HAVE_EXECINFO
}

void reportLogStack(const char *const file,
                    const unsigned int line,
                    const char *const func)
{
    logger->assertLog("%s:%u: in function `%s'",
        file,
        line,
        func);
#ifdef HAVE_EXECINFO
    reportStack();
#endif  // HAVE_EXECINFO
}

void reportStack()
{
#ifdef HAVE_EXECINFO
    void *array[15];
    const int size = static_cast<int>(backtrace(array, 15));
    char **strings = backtrace_symbols(array, size);
    for (int i = 0; i < size; i++)
        logger->log1(strings[i]);
    free(strings);
#endif  // HAVE_EXECINFO
}

#endif  // ENABLE_ASSERTS
