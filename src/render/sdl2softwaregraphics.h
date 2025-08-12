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

#ifndef RENDER_SDL2SOFTWAREGRAPHICS_H
#define RENDER_SDL2SOFTWAREGRAPHICS_H

#ifdef USE_SDL2

#include "render/graphics.h"

#include "localconsts.h"

class Image;
class ImageCollection;
class ImageVertexes;
class MapLayer;

struct SDL_Surface;

/**
 * A central point of control for graphics.
 */
class SDL2SoftwareGraphics final : public Graphics
{
    public:
        friend class Sdl2SoftwareScreenshotHelper;

        /**
         * Constructor.
         */
        SDL2SoftwareGraphics();

        A_DELETE_COPY(SDL2SoftwareGraphics)

        /**
         * Destructor.
         */
        ~SDL2SoftwareGraphics() override final;

        void setRendererFlags(const uint32_t flags)
                              restrict2 noexcept2 override final
        { mRendererFlags = flags; }

        #include "render/graphicsdef.hpp"
        RENDER_GRAPHICSDEF_HPP

        #include "render/softwaregraphicsdef.hpp"
        RENDER_SOFTWAREGRAPHICSDEF_HPP

        bool resizeScreen(const int width,
                          const int height) restrict2 override final;

    protected:
        int SDL_FakeUpperBlit(const SDL_Surface *restrict const src,
                              SDL_Rect *restrict const srcrect,
                              const SDL_Surface *restrict const dst,
                              SDL_Rect *restrict dstrect) const restrict2;

        void drawHLine(int x1, int y, int x2) restrict2;

        void drawVLine(int x, int y1, int y2) restrict2;

        uint32_t mRendererFlags;
        SDL_Surface *mSurface;
        uint32_t mOldPixel;
        unsigned int mOldAlpha;
};

#endif  // USE_SDL2
#endif  // RENDER_SDL2SOFTWAREGRAPHICS_H
