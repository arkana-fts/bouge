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

#ifndef BOUGE_XML_PARSER_HPP
#define BOUGE_XML_PARSER_HPP

// Note: This file is heavily inspired/copied from the CEGUI project.

#include <bouge/Config.hpp>
#include <string>

namespace bouge
{
    class XMLHandler;

    /// This is an abstract class that is used by Bouge to interface with XML parser libraries.
    class BOUGE_API XMLParser
    {
    public:
        /// XMLParser base class destructor.
        virtual ~XMLParser();

        /// abstract method which initiates parsing of an XML file.
        /// \param handler XMLHandler based object which will process the XML elements.
        /// \param filename String object holding the filename of the XML file to be parsed.
        /// \param schemaName String object holding the name of the XML schema file to use for validating the XML.
        ///                   Note that whether this is used or not is dependant upon the XMLParser in use.
        /// \exception std::runtime_error In case the file can't be opened.
        /// \exception std::exception Any exception thrown from the XML handler.
        virtual void parseXMLFile(XMLHandler& handler, const std::string& filename, const std::string& schemaName) = 0;

        /// abstract method which initiates parsing of an XML string from memory.
        /// \param handler XMLHandler based object which will process the XML elements.
        /// \param file String object holding the XML to be parsed.
        /// \param schema String object holding the XML schema to use for validating the XML.
        ///               Note that whether this is used or not is dependant upon the XMLParser in use.
        virtual void parseXMLFileFromMem(XMLHandler& handler, const std::string& xml, const std::string& schema) = 0;

        /// Return identification string for the XML parser module. If the internal id string has not been
        /// set by the XML parser module creator, a generic string of "Unknown XML parser" will be returned.
        /// \return String object holding a string that identifies the XML parser in use.
        const std::string& getIdentifierString() const;

    protected:
        /// XMLParser base class constructor.
        XMLParser(const std::string& identifierString);

        std::string m_identifierString; //!< String that holds some id information about the module.
    };

} // namespace Bouge


#endif  // BOUGE_XML_PARSER_HPP
