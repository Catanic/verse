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

#include "listeners/banklistener.h"

#include "utils/foreach.h"

#include "debug.h"

defineListener(BankListener)

void BankListener::distributeEvent(const int money)
{
    FOR_EACH (STD_VECTOR<BankListener*>::iterator,
              it, mListeners)
    {
        BankListener *const listener = *it;
        if (listener != nullptr)
            listener->bankMoneyChanged(money);
    }
}
