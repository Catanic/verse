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

#include "utils/itemxmlutils.h"

#include "utils/checkutils.h"
#include "utils/foreach.h"
#include "utils/stringutils.h"
#include "utils/translation/podict.h"

#include "resources/item/itemfieldtype.h"

#include "debug.h"

static void convertSignedValue(std::string &outStr,
                               const std::string &srcStr)
{
    if (!srcStr.empty() && srcStr[0] != '-')
        outStr = std::string("+").append(srcStr);
    else
        outStr = srcStr;
}

static bool readField(ItemFieldInfos::const_iterator it,
                      XmlNodeConstPtr node,
                      std::string &combined)
{
    const std::string fieldName = (*it).first;
    const ItemFieldType *const field = (*it).second;

    std::string value = XML::getProperty(node,
        fieldName.c_str(),
        "");
    if (value.empty())
        return false;

    StringVect tokens;
    splitToStringVector(tokens,
        value,
        '-');
    if (tokens.size() > 1)
    {
        std::string value1;
        std::string value2;
        if (field->sign)
        {
            convertSignedValue(value1, tokens[0]);
            convertSignedValue(value2, tokens[1]);
        }
        else
        {
            value1 = tokens[0];
            value2 = tokens[1];
        }
        value = strprintf("%s - %s",
            value1.c_str(),
            value2.c_str());
    }
    else
    {
        if (field->sign)
            convertSignedValue(value, value);
    }
    const std::string format = translator->getStr(field->description);
    combined = strprintf(format.c_str(),
        value.c_str());
    return true;
}

void readItemStatsString(std::string &effect,
                         XmlNodeConstPtr node,
                         const ItemFieldInfos &fields)
{
    if (translator == nullptr)
    {
        reportAlways("called readItemStatsString without translator")
        return;
    }

    FOR_EACH (ItemFieldInfos::const_iterator, it, fields)
    {
        std::string field;
        if (!readField(it, node, field))
            continue;
        if (!effect.empty())
            effect.append(" / ");
        effect.append(field);
    }
}

void readItemStatsVector(STD_VECTOR<std::string> &effect,
                         XmlNodeConstPtr node,
                         const ItemFieldInfos &fields)
{
    if (translator == nullptr)
    {
        reportAlways("called readItemStatsVector without translator")
        return;
    }

    FOR_EACH (ItemFieldInfos::const_iterator, it, fields)
    {
        std::string field;
        if (!readField(it, node, field))
            continue;
        effect.push_back(field);
    }
}
