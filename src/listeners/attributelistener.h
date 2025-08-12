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

#ifndef LISTENERS_ATTRIBUTELISTENER_H
#define LISTENERS_ATTRIBUTELISTENER_H

#include "enums/being/attributes.h"

#include "listeners/baselistener.hpp"

#include "localconsts.h"

class AttributeListener notfinal
{
    public:
        A_DELETE_COPY(AttributeListener)

        virtual void attributeChanged(const AttributesT id,
                                      const int64_t oldVal,
                                      const int64_t newVal) = 0;

        static void distributeEvent(const AttributesT id,
                                    const int64_t oldVal,
                                    const int64_t newVal);

    defineListenerHeader(AttributeListener)
};

#endif  // LISTENERS_ATTRIBUTELISTENER_H
