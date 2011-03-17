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
#include <bouge/IOModules/Cal3dX/CoreMaterial_Handler.hpp>
#include <bouge/IOModules/XMLParserCommon/XMLAttributes.hpp>

#include <bouge/CoreMaterial.hpp>
#include <bouge/Util.hpp>

namespace bouge {

    CoreMaterial_Cal3dXHandler::CoreMaterial_Cal3dXHandler(std::vector<CoreMaterialPtr>& materialsToLoad, std::string name)
        : m_materialsToLoad(materialsToLoad)
        , m_name(name)
        , m_iCurrMapId(0)
    { }

    CoreMaterial_Cal3dXHandler::~CoreMaterial_Cal3dXHandler()
    { }

    bool CoreMaterial_Cal3dXHandler::wantsToEnter(const std::string& element)
    {
        static const std::string accepted[] = {"MATERIAL", "AMBIENT", "DIFFUSE", "SPECULAR", "SHININESS", "MAP", ""};

        for(const std::string* i = &accepted[0] ; i->length() > 0 ; ++i) {
            if(element == *i)
                return true;
        }

        return false;
    }

    void CoreMaterial_Cal3dXHandler::elementStart(const std::string& element, const bouge::XMLAttributes& attributes)
    {
        if(element == "MATERIAL") {
            this->materialStart(attributes);
        } else {
            // Just ignore superfluous entries silently.
        }
    }

    void CoreMaterial_Cal3dXHandler::elementEnd(const std::string& element)
    {
        if(element == "MATERIAL") {
            this->materialEnd();
        } else {
            // Just ignore superfluous entries silently.
        }
    }

    static std::string cal3d2bouge_color(const std::string& cal)
    {
        std::vector<float> col = to< std::vector<float> >(cal);
        for(std::size_t i = 0 ; i < col.size() ; ++i) {
            col[i] /= 255.0f;
        }

        return to_s(col);
    }

    void CoreMaterial_Cal3dXHandler::textElement(const std::string& element, const bouge::XMLAttributes& attributes, const std::string& text)
    {
        if(element == "AMBIENT") {
            m_materialsToLoad.back()->property("ambient", cal3d2bouge_color(text));
        } else if(element == "DIFFUSE") {
            m_materialsToLoad.back()->property("diffuse", cal3d2bouge_color(text));
        } else if(element == "SPECULAR") {
            m_materialsToLoad.back()->property("specular", cal3d2bouge_color(text));
        } else if(element == "SHININESS") {
            m_materialsToLoad.back()->property("shininess", text);
        } else if(element == "MAP") {
            std::string type = ansitolower(attributes.getValueAsString("TYPE"));
            if(type.find("diffuse") != std::string::npos) {
                m_materialsToLoad.back()->property("diffusemap", text);
            } else if(type.find("normal") != std::string::npos) {
                m_materialsToLoad.back()->property("normalmap", text);
            } else if(type.find("specular") != std::string::npos) {
                m_materialsToLoad.back()->property("specularmap", text);
            } else {
                if(m_iCurrMapId == 0) {
                    m_materialsToLoad.back()->property("map", text);
                } else {
                    m_materialsToLoad.back()->property("map" + to_s(m_iCurrMapId), text);
                }
                m_iCurrMapId++;
            }
        }
    }

    void CoreMaterial_Cal3dXHandler::materialStart(const XMLAttributes& attributes)
    {
        m_materialsToLoad.push_back(CoreMaterialPtr(new CoreMaterial(m_name)));
    }

    void CoreMaterial_Cal3dXHandler::materialEnd()
    {
        // Nothing to do here. It already is in the list.
    }

} // namespace bouge
