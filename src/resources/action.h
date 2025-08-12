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

#ifndef RESOURCES_ACTION_H
#define RESOURCES_ACTION_H

#include "enums/resources/spritedirection.h"

#include "resources/memorycounter.h"

#include <map>

#include "localconsts.h"

class Animation;

/**
 * An action consists of several animations, one for each direction.
 */
class Action final : public MemoryCounter
{
    public:
        explicit Action(const std::string &name) noexcept2;

        A_DELETE_COPY(Action)

        ~Action() override final;

        void setAnimation(const SpriteDirection::Type direction,
                          Animation *const animation) noexcept2;

        const Animation *getAnimation(SpriteDirection::Type direction) const
                                      noexcept2 A_WARN_UNUSED;

        unsigned getNumber() const noexcept2 A_WARN_UNUSED
        { return mNumber; }

        void setNumber(const unsigned n) noexcept2
        { mNumber = n; }

        void setLastFrameDelay(const int delay) noexcept2;

        int calcMemoryLocal() const override final;

        int calcMemoryChilds(const int level) const override final;

        std::string getCounterName() const override
        { return mCounterName; }

    protected:
        typedef std::map<SpriteDirection::Type, Animation*> Animations;
        typedef Animations::iterator AnimationIter;
        typedef Animations::const_iterator AnimationCIter;

        Animations mAnimations;
        std::string mCounterName;
        unsigned mNumber;
};

#endif  // RESOURCES_ACTION_H
