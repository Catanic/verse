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

#ifndef GUI_WINDOWS_CHANGEPASSWORDDIALOG_H
#define GUI_WINDOWS_CHANGEPASSWORDDIALOG_H

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"

class Button;
class LoginData;
class TextField;
class WrongDataNoticeListener;

/**
 * The Change password dialog.
 *
 * \ingroup Interface
 */
class ChangePasswordDialog final : public Window,
                                   public ActionListener
{
    public:
        /**
         * Constructor
         *
         * @see Window::Window
         */
        explicit ChangePasswordDialog(LoginData &data);

        A_DELETE_COPY(ChangePasswordDialog)

        /**
         * Destructor
         */
        ~ChangePasswordDialog() override final;

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const ActionEvent &event) override final;

    private:
        TextField *mOldPassField A_NONNULLPOINTER;
        TextField *mFirstPassField A_NONNULLPOINTER;
        TextField *mSecondPassField A_NONNULLPOINTER;

        Button *mChangePassButton A_NONNULLPOINTER;
        Button *mCancelButton A_NONNULLPOINTER;

        WrongDataNoticeListener *mWrongDataNoticeListener A_NONNULLPOINTER;

        LoginData *mLoginData A_NONNULLPOINTER;
};

#endif  // GUI_WINDOWS_CHANGEPASSWORDDIALOG_H
