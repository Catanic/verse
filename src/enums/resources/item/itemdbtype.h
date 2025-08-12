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

#ifndef ENUMS_RESOURCES_ITEM_ITEMDBTYPE_H
#define ENUMS_RESOURCES_ITEM_ITEMDBTYPE_H

#include "enums/simpletypes/enumdefines.h"

enumStart(ItemDbType)
{
    UNUSABLE = 0,
    USABLE,
    EQUIPMENT_ONE_HAND_WEAPON,
    EQUIPMENT_TWO_HANDS_WEAPON,
    EQUIPMENT_TORSO,
    EQUIPMENT_ARMS,  // 5
    CARD,
    EQUIPMENT_HEAD,
    EQUIPMENT_LEGS,
    EQUIPMENT_SHIELD,
    EQUIPMENT_RING,
    EQUIPMENT_NECKLACE,  // 10
    EQUIPMENT_FEET,
    EQUIPMENT_AMMO,
    EQUIPMENT_CHARM,
    SPRITE_RACE,
    SPRITE_HAIR  // 15
}
enumEnd(ItemDbType);

#endif  // ENUMS_RESOURCES_ITEM_ITEMDBTYPE_H
