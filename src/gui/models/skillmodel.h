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

#ifndef GUI_MODELS_SKILLMODEL_H
#define GUI_MODELS_SKILLMODEL_H

#include "gui/models/listmodel.h"

#include "utils/cast.h"

#include "resources/skill/skillinfo.h"

#include "localconsts.h"

class SkillModel final : public ListModel
{
    public:
        SkillModel();

        A_DELETE_COPY(SkillModel)

        SkillInfo *getSkillAt(const int i) const;

        std::string getElementAt(int i) override final;

        int getNumberOfElements() override final
        { return CAST_S32(mVisibleSkills.size()); }

        void addSkill(SkillInfo *const info)
        { mSkills.push_back(info); }

        void updateVisibilities();

    private:
        SkillList mSkills;
        SkillList mVisibleSkills;
};

#endif  // GUI_MODELS_SKILLMODEL_H
