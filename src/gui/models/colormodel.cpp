/*
 *  The ManaVerse Client
 *  Copyright (C) 2012-2020  The ManaPlus Developers
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

#include "gui/models/colormodel.h"

#include "gui/widgets/widget2.h"

#include "utils/gettext.h"

#include "debug.h"

ColorModel::ColorModel() :
    ListModel(),
    mNames(),
    mColors()
{
}

ColorModel::~ColorModel()
{
}

int ColorModel::getNumberOfElements()
{
    return CAST_S32(mNames.size());
}

std::string ColorModel::getElementAt(int i)
{
    if (i >= getNumberOfElements() || i < 0)
        return "???";
    return mNames[CAST_SIZE(i)];
}

const ColorPair *ColorModel::getColorAt(const int i) const
{
    if (i >= CAST_S32(mColors.size()) || i < 0)
        return &mColors[0];

    return &mColors[CAST_SIZE(i)];
}

void ColorModel::add(const std::string &name, const Color *const color1,
                     const Color *const color2)
{
    mNames.push_back(name);
    mColors.push_back(ColorPair(color1, color2));
}

#define addColor(name, color) \
    model->add(name, &widget->getThemeColor(ThemeColorId::color, 255U), \
        &widget->getThemeColor(ThemeColorId::color##_OUTLINE, 255U))

ColorModel *ColorModel::createDefault(const Widget2 *const widget)
{
    ColorModel *const model = new ColorModel;
    if (widget == nullptr)
        return model;
    // TRANSLATORS: color name
    addColor(_("black"), BLACK);
    // TRANSLATORS: color name
    addColor(_("red"), RED);
    // TRANSLATORS: color name
    addColor(_("green"), GREEN);
    // TRANSLATORS: color name
    addColor(_("blue"), BLUE);
    // TRANSLATORS: color name
    addColor(_("gold"), ORANGE);
    // TRANSLATORS: color name
    addColor(_("yellow"), YELLOW);
    // TRANSLATORS: color name
    addColor(_("pink"), PINK);
    // TRANSLATORS: color name
    addColor(_("purple"), PURPLE);
    // TRANSLATORS: color name
    addColor(_("grey"), GRAY);
    // TRANSLATORS: color name
    addColor(_("brown"), BROWN);
    return model;
}
