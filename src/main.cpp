/*
 *  The ManaVerse Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifdef TRACY_ENABLE
#include "Tracy.hpp"
#endif

#include "maingui.h"

#ifdef _WIN32
PRAGMA48(GCC diagnostic push)
PRAGMA48(GCC diagnostic ignored "-Wshadow")
#include <SDL.h>
PRAGMA48(GCC diagnostic pop)
#endif  // _WIN32

#include "debug.h"

#if !defined(UNITTESTS) && !defined(ANDROID)
#if defined __native_client__
extern "C"
#endif  // defined __native_client__

int main(int argc, char *argv[])
{
#ifdef TRACY_ENABLE
    ZoneScopedN("Main");
#endif
    return mainGui(argc, argv);
}
#endif  // !defined(UNITTESTS) && !defined(ANDROID)
