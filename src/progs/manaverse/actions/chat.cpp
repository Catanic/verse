/*
 *  The ManaVerse Client
 *  Copyright (C) 2012-2020  The ManaPlus Developers
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

#include "actions/chat.h"

#include "configuration.h"

#include "actions/actiondef.h"

#include "being/localplayer.h"

#include "gui/sdlinput.h"

#include "gui/windows/chatwindow.h"

#include "listeners/inputactionreplaylistener.h"

#include "net/charserverhandler.h"
#include "net/chathandler.h"
#include "net/clanhandler.h"
#include "net/guildhandler.h"
#include "net/net.h"
#include "net/partyhandler.h"

#ifdef TMWA_SUPPORT
#include "net/tmwa/guildmanager.h"
#endif  // TMWA_SUPPORT

#include "resources/iteminfo.h"

#include "resources/db/itemdb.h"

#include "utils/booleanoptions.h"
#include "utils/chatutils.h"
#include "utils/parameters.h"

#include "utils/translation/podict.h"

#include "debug.h"

const int DEFAULT_CHAT_WINDOW_SCROLL = 7;

namespace Actions
{

static void outString(ChatTab *const tab,
                      const std::string &str,
                      const std::string &def)
{
    if (tab == nullptr)
    {
        if (chatHandler != nullptr)
            chatHandler->talk(def);
        return;
    }

    switch (tab->getType())
    {
        case ChatTabType::CLAN:
        {
            if (clanHandler != nullptr)
                clanHandler->chat(str);
            break;
        }
        case ChatTabType::PARTY:
        {
            if (partyHandler != nullptr)
                partyHandler->chat(str);
            break;
        }
        case ChatTabType::GUILD:
        {
            if ((guildHandler == nullptr) || (localPlayer == nullptr))
                return;
            const Guild *const guild = localPlayer->getGuild();
            if (guild != nullptr)
            {
#ifdef TMWA_SUPPORT
                if (guild->getServerGuild())
                {
                    if (Net::getNetworkType() == ServerType::TMWATHENA)
                        return;
                    guildHandler->chat(str);
                }
                else if (guildManager != nullptr)
                {
                    guildManager->chat(str);
                }
#else  // TMWA_SUPPORT

                if (guild->getServerGuild())
                    guildHandler->chat(str);
#endif  // TMWA_SUPPORT
            }
            break;
        }
        case ChatTabType::CHANNEL:
        case ChatTabType::GM:
        case ChatTabType::TRADE:
            tab->chatInput(str);
            break;
        default:
        case ChatTabType::UNKNOWN:
        case ChatTabType::INPUT:
        case ChatTabType::WHISPER:
        case ChatTabType::DEBUG:
        case ChatTabType::BATTLE:
        case ChatTabType::LANG:
            if (chatHandler != nullptr)
                chatHandler->talk(str);
            break;
    }
}

impHandler0(toggleChat)
{
    return chatWindow != nullptr ? chatWindow->requestChatFocus() : false;
}

impHandler0(prevChatTab)
{
    if (chatWindow != nullptr)
    {
        chatWindow->prevTab();
        return true;
    }
    return false;
}

impHandler0(nextChatTab)
{
    if (chatWindow != nullptr)
    {
        chatWindow->nextTab();
        return true;
    }
    return false;
}

impHandler0(closeChatTab)
{
    if (chatWindow != nullptr)
    {
        chatWindow->closeTab();
        return true;
    }
    return false;
}

impHandler0(closeAllChatTabs)
{
    if (chatWindow != nullptr)
    {
        chatWindow->removeAllWhispers();
        chatWindow->saveState();
        return true;
    }
    return false;
}

impHandler0(ignoreAllWhispers)
{
    if (chatWindow != nullptr)
    {
        chatWindow->ignoreAllWhispers();
        chatWindow->saveState();
        return true;
    }
    return false;
}

impHandler0(scrollChatUp)
{
    if ((chatWindow != nullptr) && chatWindow->isWindowVisible())
    {
        chatWindow->scroll(-DEFAULT_CHAT_WINDOW_SCROLL);
        return true;
    }
    return false;
}

impHandler0(scrollChatDown)
{
    if ((chatWindow != nullptr) && chatWindow->isWindowVisible())
    {
        chatWindow->scroll(DEFAULT_CHAT_WINDOW_SCROLL);
        return true;
    }
    return false;
}

static bool splitWhisper(const std::string &args,
                         std::string &recvnick,
                         std::string &message)
{
    if (args.substr(0, 1) == "\"")
    {
        const size_t pos = args.find('"', 1);
        if (pos != std::string::npos)
        {
            recvnick = args.substr(1, pos - 1);
            if (pos + 2 < args.length())
                message = args.substr(pos + 2, args.length());
        }
    }
    else
    {
        const size_t pos = args.find(' ');
        if (pos != std::string::npos)
        {
            recvnick = args.substr(0, pos);
            if (pos + 1 < args.length())
                message = args.substr(pos + 1, args.length());
        }
        else
        {
            recvnick = std::string(args);
            message.clear();
        }
    }

    trim(message);

    if (message.length() > 0)
    {
        std::string playerName = localPlayer->getName();
        std::string tempNick = recvnick;

        toLower(playerName);
        toLower(tempNick);

        if (tempNick == playerName || args.empty())
            return false;

        return true;
    }
    return false;
}

impHandler(msg)
{
    std::string recvnick;
    std::string message;

    if (splitWhisper(event.args, recvnick, message))
    {
        if (chatWindow == nullptr)
            return false;
        ChatTab *const tab = chatWindow->addChatTab(recvnick, false, true);
        if (tab != nullptr)
        {
            chatWindow->saveState();
            tab->chatInput(message);
        }
    }
    else
    {
        if (event.tab != nullptr)
        {
            event.tab->chatLog(
                // TRANSLATORS: whisper send
                _("Cannot send empty whisper or channel message!"),
                ChatMsgType::BY_SERVER,
                IgnoreRecord_false,
                TryRemoveColors_true);
        }
    }
    return true;
}

impHandler(msgText)
{
    if (chatWindow == nullptr)
        return false;

    if (config.getBoolValue("whispertab"))
    {
        chatWindow->localChatInput("/q " + event.args);
    }
    else
    {
        chatWindow->addInputText(
            std::string("/w \"").append(event.args).append("\" "),
            true);
    }
    return true;
}

impHandler(msg2)
{
    std::string recvnick;
    std::string message;

    if (chatHandler != nullptr &&
        splitWhisper(event.args, recvnick, message))
    {
        chatHandler->privateMessage(recvnick, message);
    }
    return true;
}

impHandler(query)
{
    const std::string &args = event.args;
    if (chatWindow != nullptr)
    {
        if (chatWindow->addChatTab(args, true, true) != nullptr)
        {
            chatWindow->saveState();
            return true;
        }
    }

    if (event.tab != nullptr)
    {
        // TRANSLATORS: new whisper or channel query
        event.tab->chatLog(strprintf(_("Cannot create a whisper tab "
            "\"%s\"! It probably already exists."),
            args.c_str()),
            ChatMsgType::BY_SERVER,
            IgnoreRecord_false,
            TryRemoveColors_true);
    }
    return true;
}

impHandler0(clearChatTab)
{
    if (chatWindow != nullptr)
    {
        chatWindow->clearTab();
        return true;
    }
    return false;
}

impHandler(createParty)
{
    if (partyHandler == nullptr)
        return false;

    if (event.args.empty())
    {
        // TRANSLATORS: dialog header
        inputActionReplayListener.openDialog(_("Create party"),
            "",
            InputAction::CREATE_PARTY);
    }
    else
    {
        partyHandler->create(event.args);
    }
    return true;
}

impHandler(createGuild)
{
    if ((guildHandler == nullptr) ||
        Net::getNetworkType() == ServerType::TMWATHENA)
    {
        return false;
    }

    if (event.args.empty())
    {
        // TRANSLATORS: dialog header
        inputActionReplayListener.openDialog(_("Create guild"),
            "",
            InputAction::CREATE_GUILD);
    }
    else
    {
        guildHandler->create(event.args);
    }
    return true;
}

impHandler(party)
{
    if (!event.args.empty())
    {
        if (partyHandler != nullptr)
            partyHandler->invite(event.args);
    }
    else
    {
        if (event.tab != nullptr)
        {
            // TRANSLATORS: party invite message
            event.tab->chatLog(_("Please specify a name."),
                ChatMsgType::BY_SERVER,
                IgnoreRecord_false,
                TryRemoveColors_true);
        }
    }
    return true;
}

impHandler(guild)
{
    if ((guildHandler == nullptr) || (localPlayer == nullptr))
        return false;

    const std::string args = event.args;
    if (!args.empty())
    {
        const Guild *const guild = localPlayer->getGuild();
        if (guild != nullptr)
        {
#ifdef TMWA_SUPPORT
            if (guild->getServerGuild())
                guildHandler->invite(args);
            else if (guildManager != nullptr)
                GuildManager::invite(args);
#else  // TMWA_SUPPORT

            guildHandler->invite(args);
#endif  // TMWA_SUPPORT
        }
    }
    else
    {
        if (event.tab != nullptr)
        {
            // TRANSLATORS: guild invite message
            event.tab->chatLog(_("Please specify a name."),
                ChatMsgType::BY_SERVER,
                IgnoreRecord_false,
                TryRemoveColors_true);
        }
        else if (localChatTab != nullptr)
        {
            // TRANSLATORS: guild invite message
            localChatTab->chatLog(_("Please specify a name."),
                ChatMsgType::BY_SERVER,
                IgnoreRecord_false,
                TryRemoveColors_true);
        }
    }
    return true;
}

impHandler(me)
{
    outString(event.tab, textToMe(event.args), event.args);
    return true;
}

impHandler(toggle)
{
    if (event.args.empty())
    {
        if ((chatWindow != nullptr) && (event.tab != nullptr))
        {
            event.tab->chatLog(chatWindow->getReturnTogglesChat() ?
                // TRANSLATORS: message from toggle chat command
                _("Return toggles chat.") : _("Message closes chat."),
                ChatMsgType::BY_SERVER,
                IgnoreRecord_false,
                TryRemoveColors_true);
        }
        return true;
    }

    switch (parseBoolean(event.args))
    {
        case 1:
            if (event.tab != nullptr)
            {
                // TRANSLATORS: message from toggle chat command
                event.tab->chatLog(_("Return now toggles chat."),
                    ChatMsgType::BY_SERVER,
                    IgnoreRecord_false,
                    TryRemoveColors_true);
            }
            if (chatWindow != nullptr)
                chatWindow->setReturnTogglesChat(true);
            return true;
        case 0:
            if (event.tab != nullptr)
            {
                // TRANSLATORS: message from toggle chat command
                event.tab->chatLog(_("Message now closes chat."),
                    ChatMsgType::BY_SERVER,
                    IgnoreRecord_false,
                    TryRemoveColors_true);
            }
            if (chatWindow != nullptr)
                chatWindow->setReturnTogglesChat(false);
            return true;
        case -1:
            if (event.tab != nullptr)
            {
                event.tab->chatLog(strprintf(BOOLEAN_OPTIONS, "toggle"),
                    ChatMsgType::BY_SERVER,
                    IgnoreRecord_false,
                    TryRemoveColors_true);
            }
            return true;
        default:
            return true;
    }
}

impHandler(kickParty)
{
    if (!event.args.empty())
    {
        if (partyHandler != nullptr)
            partyHandler->kick(event.args);
    }
    else
    {
        if (event.tab != nullptr)
        {
            // TRANSLATORS: party kick message
            event.tab->chatLog(_("Please specify a name."),
                ChatMsgType::BY_SERVER,
                IgnoreRecord_false,
                TryRemoveColors_true);
        }
    }
    return true;
}

impHandler(kickGuild)
{
    if (!event.args.empty())
    {
        if (localPlayer != nullptr)
        {
            const Guild *const guild = localPlayer->getGuild();
            if (guild != nullptr)
            {
                if (guild->getServerGuild())
                {
                    if (guildHandler != nullptr)
                        guildHandler->kick(guild->getMember(event.args), "");
                }
#ifdef TMWA_SUPPORT
                else if (guildManager != nullptr)
                {
                    GuildManager::kick(event.args);
                }
#endif  // TMWA_SUPPORT
            }
        }
    }
    else
    {
        if (event.tab != nullptr)
        {
            // TRANSLATORS: party kick message
            event.tab->chatLog(_("Please specify a name."),
                ChatMsgType::BY_SERVER,
                IgnoreRecord_false,
                TryRemoveColors_true);
        }
    }
    return true;
}

impHandler(addText)
{
    if (chatWindow != nullptr)
        chatWindow->addInputText(event.args, true);
    return true;
}

impHandler0(clearChat)
{
    if (chatWindow != nullptr)
        chatWindow->clearTab();
    return true;
}

impHandler0(chatGeneralTab)
{
    if (chatWindow != nullptr)
        chatWindow->selectTabByType(ChatTabType::INPUT);
    return true;
}

impHandler0(chatDebugTab)
{
    if (chatWindow != nullptr)
        chatWindow->selectTabByType(ChatTabType::DEBUG);
    return true;
}

impHandler0(chatBattleTab)
{
    if (chatWindow != nullptr)
        chatWindow->selectTabByType(ChatTabType::BATTLE);
    return true;
}

impHandler0(chatTradeTab)
{
    if (chatWindow != nullptr)
        chatWindow->selectTabByType(ChatTabType::TRADE);
    return true;
}

impHandler0(chatLangTab)
{
    if (chatWindow != nullptr)
        chatWindow->selectTabByType(ChatTabType::LANG);
    return true;
}

impHandler0(chatGmTab)
{
    if (chatWindow != nullptr)
        chatWindow->selectTabByType(ChatTabType::GM);
    return true;
}

impHandler0(chatPartyTab)
{
    if (chatWindow != nullptr)
        chatWindow->selectTabByType(ChatTabType::PARTY);
    return true;
}

impHandler0(chatGuildTab)
{
    if (chatWindow != nullptr)
        chatWindow->selectTabByType(ChatTabType::GUILD);
    return true;
}

impHandler(hat)
{
    if ((localPlayer == nullptr) || (charServerHandler == nullptr))
        return false;

    const int sprite = localPlayer->getSpriteID(
        charServerHandler->hatSprite());
    std::string str;
    if (sprite == 0)
    {
        // TRANSLATORS: equipped hat chat message
        str = _("no hat equipped.");
    }
    else
    {
        const ItemInfo &info = ItemDB::get(sprite);
        // TRANSLATORS: equipped hat chat message
        str = strprintf(_("equipped hat %s."),
            info.getName().c_str());
    }
    outString(event.tab, str, str);
    return true;
}

impHandler(chatClipboard)
{
    int x = 0;
    int y = 0;

    if ((chatWindow != nullptr) && parse2Int(event.args, x, y))
    {
        chatWindow->copyToClipboard(x, y);
        return true;
    }
    return false;
}

impHandler(guildNotice)
{
    if (localPlayer == nullptr)
        return false;
    const std::string args = event.args;
    if (args.empty())
    {
        // TRANSLATORS: dialog header
        inputActionReplayListener.openDialog(_("Guild notice"),
            "",
            InputAction::GUILD_NOTICE);
        return true;
    }

    std::string str2;
    if (args.size() > 60)
        str2 = args.substr(60);
    const Guild *const guild = localPlayer->getGuild();
    if (guild != nullptr)
    {
        guildHandler->changeNotice(guild->getId(),
            args.substr(0, 60),
            str2);
    }
    return true;
}

impHandler(translate)
{
    if (reverseDictionary == nullptr ||
        localPlayer == nullptr ||
        event.args.empty())
    {
        return false;
    }

    ChatTab *const tab = event.tab;
    if (tab == nullptr)
        return false;

    std::string srcStr = event.args;
    std::string enStr;
    toLower(srcStr);
    if (localPlayer->getLanguageId() > 0)
    {
        if (reverseDictionary->haveStr(srcStr))
            enStr = reverseDictionary->getStr(srcStr);
        else if (dictionary->haveStr(srcStr))
            enStr = srcStr;
    }
    else
    {
        if (dictionary->haveStr(srcStr))
            enStr = srcStr;
    }

    if (enStr.empty())
    {
        tab->chatLog(
            // TRANSLATORS: translation error message
            strprintf(_("No translation found for string: %s"),
            srcStr.c_str()),
            ChatMsgType::BY_SERVER,
            IgnoreRecord_false,
            TryRemoveColors_true);
        return true;
    }

    tab->chatInput(enStr);
    return true;
}

impHandler(sendGuiKey)
{
    if (guiInput == nullptr)
        return false;

    const std::string args = event.args;
    if (args.empty())
        return false;
    StringVect pars;
    if (!splitParameters(pars, args, " ,", '\"'))
        return false;
    const int sz = CAST_S32(pars.size());
    if (sz < 1)
        return false;

    int keyValue = atoi(pars[0].c_str());
    if (keyValue == 0 &&
        pars[0].size() == 1)
    {
        keyValue = CAST_S32(pars[0][0]);
    }
    if (sz == 2)
    {
        const InputActionT actionId = InputManager::getActionByConfigField(
            pars[1]);
        guiInput->simulateKey(keyValue, actionId);
    }
    else
    {
        guiInput->simulateKey(keyValue, InputAction::NO_VALUE);
    }
    return true;
}

impHandler(sendMouseKey)
{
    if (guiInput == nullptr)
        return false;
    const std::string args = event.args;
    if (args.empty())
        return false;
    StringVect pars;
    if (!splitParameters(pars, args, " ,", '\"'))
        return false;
    const int sz = CAST_S32(pars.size());
    if (sz != 3)
        return false;

    const int x = atoi(pars[0].c_str());
    const int y = atoi(pars[1].c_str());
    const int key1 = CAST_S32(MouseButton::LEFT);
    const int key2 = CAST_S32(MouseButton::MIDDLE);
    const int key = atoi(pars[2].c_str());
    if (key < key1 || key > key2)
        return false;
    guiInput->simulateMouseClick(x,
        y,
        static_cast<MouseButtonT>(key));
    return true;
}

impHandler(sendChars)
{
    if (guiInput == nullptr)
        return false;

    const std::string args = event.args;
    if (args.empty())
        return false;

    const size_t sz = args.size();
    for (size_t f = 0; f < sz; f ++)
    {
        guiInput->simulateKey(CAST_S32(args[f]),
            InputAction::NO_VALUE);
    }

    return true;
}

}  // namespace Actions
