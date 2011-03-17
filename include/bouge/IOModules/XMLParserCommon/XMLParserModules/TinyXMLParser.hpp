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

#ifndef BOUGE_TINY_XML_PARSER_HPP
#define BOUGE_TINY_XML_PARSER_HPP

// Note: This file is heavily inspired/copied from the CEGUI project.

#include <bouge/IOModules/XMLParserCommon/XMLParser.hpp>

namespace BougeTinyXML {
    class TiXmlElement;
    class TiXmlDocument;
}

namespace bouge
{
    /// Implementation of XMLParser using TinyXML
    class BOUGE_API TinyXMLParser : public XMLParser
    {
    public:
        TinyXMLParser();
        ~TinyXMLParser();

        // Implementation of public abstract interface
        void parseXMLFile(XMLHandler& handler, const std::string& filename, const std::string& schemaName);
        virtual void parseXMLFileFromMem(XMLHandler& handler, const std::string& xml, const std::string& schema);

    protected:
        void processAllElements(BougeTinyXML::TiXmlDocument& doc, XMLHandler& handler);
        void processElement(XMLHandler& handler, const BougeTinyXML::TiXmlElement* element);
    };

} // End of  CEGUI namespace section


#endif // BOUGE_TINY_XML_PARSER_HPP
