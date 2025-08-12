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

#ifndef GUI_MODELS_SHOPITEMS_H
#define GUI_MODELS_SHOPITEMS_H

#include "enums/resources/item/itemtype.h"

#include "enums/simpletypes/itemcolor.h"

#include "gui/models/listmodel.h"

#include "utils/cast.h"
#include "utils/vector.h"

#include "localconsts.h"

class ShopItem;

/**
 * This class handles the list of items available in a shop.
 *
 * The addItem routine can automatically check, if an item already exists and
 * only adds duplicates to the old item, if one is found. The original
 * distribution of the duplicates can be retrieved from the item.
 *
 * This functionality can be enabled in the constructor.
 */
class ShopItems final : public ListModel
{
    public:
        /**
         * Constructor.
         *
         * @param mergeDuplicates lets the Shop look for duplicate entries and
         *                        merges them to one item.
         */
        ShopItems(const bool mergeDuplicates,
                  const std::string &currency);

        A_DELETE_COPY(ShopItems)

        ~ShopItems() override final;

        /**
         * Adds an item to the list.
         */
        ShopItem *addItem(const int id,
                          const ItemTypeT type,
                          const ItemColor color,
                          const int amount,
                          const int price);

        /**
         * Adds an item to the list (used by sell dialog). Looks for
         * duplicate entries, if mergeDuplicates was turned on.
         *
         * @param inventoryIndex the inventory index of the item
         * @param id the id of the item
         * @param amount number of available copies of the item
         * @param price price of the item
         */
        ShopItem *addItem2(const int inventoryIndex,
                           const int id,
                           const ItemTypeT type,
                           const ItemColor color,
                           const int amount,
                           const int price);

        ShopItem *addItemNoDup(const int id,
                               const ItemTypeT type,
                               const ItemColor color,
                               const int amount,
                               const int price);

        /**
         * Returns the number of items in the shop.
         */
        int getNumberOfElements() override final A_WARN_UNUSED
        { return CAST_S32(mShopItems.size()); }

        bool empty() const noexcept2 A_WARN_UNUSED
        { return mShopItems.empty(); }

        /**
         * Returns the name of item number i in the shop.
         *
         * @param i the index to retrieve
         */
        std::string getElementAt(int i) override final A_WARN_UNUSED;

        /**
         * Returns the item number i in the shop.
         */
        ShopItem *at(const size_t i) const A_WARN_UNUSED;

        /**
         * Removes an element from the shop.
         *
         * @param i index to remove
         */
        void erase(const unsigned int i);

        /**
         * Removes an element from the shop and destroy it.
         *
         * @param i index to remove
         */
        void del(const unsigned int i);

        /**
         * Clears the list of items in the shop.
         */
        void clear();

        STD_VECTOR<ShopItem*> &items() A_WARN_UNUSED
        { return mShopItems; }

        STD_VECTOR<ShopItem*> &allItems() A_WARN_UNUSED
        { return mAllShopItems; }

        void setMergeDuplicates(const bool b)
        { mMergeDuplicates = b; }

        void updateList();

    private:
        /**
         * Searches the current items in the shop for the specified
         * id and returns the item if found, or 0 else.
         *
         * @return the item found or 0
         */
        ShopItem *findItem(const int id,
                           const ItemColor color) const A_WARN_UNUSED;

        bool findInAllItems(STD_VECTOR<ShopItem*>::iterator &it,
                            const ShopItem *const item);

        /** The list of items in the shop. */
        STD_VECTOR<ShopItem*> mAllShopItems;

        STD_VECTOR<ShopItem*> mShopItems;

        std::string mCurrency;

        /** Look for duplicate entries on addition. */
        bool mMergeDuplicates;
};

#endif  // GUI_MODELS_SHOPITEMS_H
