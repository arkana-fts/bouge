////////////////////////////////////////////////////////////
//
// Bouge - Modern and flexible skeletal animation library
// Copyright (C) 2010 Lucas Beyer (pompei2@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////
#include <bouge/IOModules/XMLParserCommon/XMLAttributes.hpp>

#include <sstream>
#include <iterator>
#include <stdexcept>
#include <typeinfo>
#include <algorithm>
#include <cctype>

namespace bouge
{
    XMLAttributes::XMLAttributes()
    {}

    XMLAttributes::~XMLAttributes()
    {}

    void XMLAttributes::add(const std::string& attrName, const std::string& attrValue)
    {
        m_attrs[attrName] = attrValue;
    }

    void XMLAttributes::remove(const std::string& attrName)
    {
        AttributeMap::iterator pos = m_attrs.find(attrName);

        if (pos != m_attrs.end())
            m_attrs.erase(pos);
    }

    bool XMLAttributes::exists(const std::string& attrName) const
    {
        return m_attrs.find(attrName) != m_attrs.end();
    }

    size_t XMLAttributes::getCount(void) const
    {
        return m_attrs.size();
    }

    const std::string& XMLAttributes::getValue(const std::string& attrName) const
    {
        AttributeMap::const_iterator pos = m_attrs.find(attrName);

        if (pos != m_attrs.end())
        {
            return (*pos).second;
        }
        else
        {
            throw std::runtime_error("XMLAttributes::getValue - no value exists for an attribute named '" + attrName + "'.");
        }
    }

    const std::string& XMLAttributes::getValueAsString(const std::string& attrName, const std::string& def) const
    {
        return (exists(attrName)) ? getValue(attrName) : def;
    }


    bool XMLAttributes::getValueAsBool(const std::string& attrName, bool def) const
    {
        if (!exists(attrName))
        {
            return def;
        }

        const std::string& val = getValue(attrName);

        if (val == "false" || val == "0")
        {
            return false;
        }
        else if (val == "true" || val == "1")
        {
            return true;
        }
        else
        {
            throw std::runtime_error("XMLAttributes::getValueAsInteger - failed to convert attribute '" + attrName + "' with value '" + getValue(attrName) + "' to bool.");
        }
    }

    int XMLAttributes::getValueAsInteger(const std::string& attrName, int def) const
    {
        if (!exists(attrName))
        {
            return def;
        }

        int val;
        std::istringstream strm(getValue(attrName).c_str());

        strm >> val;

        // success?
        if (strm.fail())
        {
            throw std::runtime_error("XMLAttributes::getValueAsInteger - failed to convert attribute '" + attrName + "' with value '" + getValue(attrName) + "' to integer.");
        }

        return val;
    }

    float XMLAttributes::getValueAsFloat(const std::string& attrName, float def) const
    {
        if (!exists(attrName))
        {
            return def;
        }

        float val;
        std::istringstream strm(getValue(attrName).c_str());

        strm >> val;

        // success?
        if (strm.fail())
        {
            throw std::runtime_error("XMLAttributes::getValueAsInteger - failed to convert attribute '" + attrName + "' with value '" + getValue(attrName) + "' to float.");
        }

        return val;
    }

} // End of  CEGUI namespace section
