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

#include "resources/db/itemfielddb.h"

#include "configuration.h"

#include "utils/checkutils.h"
#include "utils/dtor.h"

#include "resources/beingcommon.h"

#include "resources/item/itemfieldtype.h"

#include "debug.h"

namespace
{
    ItemFieldInfos mRequiredInfos;
    ItemFieldInfos mAddInfos;
    bool mLoaded = false;
}  // namespace

void ItemFieldDb::load()
{
    if (mLoaded)
        unload();

    logger->log1("Initializing item field database...");

    loadXmlFile(paths.getStringValue("itemFieldsFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("itemFieldsPatchFile"), SkipError_true);
    loadXmlDir("itemFieldsPatchDir", loadXmlFile)
    mLoaded = true;
}

static void loadFields(XmlNodeConstPtr groupNode,
                       ItemFieldInfos &fields1,
                       ItemFieldInfos &fields2)
{
    for_each_xml_child_node(node, groupNode)
    {
        if (!xmlNameEqual(node, "field"))
            continue;

        const std::string name = XML::getProperty(node,
            "name",
            "");
        if (name.empty())
        {
            reportAlways("Empty name field in ItemFieldDb")
            continue;
        }
        const std::string description = XML::langProperty(node,
            "description",
            "");
        if (description.empty())
        {
            reportAlways("Empty description field in ItemFieldDb")
            continue;
        }
        const bool sign = XML::getBoolProperty(node,
            "signed",
            true);
        if (fields2.find(name) != fields2.end())
        {
            reportAlways(
                "Same field name detected in requeted and add groups: %s",
                name.c_str())
            continue;
        }
        if (fields1.find(name) != fields1.end())
        {
            reportAlways(
                "Same field name detected: %s",
                name.c_str())
            continue;
        }
        fields1[name] = new ItemFieldType(name,
            description,
            sign);
    }
}

void ItemFieldDb::loadXmlFile(const std::string &fileName,
                              const SkipError skipError)
{
    XML::Document doc(fileName,
        UseVirtFs_true,
        skipError);
    XmlNodeConstPtrConst rootNode = doc.rootNode();

    if ((rootNode == nullptr) || !xmlNameEqual(rootNode, "itemfields"))
    {
        logger->log("ItemFieldDb: Error while loading %s!",
            fileName.c_str());
        return;
    }

    for_each_xml_child_node(node, rootNode)
    {
        if (xmlNameEqual(node, "include"))
        {
            const std::string name = XML::getProperty(node, "name", "");
            if (!name.empty())
                loadXmlFile(name, skipError);
            continue;
        }

        if (xmlNameEqual(node, "required"))
            loadFields(node, mRequiredInfos, mAddInfos);
        else if (xmlNameEqual(node, "add"))
            loadFields(node, mAddInfos, mRequiredInfos);
    }
}

void ItemFieldDb::unload()
{
    logger->log1("Unloading item database...");

    delete_all(mRequiredInfos);
    mRequiredInfos.clear();
    delete_all(mAddInfos);
    mAddInfos.clear();
    mLoaded = false;
}

const ItemFieldInfos &ItemFieldDb::getRequiredFields()
{
    return mRequiredInfos;
}

const ItemFieldInfos &ItemFieldDb::getAddFields()
{
    return mAddInfos;
}
