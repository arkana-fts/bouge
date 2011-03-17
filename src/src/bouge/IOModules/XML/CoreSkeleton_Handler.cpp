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
#include <bouge/IOModules/XML/CoreSkeleton_Handler.hpp>
#include <bouge/IOModules/XMLParserCommon/XMLAttributes.hpp>

#include <bouge/CoreSkeleton.hpp>
#include <bouge/CoreBone.hpp>
#include <bouge/Math/Vector.hpp>
#include <bouge/Math/Quaternion.hpp>

#include <sstream>

namespace bouge {

    CoreSkeleton_XMLHandler::CoreSkeleton_XMLHandler(CoreSkeleton& skelToBuild)
        : m_skeletonToBuild(skelToBuild)
        , m_currBone()
    { }

    CoreSkeleton_XMLHandler::~CoreSkeleton_XMLHandler()
    { }

    bool CoreSkeleton_XMLHandler::wantsToEnter(const std::string& element)
    {
        static const std::string accepted[] = {"SKELETON", "BONE", ""};

        for(const std::string* i = &accepted[0] ; i->length() > 0 ; ++i) {
            if(element == *i)
                return true;
        }

        return false;
    }

    void CoreSkeleton_XMLHandler::elementStart(const std::string& element, const XMLAttributes& attributes)
    {
        if(element == "SKELETON") {
            this->skeletonStart(attributes);
        } else if(element == "BONE") {
            this->boneStart(attributes);
        } else {
            // Just ignore superfluous entries silently.
        }
    }

    void CoreSkeleton_XMLHandler::elementEnd(const std::string& element)
    {
        if(element == "SKELETON") {
            this->skeletonEnd();
        } else if(element == "BONE") {
            this->boneEnd();
        } else {
            // Just ignore superfluous entries silently.
        }
    }

    void CoreSkeleton_XMLHandler::textElement(const std::string& element, const XMLAttributes& attributes, const std::string& text)
    {
    }

    void CoreSkeleton_XMLHandler::skeletonStart(const XMLAttributes& attributes)
    {
        // We can get the name of the skeleton here.
        if(attributes.exists("NAME"))
            m_skeletonToBuild.name(attributes.getValue("NAME"));
    }

    void CoreSkeleton_XMLHandler::skeletonEnd()
    {
        // Nothing special to do, we only support one skeleton per file for now.
    }

    void CoreSkeleton_XMLHandler::boneStart(const XMLAttributes& attributes)
    {
        std::string sName = attributes.getValue("NAME");
        std::stringstream ssRelativePosition(attributes.getValue("RELPOSITION"));
        std::stringstream ssRelativeRotation(attributes.getValue("RELROTATION"));

        Vector vRelativePosition;
        Quaternion qRelativeRotation;
        ssRelativePosition >> vRelativePosition;
        ssRelativeRotation >> qRelativeRotation;

        float fLength = 1.0f;
        if(attributes.exists("LENGTH"))
            std::stringstream(attributes.getValue("LENGTH")) >> fLength;

        CoreBonePtr parentBone = m_currBone;
        CoreBonePtr newBone = CoreBonePtr(new CoreBone(sName, vRelativePosition, qRelativeRotation, fLength));

        if(parentBone) {
            parentBone->addChild(newBone);
            newBone->parent(parentBone);
        }

        m_currBone = newBone;
    }

    void CoreSkeleton_XMLHandler::boneEnd()
    {
        // Either, we are at the end of a child bone, ...
        if(m_currBone->hasParent()) {
            // Then just update to continue with the parent.
            m_currBone = m_currBone->parent();
        } else {
            // Or at the end of a root bone, add it to the skeleton.
            m_skeletonToBuild.addRootBone(m_currBone);
            m_currBone.reset();
        }
    }

} // namespace bouge
