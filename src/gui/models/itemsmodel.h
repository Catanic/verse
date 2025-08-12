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

#ifndef GUI_MODELS_ITEMSMODEL_H
#define GUI_MODELS_ITEMSMODEL_H

#include "gui/models/listmodel.h"

#include "resources/iteminfo.h"

#include "resources/db/itemdb.h"

#include "utils/foreach.h"

#include <list>

#include "localconsts.h"

class ItemsModal final : public ListModel
{
    public:
        ItemsModal() :
            mStrings()
        {
            const std::map<int, ItemInfo*> &items = ItemDB::getItemInfos();
            std::list<std::string> tempStrings;

            for (std::map<int, ItemInfo*>::const_iterator
                 i = items.begin(), i_end = items.end();
                 i != i_end; ++i)
            {
                if (i->first < 0)
                    continue;

                const ItemInfo &info = *i->second;
                const std::string &name = info.getName();
                if (name != "unnamed" && !info.getName().empty()
                    && info.getName() != "unnamed")
                {
                    tempStrings.push_back(name);
                }
            }
            tempStrings.sort();
            FOR_EACH (std::list<std::string>::const_iterator, i, tempStrings)
                mStrings.push_back(*i);
        }

        A_DELETE_COPY(ItemsModal)

        int getNumberOfElements() override final
        {
            return CAST_S32(mStrings.size());
        }

        std::string getElementAt(int i) override final
        {
            if (i < 0 || i >= getNumberOfElements())
                return "???";
            return mStrings.at(i);
        }

    private:
        StringVect mStrings;
};

#endif  // GUI_MODELS_ITEMSMODEL_H
