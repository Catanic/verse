/*
 *  The ManaVerse Client
 *  Copyright (C) 2011-2020  The ManaPlus Developers
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

#include "utils/env.h"

#include "configuration.h"
#include "logger.h"
#include "settings.h"

#include "debug.h"

void updateEnv()
{
#if defined(_WIN32) || defined(__APPLE__)
    if (config.getBoolValue("centerwindow"))
        setEnv("SDL_VIDEO_CENTERED", "1");
    else
        setEnv("SDL_VIDEO_CENTERED", "0");
#endif  // defined(_WIN32) || defined(__APPLE__)

#ifndef _WIN32
    const int vsync = settings.options.test.empty()
        ? config.getIntValue("vsync") : 1;
    // __GL_SYNC_TO_VBLANK is nvidia variable.
    // vblank_mode is MESA variable.
    switch (vsync)
    {
        case 1:
            setEnv("__GL_SYNC_TO_VBLANK", "0");
            setEnv("vblank_mode", "0");
            break;
        case 2:
            setEnv("__GL_SYNC_TO_VBLANK", "1");
            setEnv("vblank_mode", "2");
            break;
        default:
            break;
    }
#endif  // _WIN32
}

void setEnv(const char *const name, const char *const value)
{
    if ((name == nullptr) || (value == nullptr))
        return;
#ifdef _WIN32
    if (putenv(const_cast<char*>((std::string(name)
        + "=" + value).c_str())))
#else  // _WIN32

    if (setenv(name, value, 1) != 0)
#endif  // _WIN32
    {
        logger->log("setenv failed: %s=%s", name, value);
    }
}
