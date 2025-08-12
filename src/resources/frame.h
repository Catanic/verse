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

#ifndef RESOURCES_FRAME_H
#define RESOURCES_FRAME_H

#include "enums/resources/frametype.h"

#include <string>

#include "localconsts.h"

class Image;

/**
 * A single frame in an animation, with a delay and an offset.
 */
struct Frame final
{
    A_DEFAULT_COPY(Frame)

    Image *image;
    int delay;
    int offsetX;
    int offsetY;
    int rand;
    FrameTypeT type;
    std::string nextAction;
};

#endif  // RESOURCES_FRAME_H
