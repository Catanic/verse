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

#ifndef NET_EATHENA_MAIL2RECV_H
#define NET_EATHENA_MAIL2RECV_H

#include <string>
#include <queue>

struct MailQueue;

namespace Net
{
    class MessageIn;
}  // namespace Net

namespace EAthena
{
    namespace Mail2Recv
    {
        extern std::queue<MailQueue*> mMailQueue;
        extern std::string mCheckedName;

        void processMailIcon(Net::MessageIn &msg);
        void processOpenNewMailWindow(Net::MessageIn &msg);
        void processAddItemResult(Net::MessageIn &msg);
        void processRemoveItemResult(Net::MessageIn &msg);
        void processCheckNameResult(Net::MessageIn &msg);
        void processSendResult(Net::MessageIn &msg);
        void processMailListPage(Net::MessageIn &msg);
        void processReadMail(Net::MessageIn &msg);
        void processMailDelete(Net::MessageIn &msg);
        void processRequestMoney(Net::MessageIn &msg);
        void processRequestItems(Net::MessageIn &msg);
    }  // namespace Mail2Recv
}  // namespace EAthena

#endif  // NET_EATHENA_MAIL2RECV_H
