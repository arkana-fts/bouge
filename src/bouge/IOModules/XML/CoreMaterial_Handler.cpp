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
#include <bouge/IOModules/XML/CoreMaterial_Handler.hpp>
#include <bouge/IOModules/XMLParserCommon/XMLAttributes.hpp>

#include <bouge/CoreMaterial.hpp>

namespace bouge {

    CoreMaterial_XMLHandler::CoreMaterial_XMLHandler(std::vector<CoreMaterialPtr>& materialsToLoad)
        : m_materialsToLoad(materialsToLoad)
    { }

    CoreMaterial_XMLHandler::~CoreMaterial_XMLHandler()
    { }

    bool CoreMaterial_XMLHandler::wantsToEnter(const std::string& element)
    {
        static const std::string accepted[] = {"MATERIAL", "PROPERTY", ""};

        for(const std::string* i = &accepted[0] ; i->length() > 0 ; ++i) {
            if(element == *i)
                return true;
        }

        return false;
    }

    void CoreMaterial_XMLHandler::elementStart(const std::string& element, const bouge::XMLAttributes& attributes)
    {
        if(element == "MATERIAL") {
            this->materialStart(attributes);
        } else {
            // Just ignore superfluous entries silently.
        }
    }

    void CoreMaterial_XMLHandler::elementEnd(const std::string& element)
    {
        if(element == "MATERIAL") {
            this->materialEnd();
        } else {
            // Just ignore superfluous entries silently.
        }
    }

    void CoreMaterial_XMLHandler::textElement(const std::string& element, const bouge::XMLAttributes& attributes, const std::string& text)
    {
        if(element == "PROPERTY") {
            if(attributes.exists("NAME")) {
                m_materialsToLoad.back()->proprety(attributes.getValue("NAME"), text);
            }
        }
    }

    void CoreMaterial_XMLHandler::materialStart(const XMLAttributes& attributes)
    {
        m_materialsToLoad.push_back(CoreMaterialPtr(new CoreMaterial(attributes.getValue("NAME"))));
    }

    void CoreMaterial_XMLHandler::materialEnd()
    {
        // Nothing to do here. It already is in the list.
    }

} // namespace bouge
