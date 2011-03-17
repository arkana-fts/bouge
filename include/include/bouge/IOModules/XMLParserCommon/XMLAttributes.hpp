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
#ifndef BOUGE_XML_ATTRIBUTES_HPP
#define BOUGE_XML_ATTRIBUTES_HPP

// Note: This file is heavily inspired/copied from the CEGUI project.

#include <bouge/Config.hpp>

#include <string>
#include <map>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace bouge
{
    /// Class representing a block of attributes associated with an XML element.
    class BOUGE_API XMLAttributes
    {
    public:
        /// XMLAttributes constructor.
        XMLAttributes();

        /// XMLAttributes Destructor
        virtual ~XMLAttributes();

        /// Adds an attribute to the attribute block. If the attribute value already exists, it is replaced with the new value.
        /// \param attrName String object holding the name of the attribute to be added.
        /// \param attrValue String object holding a string representation of the attribute value.
        void add(const std::string& attrName, const std::string& attrValue);

        /// Removes an attribute from the attribute block.
        /// \param attrName String object holding the name of the attribute to be removed.
        void remove(const std::string& attrName);

        /// Return whether the named attribute exists within the attribute block.
        /// \param attrName String object holding the name of the attribute to be checked.
        /// \return
        ///    - true if an attribute with the name \a attrName is present in the attribute block.
        ///    - false if no attribute named \a attrName is present in the attribute block.
        bool exists(const std::string& attrName) const;

        /// Return the number of attributes in the attribute block.
        /// \return value specifying the number of attributes in this attribute block.
        size_t getCount() const;

        /// Return the value string for attribute \a attrName.
        /// \param attrName String object holding the name of the attribute whose value string is to be returned
        /// \return String object holding the value string for attribute \a attrName.
        /// \exception std::runtime_error thrown if no attribute named \a attrName is present in the attribute block.
        const std::string& getValue(const std::string& attrName) const;

        /// Return the value of attribute \a attrName as a string.
        /// \param attrName String object holding the name of the attribute whose value is to be returned.
        /// \param def
        ///    String object holding the default value to be returned if \a attrName does not exist in the attribute block.
        ///    For some parsers, defaults can be gotten from schemas and such like, though for others this may not be desired
        ///    or possible, so this parameter is used to ensure a default is available in the absence of other mechanisms.
        /// \return String object containing the value of attribute \a attrName if present, or \a def if not.
        const std::string& getValueAsString(const std::string& attrName, const std::string& def = "") const;

        /// Return the value of attribute \a attrName as a boolean value.
        /// \param attrName String object holding the name of the attribute whose value is to be returned.
        /// \param def
        ///    bool value specifying the default value to be returned if \a attrName does not exist in the attribute block.
        ///    For some parsers, defaults can be gotten from schemas and such like, though for others this may not be desired
        ///    or possible, so this parameter is used to ensure a default is available in the absence of other mechanisms.
        /// \return bool value equal to the value of attribute \a attrName if present, or \a def if not.
        /// \exception std::runtime_error thrown if the attribute value string could not be converted to the requested type.
        bool getValueAsBool(const std::string& attrName, bool def = false) const;

        /// Return the value of attribute \a attrName as a integer value.
        /// \param attrName String object holding the name of the attribute whose value is to be returned.
        /// \param def
        ///     integer value specifying the default value to be returned if \a attrName does not exist in the attribute block.
        ///     For some parsers, defaults can be gotten from schemas and such like, though for others this may not be desired
        ///     or possible, so this parameter is used to ensure a default is available in the absence of other mechanisms.
        /// \return integer value equal to the value of attribute \a attrName if present, or \a def if not.
        /// \exception std::runtime_error thrown if the attribute value string could not be converted to the requested type.
        int getValueAsInteger(const std::string& attrName, int def = 0) const;

        /// Return the value of attribute \a attrName as a floating point value.
        /// \param attrName String object holding the name of the attribute whose value is to be returned.
        /// \param def
        ///     float value specifying the default value to be returned if \a attrName does not exist in the attribute block.
        ///     For some parsers, defaults can be gotten from schemas and such like, though for others this may not be desired
        ///     or possible, so this parameter is used to ensure a default is available in the absence of other mechanisms.
        /// \return float value equal to the value of attribute \a attrName if present, or \a def if not.
        /// \exception std::runtime_error thrown if the attribute value string could not be converted to the requested type.
        float getValueAsFloat(const std::string& attrName, float def = 0.0f) const;

    protected:
        typedef std::map<std::string, std::string> AttributeMap;
        AttributeMap m_attrs;
    };

} // namespace bouge


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // BOUGE_XML_ATTRIBUTES_HPP
