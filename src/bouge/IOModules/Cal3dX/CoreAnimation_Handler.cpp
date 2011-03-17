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
#include <bouge/IOModules/Cal3dX/CoreAnimation_Handler.hpp>
#include <bouge/IOModules/Cal3dX/CoreSkeleton_Handler.hpp>
#include <bouge/IOModules/XMLParserCommon/XMLAttributes.hpp>

#include <bouge/Math/TimeFunction.hpp>
#include <bouge/CoreAnimation.hpp>
#include <bouge/CoreKeyframe.hpp>
#include <bouge/CoreTrack.hpp>
#include <bouge/CoreSkeleton.hpp>
#include <bouge/CoreBone.hpp>

#include <sstream>

namespace bouge {

    CoreAnimation_Cal3dXHandler::CoreAnimation_Cal3dXHandler(std::vector<CoreAnimationPtr>& animsToLoad, CoreSkeletonPtr skelToUse, std::string animName)
        : m_skelToUse(skelToUse)
        , m_animName(animName)
        , m_animsToLoad(animsToLoad)
    { }

    CoreAnimation_Cal3dXHandler::~CoreAnimation_Cal3dXHandler()
    { }

    bool CoreAnimation_Cal3dXHandler::wantsToEnter(const std::string& element)
    {
        static const std::string accepted[] = {"ANIMATION", "TRACK", "KEYFRAME", "TRANSLATION", "ROTATION", ""};

        for(const std::string* i = &accepted[0] ; i->length() > 0 ; ++i) {
            if(element == *i)
                return true;
        }

        return false;
    }

    void CoreAnimation_Cal3dXHandler::elementStart(const std::string& element, const bouge::XMLAttributes& attributes)
    {
        if(element == "ANIMATION") {
            this->animationStart(attributes);
        } else if(element == "TRACK") {
            this->trackStart(attributes);
        } else if(element == "KEYFRAME") {
            this->keyframeStart(attributes);
        } else {
            // Just ignore superfluous entries silently.
        }
    }

    void CoreAnimation_Cal3dXHandler::elementEnd(const std::string& element)
    {
        if(element == "ANIMATION") {
            this->animationEnd();
        } else if(element == "TRACK") {
            this->trackEnd();
        } else if(element == "KEYFRAME") {
            this->keyframeEnd();
        } else {
            // Just ignore superfluous entries silently.
        }
    }

    void CoreAnimation_Cal3dXHandler::textElement(const std::string& element, const bouge::XMLAttributes& attributes, const std::string& text)
    {
        std::stringstream ssText(text);

        if(element == "TRANSLATION") {
            ssText >> m_lastTranslation;

            // Cal3D stores the animation data in "absolute", that is not
            // relative to the rest pose. Convert it to relative.
            m_lastTranslation -= m_skelToUse->bone(m_currTrackName)->relativeRootPosition();

            m_currKeyframe->translation(m_lastTranslation);
        } else if(element == "ROTATION") {
            Quaternion q;
            ssText >> q;

            // NOTE: we need to invert the rotation quaternion, because Cal3d
            //       seems to use a left-handed coordinate system à la DirectX.
            q = q.inv();

            // Same as for the translations, need to convert absolute to relative.
            q /= m_skelToUse->bone(m_currTrackName)->relativeBoneRotation();

            m_currKeyframe->rotation(q);
        }
    }

    void CoreAnimation_Cal3dXHandler::animationStart(const XMLAttributes& attributes)
    {
        m_animsToLoad.push_back(CoreAnimationPtr(new CoreAnimation(m_animName, new RepeatTF(new LinearTF(1.0f)))));
    }

    void CoreAnimation_Cal3dXHandler::animationEnd()
    {
        // Nothing to do here. It already is in the list.
    }

    void CoreAnimation_Cal3dXHandler::trackStart(const XMLAttributes& attributes)
    {
        // Cal3D expects repeat by default.
        m_currTrack = CoreTrackPtr(new CoreTrack());

        int boneIdNeeded = attributes.getValueAsInteger("BONEID");

        // Find out which bone it belongs to.
        for(CoreSkeleton::iterator iBone = m_skelToUse->begin() ; iBone != m_skelToUse->end() ; ++iBone) {
            Cal3dXBoneIdUD* pBoneId = dynamic_cast<Cal3dXBoneIdUD*>(iBone->userData.get());
            if(pBoneId && pBoneId->id == boneIdNeeded) {
                m_currTrackName = iBone->name();
            }
        }
    }

    void CoreAnimation_Cal3dXHandler::trackEnd()
    {
        // Add the track to the animation's tracklist.
        m_animsToLoad.back()->track(m_currTrackName, m_currTrack);

        // Reset that, as they only keep it for one track.
        m_lastTranslation = Vector();
    }

    void CoreAnimation_Cal3dXHandler::keyframeStart(const XMLAttributes& attributes)
    {
        m_currKeyframe = CoreKeyframePtr(new CoreKeyframe());
        m_currKeyframeTime = attributes.getValueAsFloat("TIME");
    }

    void CoreAnimation_Cal3dXHandler::keyframeEnd()
    {
        // Cal3D has this "optimization" of leaving the translation values out
        // if no translation is animated, good idea, but strangely implemented.
        // So here, we use the "last available" translation in case this
        // keyframe doesn't have one set.
        if(!m_currKeyframe->hasTranslation() && m_lastTranslation != Vector()) {
            m_currKeyframe->translation(m_lastTranslation);
        }

        // Add the keyframe to the current track.
        m_currTrack->add(m_currKeyframeTime, m_currKeyframe);
    }

} // namespace bouge
