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

#ifndef NET_EA_CHARSERVERHANDLER_H
#define NET_EA_CHARSERVERHANDLER_H

#include "net/charserverhandler.h"

namespace Ea
{

/**
 * Deals with incoming messages from the character server.
 */
class CharServerHandler notfinal : public Net::CharServerHandler
{
    public:
        A_DELETE_COPY(CharServerHandler)

        ~CharServerHandler() override;

        void setCharSelectDialog(CharSelectDialog *const window) const
                                 override final;

        void requestCharacters() const override final;

        unsigned int baseSprite() const override final A_CONST A_WARN_UNUSED;

        unsigned int hairSprite() const override final A_CONST A_WARN_UNUSED;

        unsigned int maxSprite() const override final A_CONST A_WARN_UNUSED;

        virtual void connect() const = 0;

        void clear() const override final;

    protected:
        CharServerHandler();
};

}  // namespace Ea

#endif  // NET_EA_CHARSERVERHANDLER_H
