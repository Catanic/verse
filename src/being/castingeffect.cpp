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

#include "being/castingeffect.h"

#include "configuration.h"

#include "const/resources/map/map.h"

#include "gui/userpalette.h"

#include "render/graphics.h"

#include "resources/sprite/animatedsprite.h"

#include "utils/checkutils.h"
#include "utils/delete2.h"

#include "debug.h"

CastingEffect::CastingEffect(const int skillId,
                             const int skillLevel,
                             const std::string &animation,
                             const int x,
                             const int y,
                             const int range) :
    Actor(),
    mSprite(animation.empty() ? nullptr :
        AnimatedSprite::load(paths.getStringValue("sprites") + animation, 0)),
    mRectX((x - range) * mapTileSize),
    mRectY((y - range) * mapTileSize),
    mRectSize(range * mapTileSize * 2 + mapTileSize),
    mAnimationX(mRectX + (mRectSize - (mSprite != nullptr ?
        mSprite->getWidth() : 0)) / 2),
    mAnimationY(mRectY + (mRectSize - (mSprite != nullptr ?
        mSprite->getHeight() : 0)) / 2)
{
    mPixelX = x * mapTileSize;
    mPixelY = y * mapTileSize;
    mYDiff = range * mapTileSize + 31;
    if (mSprite == nullptr)
    {
        reportAlways("Skill %d/%d casting animation '%s' load failed",
            skillId,
            skillLevel,
            animation.c_str())
    }
}

CastingEffect::~CastingEffect()
{
    delete2(mSprite)
}

void CastingEffect::draw(Graphics *const graphics,
                         const int offsetX,
                         const int offsetY) const
{
    graphics->setColor(userPalette->getColorWithAlpha(
        UserColorId::ATTACK_RANGE_BORDER));

    graphics->drawRectangle(Rect(
        mRectX + offsetX,
        mRectY + offsetY,
        mRectSize,
        mRectSize));
    if (mSprite != nullptr)
    {
        mSprite->drawRescaled(graphics,
            mRectX + offsetX,
            mRectY + offsetY,
            mRectSize,
            mRectSize);
    }
}

void CastingEffect::update(const int time)
{
    if (mSprite != nullptr)
        mSprite->update(time);
}

bool CastingEffect::isTerminated() const
{
    if (mSprite != nullptr)
        return mSprite->isTerminated();
    return false;
}
