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

#ifndef ENUMS_BEING_COOKINGTYPE_H
#define ENUMS_BEING_COOKINGTYPE_H

#include "enums/simpletypes/enumdefines.h"

enumStart(CookingType)
{
    Cooking    = 1,
    Arrow      = 2,
    Elemental  = 3,
    MixCooking = 4,
    MakeBomb   = 5,
    Pharmacy   = 6
}
enumEnd(CookingType);

#endif  // ENUMS_BEING_COOKINGTYPE_H
