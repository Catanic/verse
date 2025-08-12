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

#include "net/tmwa/searchstorehandler.h"

#include "debug.h"

namespace TmwAthena
{

SearchStoreHandler::SearchStoreHandler() :
    Net::SearchStoreHandler()
{
    searchStoreHandler = this;
}

SearchStoreHandler::~SearchStoreHandler()
{
    searchStoreHandler = nullptr;
}

void SearchStoreHandler::search(const StoreSearchTypeT type A_UNUSED,
                                const int minPrice A_UNUSED,
                                const int maxPrice A_UNUSED,
                                const int itemId A_UNUSED) const
{
}

void SearchStoreHandler::nextPage() const
{
}

void SearchStoreHandler::close() const
{
}

void SearchStoreHandler::select(const int accountId A_UNUSED,
                                const int storeId A_UNUSED,
                                const int itemId A_UNUSED) const
{
}

}  // namespace TmwAthena
