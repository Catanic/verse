/*
 *  The ManaVerse Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#ifndef GUI_MODELS_IGNORECHOICESLISTMODEL_H
#define GUI_MODELS_IGNORECHOICESLISTMODEL_H

#include "gui/models/playerrelationlistmodel.h"

#include "being/playerignorestrategy.h"
#include "being/playerrelations.h"

#include "utils/cast.h"

/**
 * Class for choosing one of the various `what to do when ignoring a player' options
 */
class IgnoreChoicesListModel final : public ListModel
{
    public:
        IgnoreChoicesListModel() :
            ListModel()
        { }

        A_DELETE_COPY(IgnoreChoicesListModel)

        int getNumberOfElements() override final
        {
            return CAST_S32(playerRelations.
                getPlayerIgnoreStrategies()->size());
        }

        std::string getElementAt(int i) override final
        {
            if (i >= getNumberOfElements() || i < 0)
                return "???";

            return (*playerRelations.getPlayerIgnoreStrategies())
                [i]->mDescription;
        }
};

#endif  // GUI_MODELS_IGNORECHOICESLISTMODEL_H
