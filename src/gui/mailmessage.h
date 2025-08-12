/*
 *  The ManaVerse Client
 *  Copyright (C) 2015-2020  The ManaPlus Developers
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

#ifndef GUI_MAILMESSAGE_H
#define GUI_MAILMESSAGE_H

#include "enums/net/mailmessagetype.h"

#include <string>

#include "localconsts.h"

struct MailMessage final
{
    MailMessage() :
        title(),
        sender(),
        strTime(),
        text(),
        id(0),
        time(0),
        expireTime(0),
        money(0),
        type(MailMessageType::Text),
        read(false),
        expired(false)
    {
    }

    A_DELETE_COPY(MailMessage)

    std::string title;
    std::string sender;
    std::string strTime;
    std::string text;
    int64_t id;
    int time;
    int64_t expireTime;
    int64_t money;
    MailMessageType::Type type;
    bool read;
    bool expired;
};
#endif  // GUI_MAILMESSAGE_H
