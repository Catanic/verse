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

#ifndef RESOURCES_MAP_PROPERTIES_H
#define RESOURCES_MAP_PROPERTIES_H

#include "localconsts.h"

#include <map>
#include <sstream>

/**
 * A class holding a set of properties.
 */
class Properties notfinal
{
    public:
        Properties() :
            mProperties()
        {
        }

        A_DELETE_COPY(Properties)

        /**
         * Destructor.
         */
        virtual ~Properties()
        { }

        /**
         * Get a map property.
         *
         * @param name The name of the property.
         * @param def  Default value, empty string by default.
         * @return the value of the given property or the given default when it
         *         doesn't exist.
         */
        const std::string getProperty(const std::string &name,
                                      const std::string &def)
                                      const A_WARN_UNUSED
        {
            const PropertyMap::const_iterator i = mProperties.find(name);
            return (i != mProperties.end()) ? i->second : def;
        }

        /**
         * Gets a map property as a float.
         *
         * @param name The name of the property.
         * @param def  Default value, 0.0F by default.
         * @return the value of the given property or the given default when it
         *         doesn't exist.
         */
        float getFloatProperty(const std::string &name,
                               const float def) const A_WARN_UNUSED
        {
            const PropertyMap::const_iterator i = mProperties.find(name);
            float ret = def;
            if (i != mProperties.end())
            {
                std::stringstream ss;
                ss.str(i->second);
                ss >> ret;
            }
            return ret;
        }

        /**
         * Gets a map property as a boolean.
         *
         * @param name The name of the property.
         * @param def  Default value, false by default.
         * @return the value of the given property or the given default when it
         *         doesn't exist.
         */
        bool getBoolProperty(const std::string &name,
                             const bool def) const A_WARN_UNUSED
        {
            const PropertyMap::const_iterator i = mProperties.find(name);
            bool ret = def;
            if (i != mProperties.end())
            {
                if (i->second == "true")
                    ret = true;
                if (i->second == "false")
                    ret = false;
            }
            return ret;
        }

        /**
         * Returns whether a certain property is available.
         *
         * @param name The name of the property.
         * @return <code>true</code> when a property is defined,
         *         <code>false</code> otherwise.
         */
        bool hasProperty(const std::string &name) const A_WARN_UNUSED
        { return (mProperties.find(name) != mProperties.end()); }

        /**
         * Set a map property.
         *
         * @param name  The name of the property.
         * @param value The value of the property.
         */
        void setProperty(const std::string &name, const std::string &value)
        { mProperties[name] = value; }


    private:
        typedef std::map<std::string, std::string> PropertyMap;
        PropertyMap mProperties;
};

#endif  // RESOURCES_MAP_PROPERTIES_H
