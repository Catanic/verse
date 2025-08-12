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

#ifndef GUI_WIDGETS_TABS_SOCIALPARTYTAB_H
#define GUI_WIDGETS_TABS_SOCIALPARTYTAB_H

#include "gui/widgets/tabs/socialtab.h"

#include "const/sound.h"

#include "party.h"

#include "being/localplayer.h"

#include "net/partyhandler.h"

#include "utils/delete2.h"
#include "utils/foreach.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "localconsts.h"

class SocialPartyTab final : public SocialTab,
                             public ActionListener
{
    public:
        SocialPartyTab(const Widget2 *const widget,
                       Party *const party,
                       const Opaque showBackground) :
            SocialTab(widget),
            ActionListener(),
            mParty(party)
        {
            // TRANSLATORS: tab in social window
            setCaption(_("Party"));

            setTabColor(&getThemeColor(ThemeColorId::PARTY_SOCIAL_TAB, 255U),
                &getThemeColor(ThemeColorId::PARTY_SOCIAL_TAB_OUTLINE, 255U));
            setHighlightedTabColor(&getThemeColor(
                ThemeColorId::PARTY_SOCIAL_TAB_HIGHLIGHTED, 255U),
                &getThemeColor(
                ThemeColorId::PARTY_SOCIAL_TAB_HIGHLIGHTED_OUTLINE, 255U));
            setSelectedTabColor(&getThemeColor(
                ThemeColorId::PARTY_SOCIAL_TAB_SELECTED, 255U),
                &getThemeColor(
                ThemeColorId::PARTY_SOCIAL_TAB_SELECTED_OUTLINE, 255U));

            createControls(party, showBackground);
            mMenuAction = "party";
        }

        A_DELETE_COPY(SocialPartyTab)

        ~SocialPartyTab() override final
        {
            delete2(mList)
            delete2(mScroll)
        }

        void action(const ActionEvent &event) override final
        {
            const std::string &eventId = event.getId();
            if (eventId == "do invite")
            {
                const std::string name = mInviteDialog->getText();
                partyHandler->invite(name);

                if (localChatTab != nullptr)
                {
                    localChatTab->chatLog(strprintf(
                        // TRANSLATORS: chat message
                        _("Invited user %s to party."),
                        name.c_str()),
                        ChatMsgType::BY_SERVER,
                        IgnoreRecord_false,
                        TryRemoveColors_true);
                }
                mInviteDialog = nullptr;
            }
            else if (eventId == "~do invite")
            {
                mInviteDialog = nullptr;
            }
            else if (eventId == "yes")
            {
                partyHandler->leave();
                if (localChatTab != nullptr)
                {
                    localChatTab->chatLog(strprintf(
                        // TRANSLATORS: tab in social window
                        _("Party %s quit requested."),
                        mParty->getName().c_str()),
                        ChatMsgType::BY_SERVER,
                        IgnoreRecord_false,
                        TryRemoveColors_true);
                }
                mConfirmDialog = nullptr;
            }
            else if (eventId == "~yes")
            {
                mConfirmDialog = nullptr;
            }
        }

        void invite() override final
        {
            CREATEWIDGETV(mInviteDialog, TextDialog,
                // TRANSLATORS: party invite message
                _("Member Invite to Party"),
                // TRANSLATORS: party invite message
                strprintf(_("Who would you like to invite to party %s?"),
                mParty->getName().c_str()),
                socialWindow,
                false);
            mInviteDialog->setActionEventId("do invite");
            mInviteDialog->addActionListener(this);
        }

        void leave() override final
        {
            CREATEWIDGETV(mConfirmDialog, ConfirmDialog,
                // TRANSLATORS: party leave message
                _("Leave Party?"),
                // TRANSLATORS: party leave message
                strprintf(_("Are you sure you want to leave party %s?"),
                mParty->getName().c_str()),
                SOUND_REQUEST,
                false,
                Modal_false,
                socialWindow);
            mConfirmDialog->addActionListener(this);
        }

        void buildCounter(const int online0 A_UNUSED,
                          const int total0 A_UNUSED) override final
        {
            if (localPlayer == nullptr)
                return;

            const Party *const party = localPlayer->getParty();
            if (party == nullptr)
                return;

            const Party::MemberList *const members = party->getMembers();
            int online = 0;
            int total = 0;
            FOR_EACHP (Party::MemberList::const_iterator, it, members)
            {
                if ((*it)->getOnline())
                    online ++;
                total ++;
            }

            // TRANSLATORS: social window label
            mCounterString = strprintf(_("Players: %u/%u"),
                CAST_U32(online),
                CAST_U32(total));
            updateCounter();
        }

    private:
        Party *mParty;
};

#endif  // GUI_WIDGETS_TABS_SOCIALPARTYTAB_H
