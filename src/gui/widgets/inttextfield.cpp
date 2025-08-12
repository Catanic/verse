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

#include "gui/widgets/inttextfield.h"

#ifdef USE_SDL2
#include "enums/input/keyvalue.h"
#endif  // USE_SDL2

#include "utils/stringutils.h"

#include <sstream>

#include "debug.h"

IntTextField::IntTextField(const Widget2 *const widget,
                           const int def,
                           const int min,
                           const int max,
                           const Enable enabled,
                           const int width) :
    TextField(widget, toString(def),
        LoseFocusOnTab_true, nullptr, std::string(), false),
    mMin(0),
    mMax(0),
    mDefault(def),
    mValue(def)
{
    if (min != 0 || max != 0)
        setRange(min, max);

    setEnabled(enabled == Enable_true);
    if (width != 0)
        setWidth(width);
}

void IntTextField::keyPressed(KeyEvent &event)
{
    const InputActionT action = event.getActionId();

    if (action == InputAction::GUI_DELETE
        || action == InputAction::GUI_BACKSPACE)
    {
        setText(std::string());
        if (mSendAlwaysEvents)
            distributeActionEvent();

        event.consume();
    }

#ifdef USE_SDL2
    const int val = event.getKey().getValue();
    if (val != KeyValue::TEXTINPUT)
        return;

    const std::string str = event.getText();
    if (str.empty())
        return;
    const size_t sz = str.size();
    for (size_t f = 0; f < sz; f ++)
    {
        const char chr = str[f];
        if (chr < '0' || chr > '9')
            return;
    }
#else  // USE_SDL2

    if (!event.getKey().isNumber())
        return;
#endif  // USE_SDL2

    TextField::keyPressed(event);

    std::istringstream s(getText());
    int i;
    s >> i;
    setValue(i);
    if (mSendAlwaysEvents)
        distributeActionEvent();
}

void IntTextField::setRange(const int min, const int max)
{
    mMin = min;
    mMax = max;

    if (mValue < mMin)
        mValue = mMin;
    else if (mValue > mMax)
        mValue = mMax;

    if (mDefault < mMin)
        mDefault = mMin;
    else if (mDefault > mMax)
        mDefault = mMax;
}

int IntTextField::getValue() const
{
    return getText().empty() ? mMin : mValue;
}

void IntTextField::setValue(const int i)
{
    if (i < mMin)
        mValue = mMin;
    else if (i > mMax)
        mValue = mMax;
    else
        mValue = i;

    const std::string valStr = toString(mValue);
    setText(valStr);
    setCaretPosition(CAST_U32(valStr.length()) + 1);
}

void IntTextField::setDefaultValue(const int value)
{
    if (value < mMin)
        mDefault = mMin;
    else if (value > mMax)
        mDefault = mMax;
    else
        mDefault = value;
}

void IntTextField::reset()
{
    setValue(mDefault);
}
