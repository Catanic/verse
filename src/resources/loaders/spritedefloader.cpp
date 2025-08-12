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

#include "settings.h"

#include "resources/loaders/spritedefloader.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "resources/sprite/spritedef.h"

#include "utils/stringutils.h"

#include "debug.h"

namespace
{
    struct SpriteDefLoader final
    {
        A_DEFAULT_COPY(SpriteDefLoader)

        const std::string path;
        const int variant;
        static Resource *load(const void *const v)
        {
            if (v == nullptr)
                return nullptr;

            const SpriteDefLoader *const
                rl = static_cast<const SpriteDefLoader *>(v);
            return SpriteDef::load(rl->path,
                rl->variant,
                settings.uselonglivesprites);
        }
    };
}  // namespace

SpriteDef *Loader::getSprite(const std::string &path,
                             const int variant)
{
    SpriteDefLoader rl = { path, variant};
    const std::string str = std::string(
        "sprite_").append(
        path).append(
        "[").append(toString(
        variant).append(
        "]"));
    return static_cast<SpriteDef*>(ResourceManager::get(str,
        SpriteDefLoader::load, &rl));
}
