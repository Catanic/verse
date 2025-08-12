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

#include "resources/imageset.h"

#include "logger.h"

#include "resources/image/image.h"

#include "utils/cast.h"
#include "utils/dtor.h"

#include "debug.h"

ImageSet::ImageSet(Image *const img, const int width, const int height,
                   const int margin, const int spacing) :
    Resource(),
    mImages(),
    mWidth(width),
    mHeight(height),
    mOffsetX(0),
    mOffsetY(0)
{
    if (img != nullptr)
    {
        const int hAdd = height + spacing;
        const int wAdd = width + spacing;
        for (int y = margin; y + height <= img->getHeight() - margin;
             y += hAdd)
        {
            for (int x = margin; x + width <= img->getWidth() - margin;
                 x += wAdd)
            {
                Image *const image = img->getSubImage(x, y, width, height);
                image->mNotCount = true;
                mImages.push_back(image);
            }
        }
    }
}

ImageSet::~ImageSet()
{
    delete_all(mImages);
}

Image* ImageSet::get(const size_type i) const
{
    if (i >= mImages.size())
    {
        logger->log("Warning: No sprite %d in this image set",
            CAST_S32(i));
        return nullptr;
    }
    return mImages[i];
}

int ImageSet::calcMemoryLocal() const
{
    return static_cast<int>(sizeof(ImageSet)) +
        Resource::calcMemoryLocal() +
        static_cast<int>(mImages.capacity() * sizeof(Image));
}
