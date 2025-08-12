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

#if defined(USE_OPENGL) && defined(USE_X11)

#include "render/openglx/mglxinit.h"

#include "render/opengl/mglfunctions.h"

#include "render/openglx/mglx.h"

#if defined(USE_SDL2) && defined(__SWITCH__)
#include <SDL_video.h>
#endif  // defined(USE_SDL2) && defined(__SWITCH__)

#include "debug.h"

void Glx::initFunctions()
{
    assignFunction(glXCreateContext)
    assignFunction(glXGetCurrentContext)
    assignFunctionARB(glXCreateContextAttribs)
    assignFunction(glXChooseFBConfig)
    assignFunction(glXDestroyContext)
    assignFunction(glXMakeCurrent)
    assignFunction(glXSwapBuffers)
}

#endif  // defined(USE_OPENGL) && defined(USE_X11)
