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
#include <bouge/IOModules/XMLParserCommon/XMLParserModules/TinyXMLParser.hpp>

#include "bougetinyxml/tinyxml.h"

#include <bouge/IOModules/XMLParserCommon/XMLAttributes.hpp>
#include <bouge/IOModules/XMLParserCommon/XMLHandler.hpp>
#include <stdexcept>

namespace bouge
{

    TinyXMLParser::TinyXMLParser()
        : XMLParser("bouge::TinyXMLParser - Official TinyXML based parser module for bouge")
    {}

    TinyXMLParser::~TinyXMLParser()
    {}

    void TinyXMLParser::parseXMLFile(XMLHandler& handler, const std::string& filename, const std::string& schemaName)
    {
        BougeTinyXML::TiXmlDocument doc(filename);
        if(!doc.LoadFile(filename))
            throw std::runtime_error("Cannot open the XML file " + filename);

        this->processAllElements(doc, handler);
    }

    void TinyXMLParser::parseXMLFileFromMem(XMLHandler& handler, const std::string& xml, const std::string& schema)
    {
        BougeTinyXML::TiXmlDocument doc;
        doc.Parse(xml.c_str());

        this->processAllElements(doc, handler);
    }

    void TinyXMLParser::processAllElements(BougeTinyXML::TiXmlDocument& doc, XMLHandler& handler) {
        for(BougeTinyXML::TiXmlElement* ele = doc.RootElement() ; ele ; ele = ele->NextSiblingElement()) {
            this->processElement(handler, ele);
        }
    }

    void TinyXMLParser::processElement(XMLHandler& handler, const BougeTinyXML::TiXmlElement* element)
    {
        // build attributes block for the element
        XMLAttributes attrs;

        // We might want to completely skip this element.
        if(!handler.wantsToEnter(element->ValueTStr()))
            return;

        for(const BougeTinyXML::TiXmlAttribute *currAttr = element->FirstAttribute() ; currAttr ; currAttr = currAttr->Next()) {
            attrs.add(currAttr->NameTStr(), currAttr->ValueStr());
        }

        // Check for text-only elements (they got only one child, of text type):
        if(!element->NoChildren() && (element->FirstChild() == element->LastChild()) && (element->FirstChild()->Type() == BougeTinyXML::TiXmlNode::TINYXML_TEXT)) {
            handler.textElement(element->ValueTStr(), attrs, element->FirstChild()->ToText()->ValueTStr());
        } else {
            // start element
            handler.elementStart(element->ValueTStr(), attrs);

            // do children
            for(const BougeTinyXML::TiXmlNode* childNode = element->FirstChild() ; childNode ; childNode = childNode->NextSibling()) {
                if(childNode->Type() == BougeTinyXML::TiXmlNode::TINYXML_ELEMENT) {
                    processElement(handler, childNode->ToElement());
                }

                // Silently ignore unhandled node types
            }

            // end element
            handler.elementEnd(element->ValueTStr());
        }
    }

} // namespace bouge
