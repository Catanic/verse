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

#ifndef GUI_MODELS_UPDATELISTMODEL_H
#define GUI_MODELS_UPDATELISTMODEL_H

#include "gui/models/listmodel.h"

#include "net/serverinfo.h"

#include "utils/cast.h"
#include "utils/foreach.h"

#include "localconsts.h"

class UpdateListModel final : public ListModel
{
    public:
        explicit UpdateListModel(ServerInfo *const server) :
            ListModel(),
            mNames(),
            mServer(server)
        {
            FOR_EACH(STD_VECTOR<HostsGroup>::const_iterator,
                     it,
                     server->updateHosts)
            {
                const HostsGroup &group = *it;
                mNames.push_back(group.name);
            }
        }

        A_DELETE_COPY(UpdateListModel)

        int getNumberOfElements() override final
        {
            return CAST_S32(mNames.size());
        }

        std::string getElementAt(int i) override final
        {
            if (i >= getNumberOfElements() || i < 0)
                return "???";
            return mNames[i];
        }

        bool empty() const
        {
            return mNames.empty();
        }

    protected:
        StringVect mNames;
        ServerInfo *mServer;
};

#endif  // GUI_MODELS_UPDATELISTMODEL_H
