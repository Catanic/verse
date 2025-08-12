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

#include "net/eathena/bankhandler.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocolout.h"

#include "debug.h"

extern int packetVersion;

namespace EAthena
{

BankHandler::BankHandler() :
    Net::BankHandler()
{
    bankHandler = this;
}

BankHandler::~BankHandler()
{
    bankHandler = nullptr;
}

void BankHandler::deposit(const int money) const
{
    if (packetVersion < 20130320)
        return;
    createOutPacket(CMSG_BANK_DEPOSIT);
    outMsg.writeInt32(0, "account id");
    outMsg.writeInt32(money, "money");
}

void BankHandler::withdraw(const int money) const
{
    if (packetVersion < 20130320)
        return;
    createOutPacket(CMSG_BANK_WITHDRAW);
    outMsg.writeInt32(0, "account id");
    outMsg.writeInt32(money, "money");
}

void BankHandler::check() const
{
    if (packetVersion < 20130320)
        return;
    createOutPacket(CMSG_BANK_CHECK);
    outMsg.writeInt32(0, "account id");
}

void BankHandler::open() const
{
    if (packetVersion < 20130417)
        return;
    createOutPacket(CMSG_BANK_OPEN);
    outMsg.writeInt32(0, "unused");
}

void BankHandler::close() const
{
    if (packetVersion < 20130417)
        return;
    createOutPacket(CMSG_BANK_CLOSE);
    outMsg.writeInt32(0, "unused");
}

}  // namespace EAthena
