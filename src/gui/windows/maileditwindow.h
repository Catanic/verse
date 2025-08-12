/*
 *  The ManaVerse Client
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

#ifndef GUI_WINDOWS_MAILEDITWINDOW_H
#define GUI_WINDOWS_MAILEDITWINDOW_H

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"
#include "listeners/focuslistener.h"

class Button;
class IntTextField;
class Inventory;
class Item;
class ItemContainer;
class Label;
class ScrollArea;
class TextField;

class MailEditWindow final : public Window,
                             public ActionListener,
                             public FocusListener
{
    public:
        MailEditWindow();

        A_DELETE_COPY(MailEditWindow)

        ~MailEditWindow() override final;

        void action(const ActionEvent &event) override final;

        void addItem(const Item *const item, const int amount);

        void setSubject(const std::string &str);

        void setTo(const std::string &str);

        void setMessage(const std::string &str);

        void close() override final;

        Inventory *getInventory() const A_WARN_UNUSED;

        void updateItems();

        void focusLost(const Event &event) override final;

        void validatedTo();

    private:
        void sendMail();

        Button *mSendButton;
        Button *mCloseButton;
        Button *mAddButton;
        Label *mToLabel;
        Label *mSubjectLabel;
        Label *mMoneyLabel;
        Label *mItemLabel;
        Label *mMessageLabel;
        TextField *mToField;
        TextField *mSubjectField;
        IntTextField *mMoneyField;
        TextField *mMessageField;
        Inventory *mInventory;
        ItemContainer *mItemContainer;
        ScrollArea *mItemScrollArea;
        bool mUseMail2;
};

extern MailEditWindow *mailEditWindow;

#endif  // GUI_WINDOWS_MAILEDITWINDOW_H
