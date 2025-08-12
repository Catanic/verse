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

#include "resources/db/statdb.h"

#include "configuration.h"

#include "enums/being/attributesstrings.h"

#include "utils/checkutils.h"

#include "resources/beingcommon.h"

#include "utils/gettext.h"

#include "debug.h"

namespace
{
    bool mLoaded = false;
    STD_VECTOR<BasicStat> mBasicStats;
    std::map<std::string, STD_VECTOR<BasicStat> > mStats;
    STD_VECTOR<std::string> mPages;
}  // namespace

void StatDb::addDefaultStats()
{
    mBasicStats.push_back(BasicStat(Attributes::PLAYER_STR,
        "str",
        // TRANSLATORS: player stat
        _("Strength")));
    mBasicStats.push_back(BasicStat(Attributes::PLAYER_AGI,
        "agi",
        // TRANSLATORS: player stat
        _("Agility")));
    mBasicStats.push_back(BasicStat(Attributes::PLAYER_VIT,
        "vit",
        // TRANSLATORS: player stat
        _("Vitality")));
    mBasicStats.push_back(BasicStat(Attributes::PLAYER_INT,
        "int",
        // TRANSLATORS: player stat
        _("Intelligence")));
    mBasicStats.push_back(BasicStat(Attributes::PLAYER_DEX,
        "dex",
        // TRANSLATORS: player stat
        _("Dexterity")));
    mBasicStats.push_back(BasicStat(Attributes::PLAYER_LUK,
        "luk",
        // TRANSLATORS: player stat
        _("Luck")));
}

const STD_VECTOR<BasicStat> &StatDb::getBasicStats()
{
    return mBasicStats;
}

const STD_VECTOR<BasicStat> &StatDb::getStats(const std::string &page)
{
    return mStats[page];
}

const STD_VECTOR<std::string> &StatDb::getPages()
{
    return mPages;
}

void StatDb::load()
{
    if (mLoaded)
        unload();

    logger->log1("Initializing stat database...");

    loadXmlFile(paths.getStringValue("statFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("statPatchFile"), SkipError_true);
    loadXmlDir("statPatchDir", loadXmlFile)
    mLoaded = true;
}

static void loadStats(XmlNodeConstPtr rootNode,
                      const bool isBasic,
                      const std::string &page)
{
    const int maxAttr = static_cast<int>(Attributes::MAX_ATTRIBUTE);
    STD_VECTOR<BasicStat> &stats = isBasic ? mBasicStats : mStats[page];
    if (!isBasic)
        mPages.push_back(page);

    for_each_xml_child_node(node, rootNode)
    {
        if (xmlNameEqual(node, "stat"))
        {
            const std::string name = XML::getProperty(node, "name", "");
            const std::string attr = XML::getProperty(node, "attr", "");
            const std::string tag  = XML::getProperty(node, "tag", "");
            if (attr.empty() || AttributesEnum::contains(attr) == false)
            {
                const int id = XML::getProperty(node, "id", 0);
                if (id <= 0 || id >= maxAttr)
                {
                    reportAlways("Wrong attr or id for %s stat with name %s",
                                 (isBasic ? "basic" : "extended"),
                                 name.c_str())
                }
                else
                {
                    reportAlways("StatDb: stat name '%s' has empty attr field"
                                 ", using legacy method (id).",
                                 name.c_str());
                    stats.push_back(BasicStat(static_cast<AttributesT>(id),
                                              tag, name));
                }
            }
            else
            {
                stats.push_back(BasicStat(AttributesEnum::get(attr),
                                          tag, name));
            }
        }
    }
}

void StatDb::loadXmlFile(const std::string &fileName,
                         const SkipError skipError)
{
    XML::Document doc(fileName,
        UseVirtFs_true,
        skipError);
    XmlNodeConstPtrConst rootNode = doc.rootNode();

    if ((rootNode == nullptr) || !xmlNameEqual(rootNode, "stats"))
    {
        logger->log("StatDb: Error while loading %s!",
            fileName.c_str());
        if (skipError == SkipError_false)
            addDefaultStats();
        return;
    }

    for_each_xml_child_node(node, rootNode)
    {
        if (xmlNameEqual(node, "include"))
        {
            const std::string name = XML::getProperty(node, "name", "");
            if (!name.empty())
                loadXmlFile(name, skipError);
        }
        else if (xmlNameEqual(node, "basic"))
        {
            loadStats(node, true, "");
        }
        else if (xmlNameEqual(node, "extended"))
        {
            // TRANSLATORS: stats page name
            loadStats(node, false, _("Extended"));
        }
        else if (xmlNameEqual(node, "page"))
        {
            std::string page = XML::langProperty(node, "name", "");
            if (page.empty())
            {
                reportAlways("Page without name in stats.xml")
                page = "Unknown";
            }
            loadStats(node, false, page);
        }
    }

    if (skipError == SkipError_false)
    {
        if (mBasicStats.empty() &&
            mStats.empty())
        {
            reportAlways("StatDb: no stats found")
            addDefaultStats();
        }
    }
}

void StatDb::unload()
{
    logger->log1("Unloading stat database...");

    mBasicStats.clear();
    mStats.clear();
    mPages.clear();
    mLoaded = false;
}
