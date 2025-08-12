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

#include "unittests/unittests.h"

#include "enums/being/gender.h"

#include "utils/cast.h"

#include "debug.h"

TEST_CASE("Gender", "Enums")
{
    REQUIRE(CAST_S32(Gender::FEMALE) == 1);
    REQUIRE(Gender::FEMALE == static_cast<GenderT>(1));
    REQUIRE(CAST_S32(Gender::MALE) == 0);
    REQUIRE(Gender::MALE == static_cast<GenderT>(0));
}
