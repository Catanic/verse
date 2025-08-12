/*
 *  The ManaVerse Client
 *  Copyright (C) 2013-2020  The ManaPlus Developers
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

#include "net/tmwa/pethandler.h"

#include "debug.h"

namespace TmwAthena
{

PetHandler::PetHandler() :
    mRandCounter(1000)
{
    petHandler = this;
}

PetHandler::~PetHandler()
{
    petHandler = nullptr;
}

void PetHandler::move(const int x A_UNUSED,
                      const int y A_UNUSED) const
{
}

void PetHandler::emote(const uint8_t emoteId A_UNUSED)
{
}

void PetHandler::catchPet(const Being *const being A_UNUSED) const
{
}

void PetHandler::sendPetMessage(const int data A_UNUSED) const
{
}

void PetHandler::setName(const std::string &name A_UNUSED) const
{
}

void PetHandler::requestStatus() const
{
}

void PetHandler::feed() const
{
}

void PetHandler::dropLoot() const
{
}

void PetHandler::returnToEgg() const
{
}

void PetHandler::unequip() const
{
}

void PetHandler::setDirection(const unsigned char type A_UNUSED) const
{
}

void PetHandler::evolution(const Item *item A_UNUSED) const
{
}


}  // namespace TmwAthena
