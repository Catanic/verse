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

#ifdef USE_OPENGL

#include "resources/atlas/atlasmanager.h"
#include "resources/atlas/atlasresource.h"

#include "resources/loaders/emptyatlasloader.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "utils/checkutils.h"

#include "debug.h"

struct EmptyAtlasLoader final
{
    A_DEFAULT_COPY(EmptyAtlasLoader)

    const std::string name;
    const StringVect *const files;

    static Resource *load(const void *const v)
    {
        if (v == nullptr)
            return nullptr;

        const EmptyAtlasLoader *const rl =
            static_cast<const EmptyAtlasLoader *>(v);
        AtlasResource *const resource = AtlasManager::loadEmptyAtlas(
            rl->name, *rl->files);
        if (resource == nullptr)
            reportAlways("Empty atlas creation error: %s", rl->name.c_str())
        return resource;
    }
};

Resource *Loader::getEmptyAtlas(const std::string &name,
                                const StringVect &files)
{
    EmptyAtlasLoader rl = { name, &files };
    return ResourceManager::get("atlas_" + name, EmptyAtlasLoader::load, &rl);
}

#endif  // USE_OPENGL
