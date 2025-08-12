/*
 *  The ManaVerse Client
 *  Copyright (C) 2013-2020  The ManaPlus Developers
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

#ifndef RESOURCES_DB_QUESTDB_H
#define RESOURCES_DB_QUESTDB_H

#include "enums/simpletypes/skiperror.h"

#include "utils/vector.h"

#include "resources/questvar.h"

#include <string>

#include "localconsts.h"

struct QuestEffect;
struct QuestItem;

namespace QuestDb
{
    void load();

    void loadXmlFile(const std::string &fileName,
                     const SkipError skipError);

    void unload();

    NpcQuestVarMap *getVars();

    std::map<int, STD_VECTOR<QuestItem*> > *getQuests();

    STD_VECTOR<QuestEffect*> *getAllEffects();

    std::string getName(const int id);
}  // namespace QuestDb

#endif  // RESOURCES_DB_QUESTDB_H
