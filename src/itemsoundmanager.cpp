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

#include "itemsoundmanager.h"

#include "soundmanager.h"

#include "being/flooritem.h"
#include "being/localplayer.h"

#include "resources/iteminfo.h"

#include "resources/item/item.h"

#include "debug.h"

void ItemSoundManager::playSfx(const Item *const item,
                               const ItemSoundEvent::Type sound)
{
    if (item == nullptr)
        return;
    playSfx(item->getId(), sound);
}

void ItemSoundManager::playSfx(const int itemId,
                               const ItemSoundEvent::Type sound)
{
    playSfx(ItemDB::get(itemId), sound);
}

void ItemSoundManager::playSfx(const FloorItem *const item,
                               const ItemSoundEvent::Type sound)
{
    if (item == nullptr)
        return;
    playSfx(ItemDB::get(item->getItemId()), sound);
}

std::string ItemSoundManager::getSoundEffect(const Being *const being,
                                             const ItemInfo &info,
                                             const ItemSoundEvent::Type sound)
{
    std::string sfx = info.getSound(sound).sound;
    if (sfx.empty())
    {
        if (being == nullptr)
            return std::string();

        // fallback to player race sound if no item sound.
        const int id = -100 - toInt(being->getSubType(), int);
        const ItemInfo &info2 = ItemDB::get(id);
        sfx = info2.getSound(sound).sound;
    }
    return sfx;
}

void ItemSoundManager::playSfx(const ItemInfo &info,
                               const ItemSoundEvent::Type sound)
{
    soundManager.playSfx(getSoundEffect(localPlayer, info, sound),
        0,
        0);
}

void ItemSoundManager::playSfx(const Being *const being,
                               const int itemId,
                               const ItemSoundEvent::Type sound)
{
    if (being == nullptr)
        return;

    soundManager.playSfx(getSoundEffect(being, ItemDB::get(itemId), sound),
        being->getTileX(),
        being->getTileY());
}
