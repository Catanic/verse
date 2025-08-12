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

#ifndef NET_EA_BUYSELLHANDLER_H
#define NET_EA_BUYSELLHANDLER_H

#include "net/buysellhandler.h"

namespace Ea
{

class BuySellHandler notfinal : public Net::BuySellHandler
{
    public:
        BuySellHandler();

        A_DELETE_COPY(BuySellHandler)

        void cleanDialogReference(const BuyDialog *const dialog) const
                                  override final;
};

}  // namespace Ea

#endif  // NET_EA_BUYSELLHANDLER_H
