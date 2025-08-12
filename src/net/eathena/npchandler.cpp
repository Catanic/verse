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

#include "net/eathena/npchandler.h"

#include "being/localplayer.h"

#include "const/net/inventory.h"

#include "gui/windows/npcdialog.h"

#include "gui/widgets/createwidget.h"

#include "net/messagein.h"

#include "net/ea/npcrecv.h"

#include "net/eathena/messageout.h"
#include "net/eathena/npcrecv.h"

#include "net/eathena/protocolout.h"

#include "utils/foreach.h"

#include "resources/item/shopitem.h"

#include "debug.h"

extern int packetVersion;
extern int packetVersionMain;
extern int packetVersionRe;
extern bool packets_zero;
extern int itemIdLen;

namespace EAthena
{

NpcHandler::NpcHandler() :
    Ea::NpcHandler()
{
    npcHandler = this;
}

NpcHandler::~NpcHandler()
{
    npcHandler = nullptr;
}

void NpcHandler::talk(const Being *const being) const
{
    if (being == nullptr)
        return;
    createOutPacket(CMSG_NPC_TALK);
    outMsg.writeBeingId(being->getId(), "npc id");
    outMsg.writeInt8(0, "unused");
    EAthena::NpcRecv::mNpcTypeId = being->getSubType();
}

void NpcHandler::nextDialog(const BeingId npcId) const
{
    createOutPacket(CMSG_NPC_NEXT_REQUEST);
    outMsg.writeBeingId(npcId, "npc id");
}

void NpcHandler::closeDialog(const BeingId npcId)
{
    createOutPacket(CMSG_NPC_CLOSE);
    outMsg.writeBeingId(npcId, "npc id");

    const NpcDialogs::iterator it = NpcDialog::mNpcDialogs.find(npcId);
    if (it != NpcDialog::mNpcDialogs.end())
    {
        NpcDialog *const dialog = (*it).second;
        if (dialog != nullptr)
            dialog->close();
        if (dialog == Ea::NpcRecv::mDialog)
            Ea::NpcRecv::mDialog = nullptr;
        NpcDialog::mNpcDialogs.erase(it);
    }
}

void NpcHandler::listInput(const BeingId npcId,
                           const unsigned char value) const
{
    createOutPacket(CMSG_NPC_LIST_CHOICE);
    outMsg.writeBeingId(npcId, "npc id");
    outMsg.writeInt8(value, "value");
}

void NpcHandler::integerInput(const BeingId npcId,
                              const int value) const
{
    createOutPacket(CMSG_NPC_INT_RESPONSE);
    outMsg.writeBeingId(npcId, "npc id");
    outMsg.writeInt32(value, "value");
}

void NpcHandler::stringInput(const BeingId npcId,
                             const std::string &value) const
{
    createOutPacket(CMSG_NPC_STR_RESPONSE);
    if (packetVersion >= 20151029)
    {
        outMsg.writeInt16(CAST_S16(value.length() + 8), "len");
        outMsg.writeBeingId(npcId, "npc id");
        outMsg.writeString(value, CAST_S32(value.length()), "value");
    }
    else
    {
        outMsg.writeInt16(CAST_S16(value.length() + 9), "len");
        outMsg.writeBeingId(npcId, "npc id");
        outMsg.writeString(value, CAST_S32(value.length()), "value");
        outMsg.writeInt8(0, "null byte");
    }
}

void NpcHandler::buy(const Being *const being) const
{
    if (being == nullptr)
        return;
    createOutPacket(CMSG_NPC_BUY_SELL_REQUEST);
    outMsg.writeBeingId(being->getId(), "npc id");
    outMsg.writeInt8(0, "action");
    EAthena::NpcRecv::mNpcTypeId = being->getSubType();
}

void NpcHandler::buy(const BeingId beingId) const
{
    createOutPacket(CMSG_NPC_BUY_SELL_REQUEST);
    outMsg.writeBeingId(beingId, "npc id");
    outMsg.writeInt8(0, "action");
    EAthena::NpcRecv::mNpcTypeId = BeingTypeId_zero;
}

void NpcHandler::sell(const BeingId beingId) const
{
    createOutPacket(CMSG_NPC_BUY_SELL_REQUEST);
    outMsg.writeBeingId(beingId, "npc id");
    outMsg.writeInt8(1, "action");
}

void NpcHandler::buyItem(const BeingId beingId A_UNUSED,
                         const int itemId,
                         const ItemColor color A_UNUSED,
                         const int amount) const
{
    createOutPacket(CMSG_NPC_BUY_REQUEST);
    outMsg.writeInt16(8, "len");
    outMsg.writeInt16(CAST_S16(amount), "amount");
    outMsg.writeItemId(itemId, "item id");
}

void NpcHandler::buyItems(STD_VECTOR<ShopItem*> &items) const
{
    int cnt = 0;
    const int pairSize = 2 + itemIdLen;

    FOR_EACH (STD_VECTOR<ShopItem*>::iterator, it, items)
    {
        ShopItem *const item = *it;
        const int usedQuantity = item->getUsedQuantity();
        const ItemTypeT type = item->getType();
        if (usedQuantity == 0)
            continue;
        if (type == ItemType::Weapon ||
            type == ItemType::Armor ||
            type == ItemType::PetEgg ||
            type == ItemType::PetArmor)
        {
            cnt += item->getUsedQuantity();
        }
        else
        {
            cnt ++;
        }
    }

    if (cnt > 100)
        return;

    createOutPacket(CMSG_NPC_BUY_REQUEST);
    outMsg.writeInt16(CAST_S16(4 + pairSize * cnt), "len");
    FOR_EACH (STD_VECTOR<ShopItem*>::iterator, it, items)
    {
        ShopItem *const item = *it;
        const int usedQuantity = item->getUsedQuantity();
        if (usedQuantity == 0)
            continue;
        item->increaseUsedQuantity(-usedQuantity);
        item->update();
        const ItemTypeT type = item->getType();
        if (type == ItemType::Weapon ||
            type == ItemType::Armor ||
            type == ItemType::PetEgg ||
            type == ItemType::PetArmor)
        {
            for (int f = 0; f < usedQuantity; f ++)
            {
                outMsg.writeInt16(CAST_S16(1), "amount");
                outMsg.writeItemId(item->getId(),
                    "item id");
            }
        }
        else
        {
            outMsg.writeInt16(CAST_S16(usedQuantity), "amount");
            outMsg.writeItemId(item->getId(), "item id");
        }
    }
}

void NpcHandler::sellItem(const BeingId beingId A_UNUSED,
                          const int itemId, const int amount) const
{
    createOutPacket(CMSG_NPC_SELL_REQUEST);
    outMsg.writeInt16(8, "len");
    outMsg.writeInt16(CAST_S16(itemId + INVENTORY_OFFSET),
        "item index");
    outMsg.writeInt16(CAST_S16(amount), "amount");
}

void NpcHandler::sellItems(STD_VECTOR<ShopItem*> &items) const
{
    const int pairSize = 4;
    int cnt = 0;

    FOR_EACH (STD_VECTOR<ShopItem*>::iterator, it, items)
    {
        ShopItem *const item = *it;
        const int usedQuantity = item->getUsedQuantity();
        if (usedQuantity == 0)
            continue;
        cnt ++;
    }

    createOutPacket(CMSG_NPC_SELL_REQUEST);
    outMsg.writeInt16(CAST_S16(4 + pairSize * cnt), "len");
    FOR_EACH (STD_VECTOR<ShopItem*>::iterator, it, items)
    {
        ShopItem *const item = *it;
        const int usedQuantity = item->getUsedQuantity();
        if (usedQuantity == 0)
            continue;
        item->increaseUsedQuantity(-usedQuantity);
        item->update();
        outMsg.writeInt16(CAST_S16(
            item->getCurrentInvIndex() + INVENTORY_OFFSET),
            "item index");
        outMsg.writeInt16(CAST_S16(usedQuantity), "amount");
    }
}

void NpcHandler::completeProgressBar() const
{
    createOutPacket(CMSG_NPC_COMPLETE_PROGRESS_BAR);
}

void NpcHandler::produceMix(const int nameId,
                            const int materialId1,
                            const int materialId2,
                            const int materialId3) const
{
    createOutPacket(CMSG_NPC_PRODUCE_MIX);
    outMsg.writeItemId(nameId, "item id");
    outMsg.writeItemId(materialId1, "material 1");
    outMsg.writeItemId(materialId2, "material 2");
    outMsg.writeItemId(materialId3, "material 3");
}

void NpcHandler::cooking(const CookingTypeT type,
                         const int nameId) const
{
    createOutPacket(CMSG_NPC_COOKING);
    outMsg.writeInt16(CAST_S16(type), "type");
    outMsg.writeItemId(nameId, "item id");
}

void NpcHandler::repair(const int index) const
{
    createOutPacket(CMSG_NPC_REPAIR);
    outMsg.writeInt16(CAST_S16(index), "index");
    // unused fields.
    outMsg.writeItemId(0, "item id");
    outMsg.writeInt8(0, "refine");
    for (int f = 0; f < maxCards; f ++)
        outMsg.writeItemId(0, "card");
}

void NpcHandler::refine(const int index) const
{
    createOutPacket(CMSG_NPC_REFINE);
    outMsg.writeInt32(index, "index");
}

void NpcHandler::identify(const int index) const
{
    createOutPacket(CMSG_NPC_IDENTIFY);
    outMsg.writeInt16(CAST_S16(index), "index");
}

void NpcHandler::selectArrow(const int nameId) const
{
    createOutPacket(CMSG_NPC_SELECT_ARROW);
    outMsg.writeItemId(nameId, "item id");
}

void NpcHandler::selectAutoSpell(const int skillId) const
{
    createOutPacket(CMSG_NPC_SELECT_AUTO_SPELL);
    outMsg.writeInt32(CAST_S16(skillId), "skill id");
}

BeingId NpcHandler::getNpc(Net::MessageIn &msg,
                           const NpcActionT action)
{
    const BeingId npcId = msg.readBeingId("npc id");

    const NpcDialogs::const_iterator diag = NpcDialog::mNpcDialogs.find(npcId);

    if (diag == NpcDialog::mNpcDialogs.end())
    {
        Ea::NpcRecv::mDialog = nullptr;
        // Empty dialogs don't help
        if (action == NpcAction::Close)
        {
            closeDialog(npcId);
            return npcId;
        }
        else if (action == NpcAction::Next)
        {
            nextDialog(npcId);
            return npcId;
        }
        else
        {
            CREATEWIDGETV(Ea::NpcRecv::mDialog, NpcDialog, npcId);
            Ea::NpcRecv::mDialog->saveCamera();
            if (localPlayer != nullptr)
                localPlayer->stopWalking(false);
            NpcDialog::mNpcDialogs[npcId] = Ea::NpcRecv::mDialog;
        }
    }
    else
    {
        NpcDialog *const dialog = diag->second;
        if (Ea::NpcRecv::mDialog != nullptr && Ea::NpcRecv::mDialog != dialog)
            Ea::NpcRecv::mDialog->restoreCamera();
        Ea::NpcRecv::mDialog = dialog;
        if (Ea::NpcRecv::mDialog != nullptr)
            Ea::NpcRecv::mDialog->saveCamera();
    }
    return npcId;
}

void NpcHandler::requestAirship(const std::string &mapName,
                                const int itemId) const
{
    if (packetVersionRe < 20180321 &&
        packetVersionMain < 20180620 &&
        packets_zero == false)
    {
        return;
    }
    createOutPacket(CMSG_PRIVATE_AIRSHIP_REQUEST);
    outMsg.writeString(mapName, 16, "map name");
    outMsg.writeItemId(itemId, "item");
}

}  // namespace EAthena
