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
#include <bouge/IOModules/XML/CoreMaterialSet_Handler.hpp>
#include <bouge/IOModules/XMLParserCommon/XMLAttributes.hpp>

#include <bouge/CoreMaterialSet.hpp>

namespace bouge {

    CoreMaterialSet_XMLHandler::CoreMaterialSet_XMLHandler(std::vector<CoreMaterialSetPtr>& materialsToLoad)
        : m_materialsetsToLoad(materialsToLoad)
    { }

    CoreMaterialSet_XMLHandler::~CoreMaterialSet_XMLHandler()
    { }

    bool CoreMaterialSet_XMLHandler::wantsToEnter(const std::string& element)
    {
        static const std::string accepted[] = {"MATSET", "LINK", ""};

        for(const std::string* i = &accepted[0] ; i->length() > 0 ; ++i) {
            if(element == *i)
                return true;
        }

        return false;
    }

    void CoreMaterialSet_XMLHandler::elementStart(const std::string& element, const bouge::XMLAttributes& attributes)
    {
        if(element == "MATSET") {
            this->matsetStart(attributes);
        } else {
            // Just ignore superfluous entries silently.
        }
    }

    void CoreMaterialSet_XMLHandler::elementEnd(const std::string& element)
    {
        if(element == "MATSET") {
            this->matsetEnd();
        } else {
            // Just ignore superfluous entries silently.
        }
    }

    void CoreMaterialSet_XMLHandler::textElement(const std::string& element, const bouge::XMLAttributes& attributes, const std::string& text)
    {
        if(element == "LINK") {
            if(attributes.exists("SUBMESH")) {
                m_materialsetsToLoad.back()->materialForMesh(attributes.getValue("SUBMESH"), text);
            }
        }
    }

    void CoreMaterialSet_XMLHandler::matsetStart(const XMLAttributes& attributes)
    {
        m_materialsetsToLoad.push_back(CoreMaterialSetPtr(new CoreMaterialSet(attributes.getValue("NAME"))));
    }

    void CoreMaterialSet_XMLHandler::matsetEnd()
    {
        // Nothing to do here. It already is in the list.
    }

} // namespace bouge
