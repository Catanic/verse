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

#include "resources/db/questdb.h"

#include "configuration.h"
#include "logger.h"

#include "utils/dtor.h"
#include "utils/gettext.h"

#include "utils/translation/podict.h"

#include "resources/beingcommon.h"
#include "resources/questeffect.h"
#include "resources/questitem.h"

#include "debug.h"

namespace
{
    // quest variables: var, (val1, val2, val3, time)
    NpcQuestVarMap mVars;
    // quests: var, quests
    std::map<int, STD_VECTOR<QuestItem*> > mQuests;
    STD_VECTOR<QuestEffect*> mAllEffects;
}  // namespace

void QuestDb::load()
{
    unload();
    logger->log1("Initializing quest database...");
    loadXmlFile(paths.getStringValue("questsFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("questsPatchFile"), SkipError_true);
    loadXmlDir("questsPatchDir", loadXmlFile)
}

static void loadQuest(const int var,
                      XmlNodeConstPtr node)
{
    if (node == nullptr)
        return;
    QuestItem *const quest = new QuestItem;
    // TRANSLATORS: quests window quest name
    quest->name = XML::langProperty(node, "name", _("unknown"));
    quest->group = XML::getProperty(node, "group", "");
    std::string incompleteStr = XML::getProperty(node, "incomplete", "");
    std::string completeStr = XML::getProperty(node, "complete", "");
    if (incompleteStr.empty() && completeStr.empty())
    {
        logger->log("complete flags incorrect");
        delete quest;
        return;
    }
    splitToIntSet(quest->incomplete, incompleteStr, ',');
    splitToIntSet(quest->complete, completeStr, ',');
    if (quest->incomplete.empty() && quest->complete.empty())
    {
        logger->log("complete flags incorrect");
        delete quest;
        return;
    }
    if (quest->incomplete.empty() || quest->complete.empty())
        quest->broken = true;

    for_each_xml_child_node(dataNode, node)
    {
        if (!xmlTypeEqual(dataNode, XML_ELEMENT_NODE))
            continue;
        XmlChar *const data = reinterpret_cast<XmlChar*>(
            XmlNodeGetContent(dataNode));
        if (data == nullptr)
            continue;
        std::string str = translator->getStr(data);
        XmlFree(data);

        for (int f = 1; f < 100; f ++)
        {
            const std::string key = strprintf("text%d", f);
            const std::string val = XML::getProperty(dataNode,
                key.c_str(),
                "");
            if (val.empty())
                break;
            const std::string param = strprintf("{@@%d}", f);
            replaceAll(str, param, val);
        }
        replaceItemLinks(str);
        if (xmlNameEqual(dataNode, "text"))
        {
            quest->texts.push_back(QuestItemText(str,
                QuestType::TEXT,
                std::string(),
                std::string()));
        }
        else if (xmlNameEqual(dataNode, "name"))
        {
            quest->texts.push_back(QuestItemText(str,
                QuestType::NAME,
                std::string(),
                std::string()));
        }
        else if (xmlNameEqual(dataNode, "reward"))
        {
            quest->texts.push_back(QuestItemText(str,
                QuestType::REWARD,
                std::string(),
                std::string()));
        }
        else if (xmlNameEqual(dataNode, "questgiver") ||
                 xmlNameEqual(dataNode, "giver"))
        {
            quest->texts.push_back(QuestItemText(str,
                QuestType::GIVER,
                std::string(),
                std::string()));
        }
        else if (xmlNameEqual(dataNode, "coordinates"))
        {
            const std::string str1 = toString(XML::getIntProperty(
                dataNode, "x", 0, 1, 1000));
            const std::string str2 = toString(XML::getIntProperty(
                dataNode, "y", 0, 1, 1000));
            quest->texts.push_back(QuestItemText(str,
                QuestType::COORDINATES,
                str1,
                str2));
        }
        else if (xmlNameEqual(dataNode, "npc"))
        {
            quest->texts.push_back(QuestItemText(str,
                QuestType::NPC,
                std::string(),
                std::string()));
        }
    }
    quest->var = var;
    mQuests[var].push_back(quest);
}

static void loadEffect(const int var,
                       XmlNodeConstPtr node)
{
    QuestEffect *const effect = new QuestEffect;
    effect->map = XML::getProperty(node, "map", "");
    effect->id = fromInt(XML::getProperty(node, "npc", -1), BeingTypeId);
    effect->effectId = XML::getProperty(node, "effect", -1);
    const std::string values = XML::getProperty(node, "value", "");
    splitToIntSet(effect->values, values, ',');

    if (effect->map.empty() || effect->id == BeingTypeId_negOne
        || effect->effectId == -1 || values.empty())
    {
        delete effect;
        return;
    }
    effect->var = var;
    mAllEffects.push_back(effect);
}

void QuestDb::loadXmlFile(const std::string &fileName,
                          const SkipError skipError)
{
    XML::Document doc(fileName,
        UseVirtFs_true,
        skipError);
    XmlNodeConstPtrConst root = doc.rootNode();
    if (root == nullptr)
        return;

    for_each_xml_child_node(varNode, root)
    {
        if (xmlNameEqual(varNode, "include"))
        {
            const std::string name = XML::getProperty(varNode, "name", "");
            if (!name.empty())
                loadXmlFile(name, skipError);
            continue;
        }
        else if (xmlNameEqual(varNode, "var"))
        {
            const int id = XML::getProperty(varNode, "id", 0);
            if (id < 0)
                continue;
            mVars[id] = QuestVar();
            for_each_xml_child_node(questNode, varNode)
            {
                if (xmlNameEqual(questNode, "quest"))
                    loadQuest(id, questNode);
                else if (xmlNameEqual(questNode, "effect"))
                    loadEffect(id, questNode);
            }
        }
    }
}


void QuestDb::unload()
{
    logger->log1("Unloading quest database...");
    for (std::map<int, STD_VECTOR<QuestItem*> >::iterator it
         = mQuests.begin(), it_end = mQuests.end(); it != it_end; ++ it)
    {
        STD_VECTOR<QuestItem*> &quests = (*it).second;
        for (STD_VECTOR<QuestItem*>::iterator it2 = quests.begin(),
             it2_end = quests.end(); it2 != it2_end; ++ it2)
        {
            delete *it2;
        }
    }
    delete_all(mAllEffects);
    mAllEffects.clear();
    mQuests.clear();
}

NpcQuestVarMap *QuestDb::getVars()
{
    return &mVars;
}

std::map<int, STD_VECTOR<QuestItem*> > *QuestDb::getQuests()
{
    return &mQuests;
}

STD_VECTOR<QuestEffect*> *QuestDb::getAllEffects()
{
    return &mAllEffects;
}

std::string QuestDb::getName(const int id)
{
    std::map<int, STD_VECTOR<QuestItem*> >::const_iterator it =
        mQuests.find(id);
    if (it == mQuests.end())
    {
        // TRANSLATORS: quests window quest name
        return _("unknown");
    }
    const STD_VECTOR<QuestItem*> &items = (*it).second;
    if (items.empty())
    {
        // TRANSLATORS: quests window quest name
        return _("unknown");
    }
    return items[0]->name;
}
