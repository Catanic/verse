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

#ifndef GUI_POPUPS_BEINGPOPUP_H
#define GUI_POPUPS_BEINGPOPUP_H

#include "gui/widgets/popup.h"

class Being;
class Label;

/**
 * A popup that displays information about a being.
 */
class BeingPopup final : public Popup
{
    public:
        /**
         * Constructor. Initializes the being popup.
         */
        BeingPopup();

        A_DELETE_COPY(BeingPopup)

        /**
         * Destructor. Cleans up the being popup on deletion.
         */
        ~BeingPopup() override final;

        /**
         * Sets the info to be displayed given a particular player.
         */
        void show(const int x, const int y, Being *const b);

        void postInit() override final;

#ifdef USE_PROFILER
        void logic();
#endif  // USE_PROFILER

    private:
        void addLabels(const int fontHeight);

        Label *mBeingName A_NONNULLPOINTER;
        STD_VECTOR<Label*> mLabels;
};

extern BeingPopup *beingPopup;

#endif  // GUI_POPUPS_BEINGPOPUP_H
