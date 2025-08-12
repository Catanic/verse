/*
 *  The ManaVerse Client
 *  Copyright (C) 2008  Aethyra Development Team
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

#include "resources/db/chardb.h"

#include "configuration.h"
#include "logger.h"

#include "debug.h"

namespace
{
    bool mLoaded = false;
    bool showHiddenGender = false;
    unsigned mMinHairColor = 0;
    unsigned mMaxHairColor = 0;
    unsigned mMinHairStyle = 0;
    unsigned mMaxHairStyle = 0;
    unsigned mMinStat = 0;
    unsigned mMaxStat = 0;
    unsigned mSumStat = 0;
    unsigned mMinLook = 0;
    unsigned mMaxLook = 0;
    unsigned mMinRace = 0;
    unsigned mMaxRace = 30;
    STD_VECTOR<BeingSlot> mDefaultItems;
}  // namespace

void CharDB::load()
{
    if (mLoaded)
        unload();

    logger->log1("Initializing char database...");

    XML::Document *doc = new XML::Document(
        paths.getStringValue("charCreationFile"),
        UseVirtFs_true,
        SkipError_false);
    XmlNodeConstPtrConst root = doc->rootNode();

    if ((root == nullptr) || !xmlNameEqual(root, "chars"))
    {
        logger->log("CharDB: Failed to parse %s.",
            paths.getStringValue("charCreationFile").c_str());
        delete doc;
        return;
    }

    for_each_xml_child_node(node, root)
    {
        if (xmlNameEqual(node, "haircolor"))
        {
            loadMinMax(node, &mMinHairColor, &mMaxHairColor);
        }
        else if (xmlNameEqual(node, "hairstyle"))
        {
            loadMinMax(node, &mMinHairStyle, &mMaxHairStyle);
        }
        else if (xmlNameEqual(node, "look"))
        {
            loadMinMax(node, &mMinLook, &mMaxLook);
        }
        else if (xmlNameEqual(node, "stat"))
        {
            loadMinMax(node, &mMinStat, &mMaxStat);
            mSumStat = XML::getProperty(node, "sum", 0);
        }
        else if (xmlNameEqual(node, "item"))
        {
            const int id = XML::getProperty(node, "id", 0);
            if (id > 0)
            {
                BeingSlot slot;
                slot.spriteId = id;
                for (int f = 0; f < maxCards; f ++)
                {
                    const std::string cardName = strprintf("card%d", f + 1);
                    slot.cardsId.cards[f] = XML::getProperty(node,
                        cardName.c_str(),
                        0);
                }
                mDefaultItems.push_back(slot);
            }
        }
        else if (xmlNameEqual(node, "race"))
        {
            loadMinMax(node, &mMinRace, &mMaxRace);
        }
        else if (xmlNameEqual(node, "flags"))
        {
            loadFlags(node);
        }
    }

    delete doc;

    mLoaded = true;
}

void CharDB::loadMinMax(XmlNodeConstPtr node,
                        unsigned *restrict const min,
                        unsigned *restrict const max)
{
    if (min != nullptr)
        *min = XML::getProperty(node, "min", 1);
    if (max != nullptr)
        *max = XML::getProperty(node, "max", 10);
}

void CharDB::loadFlags(XmlNodeConstPtr node)
{
    showHiddenGender = XML::getBoolProperty(node, "showHiddeGender", false);
}

void CharDB::unload()
{
    logger->log1("Unloading char database...");

    mLoaded = false;
}

unsigned CharDB::getMinHairColor()
{
    return mMinHairColor;
}

unsigned CharDB::getMaxHairColor()
{
    return mMaxHairColor;
}

unsigned CharDB::getMinHairStyle()
{
    return mMinHairStyle;
}

unsigned CharDB::getMaxHairStyle()
{
    return mMaxHairStyle;
}

unsigned CharDB::getMinStat()
{
    return mMinStat;
}

unsigned CharDB::getMaxStat()
{
    return mMaxStat;
}

unsigned CharDB::getSumStat()
{
    return mSumStat;
}

unsigned CharDB::getMinLook()
{
    return mMinLook;
}

unsigned CharDB::getMaxLook()
{
    return mMaxLook;
}

unsigned CharDB::getMinRace()
{
    return mMinRace;
}

unsigned CharDB::getMaxRace()
{
    return mMaxRace;
}

const STD_VECTOR<BeingSlot> &CharDB::getDefaultItems()
{
    return mDefaultItems;
}

bool CharDB::getShowHiddenGender()
{
    return showHiddenGender;
}
