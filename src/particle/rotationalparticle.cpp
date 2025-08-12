/*
 *  The ManaVerse Client
 *  Copyright (C) 2006-2009  The Mana World Development Team
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

#include "particle/rotationalparticle.h"

#include "resources/animation/simpleanimation.h"

#include "debug.h"

RotationalParticle::RotationalParticle(Animation *restrict const animation) :
    ImageParticle(nullptr)
{
    mType = ParticleType::Rotational;
    mAnimation = new SimpleAnimation(animation);
}

RotationalParticle::RotationalParticle(XmlNodeConstPtr animationNode,
                                       const std::string &restrict
                                       dyePalettes) :
    ImageParticle(nullptr)
{
    mType = ParticleType::Rotational;
    mAnimation = new SimpleAnimation(animationNode, dyePalettes);
}
