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

#ifdef USE_OPENGL

#include "resources/atlas/atlasresource.h"

#include "resources/atlas/atlasmanager.h"
#include "resources/atlas/textureatlas.h"

#include "resources/resourcemanager/resourcemanager.h"

#include "debug.h"

AtlasResource::~AtlasResource()
{
    FOR_EACH (STD_VECTOR<TextureAtlas*>::iterator, it, atlases)
    {
        TextureAtlas *const atlas = *it;
        if (atlas != nullptr)
        {
            FOR_EACH (STD_VECTOR<AtlasItem*>::iterator, it2, atlas->items)
            {
                AtlasItem *const item = *it2;
                if (item != nullptr)
                {
                    Image *const image2 = item->image;
                    if (image2 != nullptr)
                        image2->decRef();
                    delete item;
                }
            }
            Image *const image = atlas->atlasImage;
            if (image != nullptr)
                image->decRef();
            delete atlas;
        }
    }
    ResourceManager::clearDeleted(false);
}

void AtlasResource::incRef()
{
    if (mRefCount == 0U)
        AtlasManager::injectToResources(this);
    Resource::incRef();
}

void AtlasResource::decRef()
{
    Resource::decRef();
    if (mRefCount == 0U)
        AtlasManager::moveToDeleted(this);
}

int AtlasResource::calcMemoryLocal() const
{
    return static_cast<int>(sizeof(AtlasResource)) +
        Resource::calcMemoryLocal() +
        static_cast<int>(atlases.capacity() * sizeof(TextureAtlas*));
}

int AtlasResource::calcMemoryChilds(const int level) const
{
    int sz = 0;
    FOR_EACH (STD_VECTOR<TextureAtlas*>::const_iterator, it, atlases)
    {
        TextureAtlas *const atlas = *it;
        sz += atlas->calcMemory(level + 1);
    }
    return sz;
}

#endif  // USE_OPENGL
