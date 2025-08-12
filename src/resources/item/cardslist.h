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

#ifndef RESOURCES_ITEM_CARDSLIST_H
#define RESOURCES_ITEM_CARDSLIST_H

#include "const/resources/item/cards.h"

#include "localconsts.h"

struct CardsList final
{
    CardsList(const int card0,
              const int card1,
              const int card2,
              const int card3)
    {
        cards[0] = card0;
        cards[1] = card1;
        cards[2] = card2;
        cards[3] = card3;
    }

    explicit CardsList(const int *const cards0)
    {
        if (cards0 != nullptr)
        {
            for (int f = 0; f < maxCards; f ++)
                cards[f] = cards0[f];
        }
        else
        {
            for (int f = 0; f < maxCards; f ++)
                cards[f] = 0;
        }
    }

    A_DEFAULT_COPY(CardsList)

    bool isEmpty() const noexcept2 A_WARN_UNUSED
    {
        return cards[0] == 0 &&
            cards[1] == 0 &&
            cards[2] == 0 &&
            cards[3] == 0;
    }

    int cards[maxCards];
};

extern CardsList zeroCards;

#endif  // RESOURCES_ITEM_CARDSLIST_H
