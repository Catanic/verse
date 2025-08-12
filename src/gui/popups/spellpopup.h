/*
 *  The ManaVerse Client
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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

#ifndef GUI_POPUPS_SPELLPOPUP_H
#define GUI_POPUPS_SPELLPOPUP_H

#include "gui/widgets/popup.h"

class Label;
class TextCommand;

/**
 * A popup that displays information about an item.
 */
class SpellPopup final : public Popup
{
    public:
        /**
         * Constructor. Initializes the item popup.
         */
        SpellPopup();

        A_DELETE_COPY(SpellPopup)

        /**
         * Destructor. Cleans up the item popup on deletion.
         */
        ~SpellPopup() override final;

        void postInit() override final;

        /**
         * Sets the info to be displayed given a particular item.
         */
        void setItem(const TextCommand *const spell);

        /**
         * Sets the location to display the item popup.
         */
        void view(const int x, const int y);

        void mouseMoved(MouseEvent &event) override final;

    private:
        Label *mItemName A_NONNULLPOINTER;
        Label *mItemComment A_NONNULLPOINTER;
};

extern SpellPopup *spellPopup;

#endif  // GUI_POPUPS_SPELLPOPUP_H
