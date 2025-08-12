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

#ifndef GUI_WINDOWS_BUYSELLDIALOG_H
#define GUI_WINDOWS_BUYSELLDIALOG_H

#include "enums/simpletypes/beingid.h"

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"

class Button;

/**
 * A dialog to choose between buying or selling at a shop.
 *
 * \ingroup Interface
 */
class BuySellDialog final : public Window,
                            public ActionListener
{
    public:
        /**
         * Constructor. The action listener passed will receive "sell", "buy"
         * or "cancel" events when the respective buttons are pressed.
         *
         * @see Window::Window
         */
        explicit BuySellDialog(const BeingId npcId);

        explicit BuySellDialog(const std::string &nick);

        A_DELETE_COPY(BuySellDialog)

        ~BuySellDialog() override final;

        void init();

        void setVisible(Visible visible) override final;

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const ActionEvent &event) override final;

        /**
         * Returns true if any instances exist.
         */
        static bool isActive() A_WARN_UNUSED
        { return !dialogInstances.empty(); }

        /**
         * Closes all instances.
         */
        static void closeAll();

    private:
        typedef std::list<BuySellDialog*> DialogList;
        static DialogList dialogInstances;

        BeingId mNpcId;
        std::string mNick;
        Button *mBuyButton;
};

#endif  // GUI_WINDOWS_BUYSELLDIALOG_H
