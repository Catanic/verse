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

#ifndef GUI_WIDGETS_SKILLDATA_H
#define GUI_WIDGETS_SKILLDATA_H

#include "resources/missileinfo.h"
#include "resources/soundinfo.h"

class Image;

struct SkillData final
{
    std::string name;
    std::string shortName;
    std::string dispName;
    std::string description;

    MissileInfo missile;
    MissileInfo castingMissile;
    std::string invokeCmd;
    std::string castingAnimation;

    SoundInfo soundHit;
    SoundInfo soundMiss;

    Image *icon;

    int updateEffectId;
    int removeEffectId;
    int hitEffectId;
    int missEffectId;
    int castingSrcEffectId;
    int castingDstEffectId;
    int castingGroundEffectId;
    int srcEffectId;
    int dstEffectId;
    bool haveIcon;
    bool autoTab;

    SkillData();
    A_DELETE_COPY(SkillData)
    ~SkillData();

    void setIcon(const std::string &iconPath);
};

#endif  // GUI_WIDGETS_SKILLDATA_H
