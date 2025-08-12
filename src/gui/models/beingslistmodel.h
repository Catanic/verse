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

#ifndef GUI_MODELS_BEINGSLISTMODEL_H
#define GUI_MODELS_BEINGSLISTMODEL_H

#include "gui/models/avatarlistmodel.h"

#include "utils/cast.h"
#include "utils/dtor.h"
#include "utils/vector.h"

class BeingsListModel final : public AvatarListModel
{
    public:
        BeingsListModel() :
            AvatarListModel(),
            mMembers()
        {
        }

        A_DELETE_COPY(BeingsListModel)

        ~BeingsListModel() override final
        {
            delete_all(mMembers);
            mMembers.clear();
        }

        STD_VECTOR<Avatar*> *getMembers() RETURNS_NONNULL A_WARN_UNUSED
        {
            return &mMembers;
        }

        Avatar *getAvatarAt(int index) override final
        {
            return mMembers[index];
        }

        int getNumberOfElements() override final
        {
            return CAST_S32(mMembers.size());
        }

        STD_VECTOR<Avatar*> mMembers;
};

#endif  // GUI_MODELS_BEINGSLISTMODEL_H
