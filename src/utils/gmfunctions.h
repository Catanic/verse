/*
 *  The ManaVerse Client
 *  Copyright (C) 2016-2020  The ManaPlus Developers
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

#ifndef UTILS_GMFUNCTIONS_H
#define UTILS_GMFUNCTIONS_H

#include <string>

#include "localconsts.h"

namespace Gm
{
    void runCommand(const std::string &command,
                    const std::string &params);

    void runCommand(const std::string &command);

    void runCharCommand(const std::string &command,
                        const std::string &name,
                        const std::string &params);

    void runCharCommand(const std::string &command,
                        const std::string &name);
}  // namespace Gm

#endif  // UTILS_GMFUNCTIONS_H
