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

#include "resources/sprite/animationdelayload.h"

#include "const/resources/spriteaction.h"

#include "resources/loaders/spritedefloader.h"

#include "resources/sprite/animatedsprite.h"

#include "debug.h"

AnimationDelayLoad::AnimationDelayLoad(const std::string &fileName,
                                       const int variant,
                                       AnimatedSprite *const sprite) :
    mFileName(fileName),
    mVariant(variant),
    mSprite(sprite),
    mAction(SpriteAction::STAND)
{
}

AnimationDelayLoad::~AnimationDelayLoad()
{
    if (mSprite != nullptr)
    {
        mSprite->clearDelayLoad();
        mSprite = nullptr;
    }
}

void AnimationDelayLoad::clearSprite()
{
    mSprite = nullptr;
}

void AnimationDelayLoad::load()
{
    if (mSprite != nullptr)
    {
        SpriteDef *const s = Loader::getSprite(mFileName, mVariant);
        if (s == nullptr)
            return;
        mSprite->setSprite(s);
        mSprite->play(mAction);
    }
}
