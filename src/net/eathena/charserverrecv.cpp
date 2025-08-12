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

#include "net/eathena/charserverrecv.h"

#include "client.h"
#include "configuration.h"
#include "pincodemanager.h"
#include "settings.h"

#include "gui/windows/charcreatedialog.h"
#include "gui/windows/charselectdialog.h"
#include "gui/windows/okdialog.h"

#include "gui/widgets/createwidget.h"

#include "net/character.h"
#include "net/charserverhandler.h"
#include "net/playerhandler.h"

#include "net/ea/token.h"

#include "net/eathena/gamehandler.h"
#include "net/eathena/loginhandler.h"
#include "net/eathena/messageout.h"
#include "net/eathena/network.h"
#include "net/eathena/protocolout.h"
#include "net/eathena/sprite.h"

#include "resources/iteminfo.h"

#include "resources/db/itemdb.h"

#include "utils/dtor.h"
#include "utils/gettext.h"
#include "utils/performance.h"

#include "debug.h"

extern int packetVersion;

namespace EAthena
{

extern ServerInfo charServer;
extern ServerInfo mapServer;

namespace CharServerRecv
{
    std::string mNewName;
    BeingId mRenameId = BeingId_zero;
}  // namespace CharServerRecv

// callers must count each packet size by self
void CharServerRecv::readPlayerData(Net::MessageIn &msg,
                                    Net::Character *const character)
{
    if (character == nullptr)
        return;

    const Token &token =
        static_cast<LoginHandler*>(loginHandler)->getToken();

    LocalPlayer *const tempPlayer = new LocalPlayer(
        msg.readBeingId("player id"), BeingTypeId_zero);
    tempPlayer->setGender(token.sex);

    PlayerInfoBackend &data = character->data;
    if (packetVersion >= 20170830)
        data.mAttributes[Attributes::PLAYER_EXP] = msg.readInt64("exp");
    else
        data.mAttributes[Attributes::PLAYER_EXP] = msg.readInt32("exp");
    data.mAttributes[Attributes::MONEY] = msg.readInt32("money");
    if (packetVersion >= 20170830)
    {
        data.mAttributes[Attributes::PLAYER_JOB_EXP] =
            msg.readInt64("job exp");
    }
    else
    {
        data.mAttributes[Attributes::PLAYER_JOB_EXP] =
            msg.readInt32("job exp");
    }
    data.mAttributes[Attributes::PLAYER_JOB_LEVEL] =
        msg.readInt32("job level");

    msg.readInt16("shoes?");
    const int gloves = msg.readInt16("gloves");
    const int cape = msg.readInt16("cape");
    const int misc1 = msg.readInt16("misc1");

    msg.readInt32("option");
    tempPlayer->setKarma(msg.readInt32("karma"));
    tempPlayer->setManner(msg.readInt32("manner"));
    msg.readInt16("left points");

    if (packetVersion >= 20081217)
    {
        data.mAttributes[Attributes::PLAYER_HP] = msg.readInt32("hp");
        data.mAttributes[Attributes::PLAYER_MAX_HP] = msg.readInt32("max hp");
    }
    else
    {
        data.mAttributes[Attributes::PLAYER_HP] = msg.readInt16("hp");
        data.mAttributes[Attributes::PLAYER_MAX_HP] = msg.readInt16("max hp");
    }
    data.mAttributes[Attributes::PLAYER_MP] = msg.readInt16("mp/sp");
    data.mAttributes[Attributes::PLAYER_MAX_MP] = msg.readInt16("max mp/sp");

    msg.readInt16("speed");
    const uint16_t race = msg.readInt16("class");
//    tempPlayer->setSubtype(race, 0);
    const int hairStyle = msg.readInt16("hair style");
    if (packetVersion >= 20141022)
        msg.readInt16("body");
    const int option A_UNUSED = (msg.readInt16("weapon") | 1) ^ 1;
    const int weapon = 0;

    tempPlayer->setSpriteId(SPRITE_BODY,
        weapon);
    tempPlayer->setWeaponId(weapon);

    data.mAttributes[Attributes::PLAYER_BASE_LEVEL] = msg.readInt16("level");

    msg.readInt16("skill points");
    const int bottomClothes = msg.readInt16("head bottom");
    const int shield = msg.readInt16("shild");
    const int hat = msg.readInt16("head top");
    const int topClothes = msg.readInt16("head mid");

    const ItemColor color = fromInt(msg.readInt16("hair color"), ItemColor);
    tempPlayer->setHairColor(color);
    if (hairStyle == 0)
    {
        tempPlayer->unSetSprite(SPRITE_HAIR_COLOR);
    }
    else
    {
        tempPlayer->setSpriteColor(SPRITE_HAIR_COLOR,
            hairStyle * -1,
            ItemDB::get(-hairStyle).getDyeColorsString(
            color));
    }

    const uint16_t look = msg.readInt16("clothes color");
    tempPlayer->setSubtype(fromInt(race, BeingTypeId), look);
    tempPlayer->setName(msg.readString(24, "name"));

    character->dummy = tempPlayer;

    character->data.mStats[Attributes::PLAYER_STR].base = msg.readUInt8("str");
    character->data.mStats[Attributes::PLAYER_AGI].base = msg.readUInt8("agi");
    character->data.mStats[Attributes::PLAYER_VIT].base = msg.readUInt8("vit");
    character->data.mStats[Attributes::PLAYER_INT].base = msg.readUInt8("int");
    character->data.mStats[Attributes::PLAYER_DEX].base = msg.readUInt8("dex");
    character->data.mStats[Attributes::PLAYER_LUK].base = msg.readUInt8("luk");

    character->slot = msg.readInt16("character slot id");
    if (packetVersion >= 20061023)
        msg.readInt16("rename");
    if (packetVersion >= 20100803)
    {
        msg.readString(16, "map name");
        msg.readInt32("delete date");
    }
    int shoes = 0;
    if (packetVersion >= 20110111)
        shoes = msg.readInt32("robe");
    if (serverVersion == 0)
    {
        tempPlayer->setSpriteId(SPRITE_HAIR,
            shoes);
        tempPlayer->setSpriteId(SPRITE_SHOES,
            gloves);
        tempPlayer->setSpriteId(SPRITE_SHIELD,
            cape);
        tempPlayer->setSpriteId(SPRITE_HEAD_TOP,
            misc1);
        tempPlayer->setSpriteId(SPRITE_WEAPON,
            bottomClothes);
        tempPlayer->setSpriteId(SPRITE_FLOOR,
            shield);
        tempPlayer->setSpriteId(SPRITE_CLOTHES_COLOR,
            hat);
        tempPlayer->setSpriteId(SPRITE_HEAD_BOTTOM,
            topClothes);
//        tempPlayer->setSprite(SPRITE_HEAD_MID, misc2);
    }
    if (packetVersion >= 20110928)
        msg.readInt32("slot change");
    if (packetVersion >= 20111025)
        tempPlayer->setRename(msg.readInt32("rename (inverse)") != 0);
    uint8_t gender = 99U;
    if (packetVersion >= 20141016)
        gender = CAST_U8(msg.readUInt8("gender"));
    if (gender != 99)
        tempPlayer->setGender(Being::intToGender(gender));
}

void CharServerRecv::processCharLogin(Net::MessageIn &msg)
{
    msg.readInt16("packet len");
    int slots = 9;
    int offset = 0;
    if (packetVersion >= 20100413)
    {
        slots = msg.readInt8("MAX_CHARS");
        msg.readInt8("sd->char_slots");
        msg.readInt8("MAX_CHARS");
        offset = 3;
    }
    loginData.characterSlots = CAST_U16(slots);

    msg.skip(20, "unused 0");

    delete_all(Net::CharServerHandler::mCharacters);
    Net::CharServerHandler::mCharacters.clear();

    // Derive number of characters from message length
    const int count = (msg.getLength() - 24 - offset)
        / (106 + 4 + 2 + 16 + 4 + 4 + 4 + 4);

    for (int i = 0; i < count; ++i)
    {
        Net::Character *const character = new Net::Character;
        readPlayerData(msg, character);
        Net::CharServerHandler::mCharacters.push_back(character);
        if (character->dummy != nullptr)
        {
            logger->log("CharServer: Player: %s (%d)",
                character->dummy->getName().c_str(), character->slot);
        }
    }

    client->setState(State::CHAR_SELECT);
}

void CharServerRecv::processCharLogin2(Net::MessageIn &msg)
{
    // ignored
    msg.readInt16("len");
    msg.readUInt8("char slots");
    msg.readUInt8("left slots");
    msg.readUInt8("left slots");
    msg.readUInt8("char slots");
    msg.readUInt8("char slots");
    msg.skip(20, "unused");
}

void CharServerRecv::processCharMapInfo(Net::MessageIn &restrict msg)
{
    Network *const network = Network::mInstance;
    ServerInfo &server = mapServer;
    BLOCK_START("CharServerRecv::processCharMapInfo")
    PlayerInfo::setCharId(msg.readInt32("char id"));
    GameHandler::setMap(msg.readString(16, "map name"));

    const int mapIpAddress = msg.readInt32("map ip address");
    if (config.getBoolValue("usePersistentIP") || settings.persistentIp)
        server.hostname = settings.serverName;
    else
        server.hostname = ipToString(mapIpAddress);

    server.port = msg.readInt16("map ip port");
    if (msg.getVersion() >= 20170329)
    {
        for (int f = 0; f < 32; f ++)
            msg.readInt32("unused");
    }

    // Prevent the selected local player from being deleted
    localPlayer = Net::CharServerHandler::mSelectedCharacter->dummy;
    PlayerInfo::setBackend(Net::CharServerHandler::mSelectedCharacter->data);
    PlayerInfo::setStatBase(Attributes::PLAYER_WALK_SPEED,
        playerHandler->getDefaultWalkSpeed(),
        Notify_true);

    Net::CharServerHandler::mSelectedCharacter->dummy = nullptr;

    charServerHandler->clear();
    Net::CharServerHandler::updateCharSelectDialog();

    if (network != nullptr)
        network->disconnect();
    client->setState(State::CONNECT_GAME);
    BLOCK_END("CharServerRecv::processCharMapInfo")
}

void CharServerRecv::processChangeMapServer(Net::MessageIn &msg)
{
    Network *const network = Network::mInstance;
    ServerInfo &server = mapServer;
    BLOCK_START("CharServerRecv::processChangeMapServer")
    if (network == nullptr)
    {
        BLOCK_END("CharServerRecv::processChangeMapServer")
        return;
    }
    GameHandler::setMap(msg.readString(16, "map name"));
    const int x = msg.readInt16("x");
    const int y = msg.readInt16("y");
    const int mapIpAddress = msg.readInt32("map ip address");
    if (config.getBoolValue("usePersistentIP") || settings.persistentIp)
        server.hostname = settings.serverName;
    else
        server.hostname = ipToString(mapIpAddress);

    server.port = msg.readInt16("port");
    if (msg.getVersion() >= 20170315)
    {
        for (int f = 0; f < 32; f ++)
            msg.readInt32("unknown");
    }

    network->disconnect();
    client->setState(State::CHANGE_MAP);
    if (localPlayer != nullptr)
    {
        localPlayer->setTileCoords(x, y);
        localPlayer->setMap(nullptr);
    }
    BLOCK_END("CharServerRecv::processChangeMapServer")
}

void CharServerRecv::processPincodeStatus(Net::MessageIn &msg)
{
    pincodeManager.setSeed(msg.readUInt32("pincode seed"));
    pincodeManager.setAccountId(msg.readBeingId("account id"));
    pincodeManager.setPincodeLockFlag(false);
    if (pincodeManager.processPincodeStatus(CAST_U16(
        msg.readInt16("state"))) == false)
    {
        UNIMPLEMENTEDPACKET;
    }
}

void CharServerRecv::processPincodeStatus2(Net::MessageIn &msg)
{
    pincodeManager.setSeed(msg.readUInt32("pincode seed"));
    pincodeManager.setAccountId(msg.readBeingId("account id"));
    const uint16_t state = CAST_U16(msg.readInt16("state"));
    pincodeManager.setPincodeLockFlag(msg.readInt16("flag") == 0);
    if (pincodeManager.processPincodeStatus(state) == false)
    {
        UNIMPLEMENTEDPACKET;
    }
}

void CharServerRecv::processPincodeMakeStatus(Net::MessageIn &msg)
{
    // UNIMPLEMENTEDPACKET
    msg.readInt16("state");
    msg.readInt32("seed");
}

void CharServerRecv::processPincodeEditStatus(Net::MessageIn &msg)
{
    // UNIMPLEMENTEDPACKET
    msg.readInt16("state");
    msg.readInt32("seed");
}

void CharServerRecv::processCharCreate(Net::MessageIn &msg)
{
    BLOCK_START("CharServerRecv::processCharCreate")
    Net::Character *const character = new Net::Character;
    readPlayerData(msg, character);
    Net::CharServerHandler::mCharacters.push_back(character);

    Net::CharServerHandler::updateCharSelectDialog();

    // Close the character create dialog
    Net::CharServerHandler::mCharCreateDialog->scheduleDelete();
    Net::CharServerHandler::mCharCreateDialog = nullptr;
    BLOCK_END("CharServerRecv::processCharCreate")
}

void CharServerRecv::processCharCheckRename(Net::MessageIn &msg)
{
    if (msg.readInt16("flag") != 0)
    {
        createOutPacket(CMSG_CHAR_RENAME);
        outMsg.writeBeingId(mRenameId, "char id");
    }
    else
    {
        CREATEWIDGET(OkDialog,
            // TRANSLATORS: error header
            _("Error"),
            // TRANSLATORS: error message
            _("Character rename error."),
            // TRANSLATORS: ok dialog button
            _("Error"),
            DialogType::ERROR,
            Modal_true,
            ShowCenter_true,
            nullptr,
            260);
    }
}

void CharServerRecv::processCharRename(Net::MessageIn &msg)
{
    const int flag = msg.readInt16("flag");
    if (flag == 0)
    {
        Net::CharServerHandler::mCharSelectDialog->setName(
            mRenameId,
            mNewName);
        CREATEWIDGET(OkDialog,
            // TRANSLATORS: info header
            _("Info"),
            // TRANSLATORS: info message
            _("Character renamed."),
            // TRANSLATORS: ok dialog button
            _("OK"),
            DialogType::OK,
            Modal_true,
            ShowCenter_true,
            nullptr,
            260);
    }
    else
    {
        std::string message;
        switch (flag)
        {
            case 1:
                // TRANSLATORS: char rename error
                message = _("Rename not allowed.");
                break;
            case 2:
                // TRANSLATORS: char rename error
                message = _("New name is not set.");
                break;
            case 3:
            default:
                // TRANSLATORS: char rename error
                message = _("Character rename error.");
                break;
            case 4:
                // TRANSLATORS: char rename error
                message = _("Character not found.");
                break;
        }
        CREATEWIDGET(OkDialog,
            // TRANSLATORS: info message
            _("Info"),
            message,
            // TRANSLATORS: ok dialog button
            _("OK"),
            DialogType::OK,
            Modal_true,
            ShowCenter_true,
            nullptr,
            260);
    }
}

void CharServerRecv::processCharChangeSlot(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt16("len");
    msg.readInt16("flag");  // 0 - ok, 1 - error
    msg.readInt16("unused");
}

void CharServerRecv::processCharDeleteFailed(Net::MessageIn &msg)
{
    BLOCK_START("CharServerRecv::processCharDeleteFailed")
    Net::CharServerHandler::unlockCharSelectDialog();
    msg.readUInt8("error");
    CREATEWIDGET(OkDialog,
        // TRANSLATORS: error header
        _("Error"),
        // TRANSLATORS: error message
        _("Failed to delete character."),
        // TRANSLATORS: ok dialog button
        _("OK"),
        DialogType::ERROR,
        Modal_true,
        ShowCenter_true,
        nullptr,
        260);
    BLOCK_END("CharServerRecv::processCharDeleteFailed")
}

void CharServerRecv::processCharCaptchaNotSupported(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt16("5");
    msg.readUInt8("1");
}

void CharServerRecv::processCharDelete2Ack(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt32("char id");
    msg.readInt32("result");
    // for packets before 20130000, this is raw time
    // in other case raw time - time(NULL)
    msg.readInt32("time");
}

void CharServerRecv::processCharDelete2AcceptActual(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt32("char id");
    msg.readInt32("result");
}

void CharServerRecv::processCharDelete2CancelAck(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    msg.readInt32("char id");
    msg.readInt32("result");
}

void CharServerRecv::processCharCharacters(Net::MessageIn &msg)
{
    msg.readInt16("packet len");

    delete_all(Net::CharServerHandler::mCharacters);
    Net::CharServerHandler::mCharacters.clear();

    // Derive number of characters from message length
    const int count = (msg.getLength() - 4)
        / (106 + 4 + 2 + 16 + 4 + 4 + 4 + 4);

    for (int i = 0; i < count; ++i)
    {
        Net::Character *const character = new Net::Character;
        readPlayerData(msg, character);
        Net::CharServerHandler::mCharacters.push_back(character);
        if (character->dummy != nullptr)
        {
            logger->log("CharServer: Player: %s (%d)",
                character->dummy->getName().c_str(), character->slot);
        }
    }

    client->setState(State::CHAR_SELECT);
}

void CharServerRecv::processCharBanCharList(Net::MessageIn &msg)
{
    UNIMPLEMENTEDPACKET;
    const int count = (msg.readInt16("len") - 4) / 24;
    for (int f = 0; f < count; f ++)
    {
        msg.readInt32("char id");
        msg.readString(20, "unbun time");
    }
}

}  // namespace EAthena
