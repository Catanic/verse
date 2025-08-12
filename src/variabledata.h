/*
 *  The ManaVerse Client
 *  Copyright (C) 2010  The Mana Developers
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

#ifndef VARIABLEDATA_H
#define VARIABLEDATA_H

#include <string>

#include "localconsts.h"

class VariableData notfinal
{
    public:
        enum DataType
        {
            DATA_NONE = 0,
            DATA_INT,
            DATA_STRING,
            DATA_FLOAT,
            DATA_BOOL
        };

        VariableData()
        { }

        A_DELETE_COPY(VariableData)

        virtual ~VariableData()
        {}

        virtual int getType() const A_WARN_UNUSED = 0;
};

class IntData final : public VariableData
{
    public:
        explicit IntData(const int value) :
            VariableData(),
            mData(value)
        { }

        A_DELETE_COPY(IntData)

        int getData() const noexcept2 A_WARN_UNUSED
        { return mData; }

        int getType() const override final A_WARN_UNUSED
        { return DATA_INT; }

    private:
        int mData;
};

class StringData final : public VariableData
{
    public:
        explicit StringData(const std::string &value) :
            VariableData(),
            mData(value)
        { }

        A_DELETE_COPY(StringData)

        const std::string &getData() const noexcept2 A_WARN_UNUSED
        { return mData; }

        int getType() const override final A_WARN_UNUSED
        { return DATA_STRING; }

    private:
        std::string mData;
};

class FloatData final : public VariableData
{
    public:
        explicit FloatData(const double value) :
            VariableData(),
            mData(value)
        { }

        A_DELETE_COPY(FloatData)

        double getData() const noexcept2 A_WARN_UNUSED
        { return mData; }

        int getType() const override final A_WARN_UNUSED
        { return DATA_FLOAT; }

    private:
        double mData;
};

class BoolData final : public VariableData
{
    public:
        explicit BoolData(const bool value) :
            VariableData(),
            mData(value)
        { }

        A_DELETE_COPY(BoolData)

        bool getData() const noexcept2 A_WARN_UNUSED
        { return mData; }

        int getType() const override final A_WARN_UNUSED
        { return DATA_BOOL; }

    private:
        bool mData;
};

#endif  // VARIABLEDATA_H
