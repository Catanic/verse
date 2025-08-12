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

#ifndef USE_SDL2

#include "utils/sdlhelper.h"

#include "logger.h"

#include "utils/cast.h"
#include "utils/env.h"
#include "utils/stringutils.h"

#if defined(USE_X11) && defined(USE_OPENGL)
#include "utils/glxhelper.h"
#endif  // defined(USE_X11) && defined(USE_OPENGL)

PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL_events.h>
#include <SDL_syswm.h>
PRAGMA48(GCC diagnostic pop)

#include "debug.h"

bool SDL::getAllVideoModes(StringVect &modeList)
{
    /* Get available fullscreen/hardware modes */
    SDL_Rect *const *const modes = SDL_ListModes(nullptr,
        SDL_FULLSCREEN | SDL_HWSURFACE);

#ifdef ANDROID
    const std::string modeString =
        toString(CAST_S32(modes[0]->w)).append("x")
       .append(toString(CAST_S32(modes[0]->h)));
    logger->log("support mode: " + modeString);
    modeList.push_back(modeString);
    return true;
#else  // ANDROID

    /* Check which modes are available */
    if (modes == static_cast<SDL_Rect **>(nullptr))
    {
        logger->log1("No modes available");
        return false;
    }
    else if (modes == reinterpret_cast<SDL_Rect **>(-1))
    {
        logger->log1("All resolutions available");
        return true;
    }
    else
    {
        for (int i = 0; modes[i] != nullptr; ++ i)
        {
            const std::string modeString =
                toString(CAST_S32(modes[i]->w)).append("x")
                .append(toString(CAST_S32(modes[i]->h)));
            logger->log("support mode: " + modeString);
            modeList.push_back(modeString);
        }
        return true;
    }
#endif  // ANDROID
}

void SDL::SetWindowTitle(const SDL_Surface *const window A_UNUSED,
                         const char *const title)
{
    SDL_WM_SetCaption(title, nullptr);
}

void SDL::SetWindowIcon(const SDL_Surface *const window A_UNUSED,
                        SDL_Surface *const icon)
{
    SDL_WM_SetIcon(icon, nullptr);
}

void SDL::grabInput(const SDL_Surface *const window A_UNUSED, const bool grab)
{
    SDL_WM_GrabInput(grab ? SDL_GRAB_ON : SDL_GRAB_OFF);
}

void SDL::setGamma(const SDL_Surface *const window A_UNUSED, const float gamma)
{
    SDL_SetGamma(gamma, gamma, gamma);
}

void SDL::setVsync(const int val)
{
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, val);
}

bool SDL::getWindowWMInfo(const SDL_Surface *const window A_UNUSED,
                          SDL_SysWMinfo *const info)
{
    return SDL_GetWMInfo(info) != 0;
}

SDL_Thread *SDL::createThread(int (SDLCALL *fn)(void *),
                              const char *const name A_UNUSED,
                              void *const data)
{
    return SDL_CreateThread(fn, data);
}

#if defined(USE_X11) && defined(USE_OPENGL)
void *SDL::createGLContext(SDL_Surface *const window A_UNUSED,
                           const int major,
                           const int minor,
                           const int profile)
{
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version)
    SDL_GetWMInfo(&info);
    void *context = GlxHelper::createContext(info.info.x11.window,
        info.info.x11.display, major, minor, profile);
    if (!context && (major > 3 || (major == 3 && minor > 3)))
    {
        logger->log("Try fallback to OpenGL 3.3 core context");
        context = GlxHelper::createContext(info.info.x11.window,
            info.info.x11.display, 3, 3, profile);
        if (!context && profile == 0x01)
        {
            logger->log("Try fallback to OpenGL 3.3 compatibility context");
            context = GlxHelper::createContext(info.info.x11.window,
                info.info.x11.display, 3, 3, 0x02);
        }
    }
    if (!context && (major > 3 || (major == 3 && minor > 0)))
    {
        logger->log("Try fallback to OpenGL 3.0 core context");
        context = GlxHelper::createContext(info.info.x11.window,
            info.info.x11.display, 3, 0, profile);
    }
    if (!context && (major > 2 || (major == 2 && minor > 1)))
    {
        logger->log("Try fallback to OpenGL 2.1 compatibility context");
        context = GlxHelper::createContext(info.info.x11.window,
            info.info.x11.display, 2, 1, 0x02);
    }
    return context;
}

void SDL::makeCurrentContext(void *const context)
{
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version)
    SDL_GetWMInfo(&info);
    GlxHelper::makeCurrent(info.info.x11.window,
        info.info.x11.display,
        context);
}
#else  // defined(USE_X11) && defined(USE_OPENGL)

void *SDL::createGLContext(SDL_Surface *const window A_UNUSED,
                           const int major A_UNUSED,
                           const int minor A_UNUSED,
                           const int profile A_UNUSED)
{
    return nullptr;
}

void SDL::makeCurrentContext(void *const context A_UNUSED)
{
}
#endif  // defined(USE_X11) && defined(USE_OPENGL)

void SDL::initLogger()
{
}

void SDL::setLogLevel(const int level A_UNUSED)
{
}

void SDL::WaitThread(SDL_Thread *const thread)
{
    if (thread != nullptr && SDL_GetThreadID(thread) != 0U)
        SDL_WaitThread(thread, nullptr);
}

bool SDL::PollEvent(SDL_Event *event)
{
    SDL_PumpEvents();
    return SDL_PeepEvents(event,
        1,
        SDL_GETEVENT,
        SDL_ALLEVENTS) > 0;
}

void SDL::allowScreenSaver(const bool allow)
{
    if (allow)
        setEnv("SDL_VIDEO_ALLOW_SCREENSAVER", "1");
    else
        setEnv("SDL_VIDEO_ALLOW_SCREENSAVER", "0");
}

#endif  // USE_SDL2
