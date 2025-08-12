/*
 *  The ManaVerse Client
 *  Copyright (C) 2010  The Mana Developers
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

#ifndef RESOURCES_SPRITE_IMAGESPRITE_H
#define RESOURCES_SPRITE_IMAGESPRITE_H

#include "resources/sprite/sprite.h"

#include "resources/image/image.h"

class Graphics;

class ImageSprite final : public Sprite
{
    public:
        explicit ImageSprite(Image *const image);

        A_DELETE_COPY(ImageSprite)

        ~ImageSprite() override final;

        bool reset() override final
        { return false; }

        bool play(const std::string &action A_UNUSED) override final
        { return false; }

        bool update(const int time A_UNUSED) override final
        { return false; }

        void draw(Graphics *const graphics,
                  const int posX, const int posY)
                  const override final A_NONNULL(2);

        int getWidth() const override final A_WARN_UNUSED
        { return mImage != nullptr ? mImage->getWidth() : 0; }

        int getHeight() const override final A_WARN_UNUSED
        { return mImage != nullptr ? mImage->getHeight() : 0; }

        const Image* getImage() const override final A_WARN_UNUSED
        { return mImage; }

        bool setSpriteDirection(const SpriteDirection::Type
                                direction A_UNUSED) override final
        { return false; }

        int getNumberOfLayers() const A_WARN_UNUSED
        { return 1; }

        unsigned int getCurrentFrame() const override final A_WARN_UNUSED
        { return 0; }

        unsigned int getFrameCount() const override final A_WARN_UNUSED
        { return 1; }

        bool updateNumber(const unsigned num A_UNUSED) override final
        { return false; }

    private:
        Image *mImage;
};

#endif  // RESOURCES_SPRITE_IMAGESPRITE_H
