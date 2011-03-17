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
#include <bouge/IOModules/Cal3dX/CoreSkeleton_Handler.hpp>
#include <bouge/IOModules/XMLParserCommon/XMLAttributes.hpp>

#include <bouge/CoreSkeleton.hpp>
#include <bouge/CoreBone.hpp>
#include <bouge/Math/Vector.hpp>
#include <bouge/Math/Quaternion.hpp>
#include <bouge/Exception.hpp>
#include <bouge/Util.hpp>

#include <sstream>

namespace bouge {

    CoreSkeleton_Cal3dXHandler::CoreSkeleton_Cal3dXHandler(CoreSkeleton& skelToBuild)
        : m_skeletonToBuild(skelToBuild)
        , m_nUnnamedBoneCount(0)
        , m_iCurrBoneId(0)
        , m_iCurrBoneParentId(-1)
    { }

    CoreSkeleton_Cal3dXHandler::~CoreSkeleton_Cal3dXHandler()
    { }

    bool CoreSkeleton_Cal3dXHandler::wantsToEnter(const std::string& element)
    {
        static const std::string accepted[] = {"SKELETON", "BONE", "TRANSLATION", "ROTATION", "PARENTID", ""};

        for(const std::string* i = &accepted[0] ; i->length() > 0 ; ++i) {
            if(element == *i)
                return true;
        }

        return false;
    }

    void CoreSkeleton_Cal3dXHandler::elementStart(const std::string& element, const XMLAttributes& attributes)
    {
        if(element == "SKELETON") {
            this->skeletonStart(attributes);
        } else if(element == "BONE") {
            this->boneStart(attributes);
        } else {
            // Just ignore superfluous entries silently.
        }
    }

    void CoreSkeleton_Cal3dXHandler::elementEnd(const std::string& element)
    {
        if(element == "SKELETON") {
            this->skeletonEnd();
        } else if(element == "BONE") {
            this->boneEnd();
        } else {
            // Just ignore superfluous entries silently.
        }
    }

    void CoreSkeleton_Cal3dXHandler::textElement(const std::string& element, const XMLAttributes& attributes, const std::string& text)
    {
        std::stringstream ssText(text);

        if(element == "TRANSLATION") {

            ssText >> m_vCurrBoneRelativePosition[0]
                   >> m_vCurrBoneRelativePosition[1]
                   >> m_vCurrBoneRelativePosition[2];

        } else if(element == "ROTATION") {

            ssText >> m_qCurrBoneRelativeRotation[0]
                   >> m_qCurrBoneRelativeRotation[1]
                   >> m_qCurrBoneRelativeRotation[2]
                   >> m_qCurrBoneRelativeRotation[3];

            // NOTE: we need to invert the rotation quaternion, because Cal3d
            //       seems to use a left-handed coordinate system à la DirectX.
            m_qCurrBoneRelativeRotation = m_qCurrBoneRelativeRotation.inv();

        } else if(element == "PARENTID") {

            ssText >> m_iCurrBoneParentId;

        }

    }

    void CoreSkeleton_Cal3dXHandler::skeletonStart(const XMLAttributes& attributes)
    {
        // Nothing special to do, we only support one skeleton per file for now.
    }

    void CoreSkeleton_Cal3dXHandler::skeletonEnd()
    {
        std::vector<CoreBonePtr> rootBones;

        // Create bone family hierarchy.
        for(std::map<int, std::pair<CoreBonePtr, int> >::iterator i = m_bonesAndParents.begin() ; i != m_bonesAndParents.end() ; ++i) {
            int iBoneId = i->first;
            int iBoneParentId = i->second.second;
            CoreBonePtr pBone = i->second.first;

            // Bones with no parents need to be added as root bones.
            // We do this later, as they might still get child bones attached
            // but the skeleton requires a full hierarchy to be added.
            if(iBoneParentId < 0) {
                rootBones.push_back(pBone);
                continue;
            }

            // Error checking
            if(m_bonesAndParents.find(iBoneParentId) == m_bonesAndParents.end()) {
                throw BadDataException("Bone " + pBone->name() + " (ID: " + to_s(iBoneId) + ") has parent with ID " + to_s(iBoneParentId) + " but there is no bone with that ID", __FILE__, __LINE__);
            }

            CoreBonePtr pParent = m_bonesAndParents[iBoneParentId].first;
            pParent->addChild(pBone);
            pBone->parent(pParent);
        }

        // Now, go through all bones and estimate their length.
        for(std::map<int, std::pair<CoreBonePtr, int> >::iterator i = m_bonesAndParents.begin() ; i != m_bonesAndParents.end() ; ++i) {
            CoreBonePtr pBone = i->second.first;

            float fLength = -1.0f;
            for(CoreBone::iterator i = pBone->begin() ; i != pBone->end() ; ++i) {
                fLength = std::max(fLength, i->relativeRootPosition().len());
            }

            pBone->length(fLength <= 0.0f ? 1.0f : fLength);
        }

        // We need a second walk through for the lengths of those without children.
        // We cannot integrate it in the loop above because there a child may get
        // updated before its parent!
        for(std::map<int, std::pair<CoreBonePtr, int> >::iterator i = m_bonesAndParents.begin() ; i != m_bonesAndParents.end() ; ++i) {
            CoreBonePtr pBone = i->second.first;
            if(pBone->childCount() < 1) {
                // No children there? Give it the same length as the parent.
                if(pBone->hasParent()) {
                    pBone->length(pBone->parent()->length());
                } else {
                    // No parent either? Default to 1.0
                    pBone->length(1.0f);
                }
            }
        }

        // Now, we can add all root bones to the skeleton.
        for(std::vector<CoreBonePtr>::iterator iRootBone = rootBones.begin() ; iRootBone != rootBones.end() ; ++iRootBone) {
            m_skeletonToBuild.addRootBone(*iRootBone);
        }
    }

    void CoreSkeleton_Cal3dXHandler::boneStart(const XMLAttributes& attributes)
    {
        if(attributes.exists("NAME")) {
            m_sCurrBoneName = attributes.getValue("NAME");
        } else {
            m_sCurrBoneName = "Unnamed" + to_s(m_nUnnamedBoneCount);
            m_nUnnamedBoneCount++;
        }

        if(attributes.exists("ID")) {
            m_iCurrBoneId = attributes.getValueAsInteger("ID");
            if(m_iCurrBoneId < 0) {
                throw BadDataException("Bone " + m_sCurrBoneName + " has negative id", __FILE__, __LINE__);
            }
        } else {
            throw BadDataException("Bone " + m_sCurrBoneName + " has no id", __FILE__, __LINE__);
        }
    }

    void CoreSkeleton_Cal3dXHandler::boneEnd()
    {
        CoreBonePtr pNewBone = CoreBonePtr(new CoreBone(m_sCurrBoneName, m_vCurrBoneRelativePosition, m_qCurrBoneRelativeRotation, -1.0f));
        pNewBone->userData = UserDataPtr(new Cal3dXBoneIdUD(m_iCurrBoneId));
        m_bonesAndParents[m_iCurrBoneId] = std::make_pair(pNewBone, m_iCurrBoneParentId);
    }

} // namespace bouge
