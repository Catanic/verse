/*
 *  The ManaVerse Client
 *  Copyright (C) 2014-2020  The ManaPlus Developers
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

#ifndef LISTENERS_BANKLISTENER_H
#define LISTENERS_BANKLISTENER_H

#include "listeners/baselistener.hpp"

#include "localconsts.h"

class BankListener notfinal
{
    public:
        A_DELETE_COPY(BankListener)

        virtual void bankMoneyChanged(const int money) = 0;

        static void distributeEvent(const int money);

    defineListenerHeader(BankListener)
};

#endif  // LISTENERS_BANKLISTENER_H
