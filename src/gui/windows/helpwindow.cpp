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

#include "gui/windows/helpwindow.h"

#include "configuration.h"

#include "enums/gui/layouttype.h"

#include "fs/paths.h"

#include "fs/virtfs/tools.h"

#include "gui/gui.h"

#include "gui/windows/setupwindow.h"

#include "gui/widgets/button.h"
#include "gui/widgets/staticbrowserbox.h"
#include "gui/widgets/layout.h"
#include "gui/widgets/scrollarea.h"

#include "input/inputmanager.h"

#include "utils/foreach.h"
#include "utils/gettext.h"
#include "utils/process.h"

#include "utils/translation/podict.h"
#include "utils/translation/translationmanager.h"

#include "debug.h"

HelpWindow *helpWindow = nullptr;

HelpWindow::HelpWindow() :
    // TRANSLATORS: help window name
    Window(_("Help"), Modal_false, nullptr, "help.xml"),
    LinkHandler(),
    ActionListener(),
    // TRANSLATORS: help window. button.
    mDYKButton(new Button(this, _("Did you know..."), "DYK",
        BUTTON_SKIN, this)),
    mBrowserBox(new StaticBrowserBox(this, Opaque_true,
        "browserbox.xml")),
    mScrollArea(new ScrollArea(this, mBrowserBox,
        Opaque_true, "help_background.xml")),
    mTagFileMap()
{
    setMinWidth(300);
    setMinHeight(220);
    setContentSize(455, 350);
    setWindowName("Help");
    setCloseButton(true);
    setResizable(true);
    setStickyButtonLock(true);

    if (setupWindow != nullptr)
        setupWindow->registerWindowForReset(this);

    setDefaultSize(500, 400, ImagePosition::CENTER, 0, 0);

    mBrowserBox->setOpaque(Opaque_false);

    mBrowserBox->setLinkHandler(this);
    if (gui != nullptr)
        mBrowserBox->setFont(gui->getHelpFont());
    mBrowserBox->setProcessVars(true);
    mBrowserBox->setEnableImages(true);
    mBrowserBox->setEnableKeys(true);
    mBrowserBox->setEnableTabs(true);

    place(4, 3, mDYKButton, 1, 1);
    place(0, 0, mScrollArea, 5, 3).setPadding(3);

    Layout &layout = getLayout();
    layout.setRowHeight(0, LayoutType::SET);

    loadWindowState();
    loadTags();
    enableVisibleSound(true);
    widgetResized(Event(nullptr));
}

void HelpWindow::action(const ActionEvent &event)
{
    if (event.getId() == "DYK")
        inputManager.executeAction(InputAction::WINDOW_DIDYOUKNOW);
}

void HelpWindow::handleLink(const std::string &link,
                            MouseEvent *const event A_UNUSED)
{
    if (!strStartWith(link, "http://") && !strStartWith(link, "https://"))
    {
        // need use separate variable
        const std::string helpFile = link;  // NOLINT
        loadHelp(helpFile);
    }
    else
    {
        openBrowser(link);
    }
}

void HelpWindow::loadHelp(const std::string &helpFile)
{
    if (!checkPath(helpFile))
        return;
    mBrowserBox->clearRows();
    loadFile("header");
    loadFile(helpFile);
    loadFile("footer");
    mScrollArea->setVerticalScrollAmount(0);
    mBrowserBox->updateHeight();
    setVisible(Visible_true);
}

void HelpWindow::loadHelpSimple(const std::string &helpFile)
{
    if (!checkPath(helpFile))
        return;
    mBrowserBox->clearRows();
    loadFile(helpFile);
    mScrollArea->setVerticalScrollAmount(0);
    mBrowserBox->updateHeight();
    setVisible(Visible_true);
}

void HelpWindow::loadFile(std::string file)
{
    trim(file);
    std::string helpPath = branding.getStringValue("helpPath");
    if (helpPath.empty())
        helpPath = paths.getStringValue("help");

    StringVect lines;
    TranslationManager::translateFile(pathJoin(helpPath, file).append(".txt"),
        translator, lines);

    for (size_t i = 0, sz = lines.size(); i < sz; ++i)
        mBrowserBox->addRow(lines[i], false);
}

void HelpWindow::loadTags()
{
    std::string helpPath = branding.getStringValue("tagsPath");
    if (helpPath.empty())
        helpPath = paths.getStringValue("tags");

    StringVect filesVect;
    VirtFs::getFilesInDir(helpPath, filesVect, ".idx");
    FOR_EACH (StringVectCIter, itVect, filesVect)
    {
        StringVect lines;
        VirtFs::loadTextFile(*itVect, lines);
        FOR_EACH (StringVectCIter, it, lines)
        {
            const std::string &str = *it;
            const size_t idx = str.find('|');
            if (idx != std::string::npos)
                mTagFileMap[str.substr(idx + 1)].insert(str.substr(0, idx));
        }
    }
}

void HelpWindow::search(const std::string &text0)
{
    std::string text = text0;
    trim(text);
    toLower(text);
    if (mTagFileMap.find(text) == mTagFileMap.end())
    {
        loadHelp("searchnotfound");
    }
    else
    {
        const HelpNames &names = mTagFileMap[text];
        if (names.size() == 1)
        {
            loadHelp(*names.begin());
        }
        else
        {
            if (translator == nullptr)
                return;
            mBrowserBox->clearRows();
            loadFile("header");
            loadFile("searchmany");
            FOR_EACH (HelpNamesCIter, it, names)
            {
                const char *const str = (*it).c_str();
                mBrowserBox->addRow(strprintf(" -> @@%s|%s@@", str,
                    translator->getChar(str)),
                    false);
            }
            loadFile("footer");
            mScrollArea->setVerticalScrollAmount(0);
            mBrowserBox->updateHeight();
            setVisible(Visible_true);
        }
    }
}
