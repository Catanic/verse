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

#ifdef USE_SDL2

#include "resources/surfaceimagehelper.h"

#include "resources/image/image.h"

#include "utils/sdlcheckutils.h"

#include "debug.h"

bool SurfaceImageHelper::mEnableAlphaCache = false;

Image *SurfaceImageHelper::loadSurface(SDL_Surface *const tmpImage)
{
    return _SDLload(tmpImage);
}

Image *SurfaceImageHelper::createTextSurface(SDL_Surface *const tmpImage,
                                             const int width A_UNUSED,
                                             const int height A_UNUSED,
                                             const float alpha)
{
    if (!tmpImage)
        return nullptr;

    Image *img;
    bool hasAlpha = false;
    uint8_t *alphaChannel = nullptr;
    SDL_Surface *image = SDLDuplicateSurface(tmpImage);

    img = new Image(image, hasAlpha, alphaChannel);
    img->setAlpha(alpha);
    return img;
}

SDL_Surface* SurfaceImageHelper::SDLDuplicateSurface(SDL_Surface *const
                                                     tmpImage)
{
    if (!tmpImage || !tmpImage->format)
        return nullptr;

    return MSDL_ConvertSurface(tmpImage, tmpImage->format, SDL_SWSURFACE);
}

Image *SurfaceImageHelper::_SDLload(SDL_Surface *tmpImage) const
{
    if (!tmpImage)
        return nullptr;

    SDL_Surface *image = convertTo32Bit(tmpImage);
    return new Image(image, false, nullptr);
}

int SurfaceImageHelper::combineSurface(SDL_Surface *restrict const src,
                                       SDL_Rect *restrict const srcrect,
                                       SDL_Surface *restrict const dst,
                                       SDL_Rect *restrict const dstrect)
{
    SDL_SetSurfaceBlendMode(src, SDL_BLENDMODE_BLEND);
    SDL_BlitSurface(src, srcrect, dst, dstrect);
    return 1;
}

#endif  // USE_SDL2
