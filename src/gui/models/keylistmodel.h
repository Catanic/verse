/*
 *  The ManaVerse Client
 *  Copyright (C) 2007  Joshua Langley <joshlangley@optusnet.com.au>
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef GUI_MODELS_KEYLISTMODEL_H
#define GUI_MODELS_KEYLISTMODEL_H

#include "gui/setupactiondata.h"

#include "gui/models/listmodel.h"

#include "localconsts.h"

extern SetupActionData *const setupActionData[];

/**
 * The list model for key function list.
 *
 * \ingroup Interface
 */
class KeyListModel final : public ListModel
{
    public:
        KeyListModel() :
            mDataNum(0),
            mSelectedData(0),
            mSize(0)
        {
        }

        A_DELETE_COPY(KeyListModel)

        /**
         * Returns the number of elements in container.
         */
        int getNumberOfElements() override final
        { return mSize; }

        /**
         * Returns element from container.
         */
        std::string getElementAt(int i) override final
        { return setupActionData[mSelectedData][i].text; }

        /**
         * Sets element from container.
         */
        void setElementAt(const int i, const std::string &caption)
        { setupActionData[mSelectedData][i].text = caption; }

        void setSize(const int size) noexcept2
        { mSize = size; }

        void setDataNum(const int num) noexcept2
        { mDataNum = num; }

        void setSelectedData(const int i) noexcept2
        { mSelectedData = i; }

        int getSelectedData() const noexcept2 A_WARN_UNUSED
        { return mSelectedData; }

    private:
        int mDataNum;
        int mSelectedData;
        int mSize;
};

#endif  // GUI_MODELS_KEYLISTMODEL_H
