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

#ifdef USE_SDL2

#include "utils/sdl2logger.h"

#include "utils/checkutils.h"

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_assert.h>
#include <SDL_log.h>
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

#define logStr(category, msg) \
    case category: \
        message.append(msg); \
        break

// before 2.0.4 this names not exists
#if !SDL_VERSION_ATLEAST(2, 0, 4)
#define SDL_AssertState SDL_assert_state
#define SDL_AssertData SDL_assert_data
#endif  // !SDL_VERSION_ATLEAST(2, 0, 4)

static void logCallback(void *userData A_UNUSED,
                        int category,
                        SDL_LogPriority priority,
                        const char *msg)
{
    std::string message("SDL ERROR:");
    switch (priority)
    {
        logStr(SDL_LOG_PRIORITY_VERBOSE,
            " VERBOSE");
        logStr(SDL_LOG_PRIORITY_DEBUG,
            " DEBUG");
        logStr(SDL_LOG_PRIORITY_INFO,
            " INFO");
        logStr(SDL_LOG_PRIORITY_WARN,
            " WARN");
        logStr(SDL_LOG_PRIORITY_ERROR,
            " ERROR");
        logStr(SDL_LOG_PRIORITY_CRITICAL,
            " CRITICAL");
        case SDL_NUM_LOG_PRIORITIES:
        default:
            message.append(" ?");
            break;
    }

    switch (category)
    {
        logStr(SDL_LOG_CATEGORY_APPLICATION,
            " APPLICATION");
        logStr(SDL_LOG_CATEGORY_ERROR,
            " ERROR");
        logStr(SDL_LOG_CATEGORY_ASSERT,
            " ASSERT");
        logStr(SDL_LOG_CATEGORY_SYSTEM,
            " SYSTEM");
        logStr(SDL_LOG_CATEGORY_AUDIO,
            " AUDIO");
        logStr(SDL_LOG_CATEGORY_VIDEO,
            " VIDEO");
        logStr(SDL_LOG_CATEGORY_RENDER,
            " RENDER");
        logStr(SDL_LOG_CATEGORY_INPUT,
            " INPUT");
        logStr(SDL_LOG_CATEGORY_TEST,
            " TEST");
        logStr(SDL_LOG_CATEGORY_RESERVED1,
            " RESERVED1");
        logStr(SDL_LOG_CATEGORY_RESERVED2,
            " RESERVED2");
        logStr(SDL_LOG_CATEGORY_RESERVED3,
            " RESERVED3");
        logStr(SDL_LOG_CATEGORY_RESERVED4,
            " RESERVED4");
        logStr(SDL_LOG_CATEGORY_RESERVED5,
            " RESERVED5");
        logStr(SDL_LOG_CATEGORY_RESERVED6,
            " RESERVED6");
        logStr(SDL_LOG_CATEGORY_RESERVED7,
            " RESERVED7");
        logStr(SDL_LOG_CATEGORY_RESERVED8,
            " RESERVED8");
        logStr(SDL_LOG_CATEGORY_RESERVED9,
            " RESERVED9");
        logStr(SDL_LOG_CATEGORY_RESERVED10,
            " RESERVED10");
        logStr(SDL_LOG_CATEGORY_CUSTOM,
            " CUSTOM");
        default:
            message.append(" ?");
            break;
    }
    reportAlways("%s %s",
        message.c_str(),
        msg)
}

static SDL_AssertState assertCallback(const SDL_AssertData *data,
                                      void *userdata A_UNUSED)
{
    reportAlways(
        "SDL assert at %s (%s:%d):\n%s",
        data->function,
        data->filename,
        data->linenum,
        data->condition)
    return SDL_ASSERTION_IGNORE;
}

void SDL2Logger::init()
{
#ifdef UNITTESTS
#if SDL_VERSION_ATLEAST(2, 0, 4)
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);
#else  // SDL_VERSION_ATLEAST(2, 0, 4)
    // versions below 2.0.4 report OpenGL error even if OpenGL was not used
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_CRITICAL);
#endif  // SDL_VERSION_ATLEAST(2, 0, 4)
#else  // UNITTESTS

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
#endif  // UNITTESTS

    SDL_LogSetOutputFunction(&logCallback, nullptr);
    SDL_SetAssertionHandler(&assertCallback, nullptr);
}

void SDL2Logger::setLogLevel(const int level)
{
    if (level > 0)
        SDL_LogSetAllPriority(static_cast<SDL_LogPriority>(level));
    else
        SDL_LogResetPriorities();
}

#endif  // USE_SDL2
