/*
 *  The ManaVerse Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
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

#include "gui/shortcut/spellshortcut.h"

#include "spellmanager.h"
#include "textcommand.h"

#include "debug.h"

SpellShortcut *spellShortcut = nullptr;

SpellShortcut::SpellShortcut() :
    mItemSelected(-1)
{
    load();
}

SpellShortcut::~SpellShortcut()
{
}

void SpellShortcut::load()
{
    for (unsigned f = 0; f < SPELLS_SIZE; f ++)
        mItems[f] = -1;

    if (spellManager == nullptr)
        return;

    const STD_VECTOR<TextCommand*> &spells = spellManager->getAll();
    unsigned k = 0;

    for (STD_VECTOR<TextCommand*>::const_iterator i = spells.begin(),
         i_end = spells.end(); i != i_end && k < SPELLS_SIZE; ++i)
    {
        mItems[k++] = (*i)->getId();
    }
}

unsigned int SpellShortcut::getSpellsCount()
{
    return SPELL_SHORTCUT_ITEMS;
}
