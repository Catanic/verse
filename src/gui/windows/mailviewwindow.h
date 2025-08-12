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

#ifndef GUI_WINDOWS_MAILVIEWWINDOW_H
#define GUI_WINDOWS_MAILVIEWWINDOW_H

#include "gui/widgets/window.h"

#include "listeners/actionlistener.h"

class Button;
class Inventory;
class ItemContainer;
class Label;
class ScrollArea;

struct MailMessage;

class MailViewWindow final : public Window,
                             public ActionListener
{
    public:
        MailViewWindow(MailMessage *const message,
                       const int itemsCount) A_NONNULL(2);

        A_DELETE_COPY(MailViewWindow)

        ~MailViewWindow() override final;

        void action(const ActionEvent &event) override final;

        Inventory *getInventory() const A_WARN_UNUSED;

        void updateItems();

        void removeItems(const int64_t mailId);

        void removeMoney(const int64_t mailId);

        /**
        * notify view window that mail has been deleted.
        * MailViewWindow will delete the message when it is destroyed.
        * returns true if mailId is the one being viewed (and thus ownership
        * was transfered).
        */
        bool notifyDeleteAndTransferOwnership(const int64_t mailId);
    private:
        void updateAttachButton();

        MailMessage *mMessage;
        Button *mGetAttachButton;
        Button *mGetMoneyButton;
        Button *mCloseButton;
        Button *mPrevButton;
        Button *mNextButton;
        Button *mReplyButton;
        Label *mTimeLabel;
        Label *mExpireTimeLabel;
        Label *mMoneyLabel;
        Label *mFromLabel;
        Label *mSubjectLabel;
        Label *mMessageLabel;
        Inventory *mInventory;
        ItemContainer *mItemContainer;
        ScrollArea *mItemScrollArea;
        bool mUseMail2;
        bool mDeleted = false;
};

extern MailViewWindow *mailViewWindow;

#endif  // GUI_WINDOWS_MAILVIEWWINDOW_H
