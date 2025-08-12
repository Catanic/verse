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

#ifndef RESOURCES_DYE_DYEPALETTEPTR_H
#define RESOURCES_DYE_DYEPALETTEPTR_H

#include "localconsts.h"

class DyePalette;

typedef void (DyePalette::*DyeFunctionPtr) (uint32_t *restrict pixels,
                                            const int bufSize) const restrict2;

#endif  // RESOURCES_DYE_DYEPALETTEPTR_H
