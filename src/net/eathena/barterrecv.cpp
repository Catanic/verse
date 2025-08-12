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

#include "net/eathena/barterrecv.h"

#include "net/messagein.h"

#include "debug.h"

extern int itemIdLen;

namespace EAthena
{

namespace BarterRecv
{
    BuyDialog *mBuyDialog = nullptr;
}  // namespace BarterRecv


void BarterRecv::processOpenShop(Net::MessageIn &msg)
{
    const int count = (msg.readInt16("len") - 4) / (itemIdLen * 2 + 17);
    for (int f = 0; f < count; f ++)
    {
        msg.readItemId("item id");
        msg.readUInt8("item type");
        msg.readInt32("amount");
        msg.readItemId("currency item id");
        msg.readInt32("currency amount");
        msg.readInt32("weight");
        msg.readInt32("shop index");
    }
}

}  // namespace EAthena
