/*
 *  The ManaVerse Client
 *  Copyright (C) 2008  Lloyd Bryant <lloyd_bryant@netzero.net>
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

#ifndef NET_EATHENA_PARTYHANDLER_H
#define NET_EATHENA_PARTYHANDLER_H

#include "net/ea/partyhandler.h"

namespace EAthena
{

class PartyHandler final : public Ea::PartyHandler
{
    public:
        PartyHandler();

        A_DELETE_COPY(PartyHandler)

        ~PartyHandler() override final;

        void create(const std::string &name) const override final;

        void invite(const std::string &name) const override final;

        void inviteResponse(const int partyId,
                            const bool accept) const override final;

        void leave() const override final;

        void kick(const Being *const being) const override final;

        void kick(const std::string &name) const override final;

        void chat(const std::string &text) const override final;

        void setShareExperience(const PartyShareT share) const override final;

        void setShareItems(const PartyShareT share) const override final;

        void changeLeader(const std::string &name) const override final;

        void allowInvite(const bool allow) const override final;

        void setShareAutoItems(const PartyShareT share) const override final;

        PartyShareT getShareAutoItems() const override final A_WARN_UNUSED;
};

}  // namespace EAthena

#endif  // NET_EATHENA_PARTYHANDLER_H
