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
#include <bouge/IOModules/XML/CoreAnimation_Handler.hpp>
#include <bouge/IOModules/XMLParserCommon/XMLAttributes.hpp>

#include <bouge/Math/TimeFunction.hpp>
#include <bouge/CoreAnimation.hpp>
#include <bouge/CoreTrack.hpp>
#include <bouge/CoreKeyframe.hpp>

#include <sstream>

namespace bouge {

    CoreAnimation_XMLHandler::CoreAnimation_XMLHandler(std::vector<CoreAnimationPtr>& animsToLoad)
        : m_animsToLoad(animsToLoad)
    { }

    CoreAnimation_XMLHandler::~CoreAnimation_XMLHandler()
    { }

    bool CoreAnimation_XMLHandler::wantsToEnter(const std::string& element)
    {
        static const std::string accepted[] = {"ANIMATION", "TRACK", "KEYFRAME", "TRANSLATION", "ROTATION", "SCALE", ""};

        for(const std::string* i = &accepted[0] ; i->length() > 0 ; ++i) {
            if(element == *i)
                return true;
        }

        return false;
    }

    void CoreAnimation_XMLHandler::elementStart(const std::string& element, const bouge::XMLAttributes& attributes)
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

    void CoreAnimation_XMLHandler::elementEnd(const std::string& element)
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

    void CoreAnimation_XMLHandler::textElement(const std::string& element, const bouge::XMLAttributes& attributes, const std::string& text)
    {
        std::stringstream ssText(text);

        if(element == "TRANSLATION") {
            Vector t;
            ssText >> t;
            m_currKeyframe->translation(t);
        } else if(element == "ROTATION") {
            Quaternion q;
            ssText >> q;
            m_currKeyframe->rotation(q);
        } else if(element == "SCALE") {
            Vector s;
            ssText >> s;
            m_currKeyframe->scale(s);
        }
    }

    void CoreAnimation_XMLHandler::animationStart(const XMLAttributes& attributes)
    {
        TimeFunction* control = 0;

        if(attributes.exists("ENDCONTROL")) {
            std::string sEnd = attributes.getValue("ENDCONTROL");
            if(sEnd == "continue") {
                control = new LinearTF(1.0f);
            } else if(sEnd == "repeat") {
                control = new RepeatTF(new LinearTF(1.0f));
            } else if(sEnd == "cycle") {
                control = new CycleTF(new LinearTF(1.0f));
            } else if(sEnd == "hold") {
                control = new HoldTF(new LinearTF(1.0f));
            } else if(sEnd == "reset") {
                control = new HoldTF(new LinearTF(1.0f), 0.0f);
            }
        }

        m_animsToLoad.push_back(CoreAnimationPtr(new CoreAnimation(attributes.getValue("NAME"), control)));
    }

    void CoreAnimation_XMLHandler::animationEnd()
    {
        // Nothing to do here. It already is in the list.
    }

    void CoreAnimation_XMLHandler::trackStart(const XMLAttributes& attributes)
    {
        m_currTrack = CoreTrackPtr(new CoreTrack());
        m_currTrackName = attributes.getValue("BONE");
    }

    void CoreAnimation_XMLHandler::trackEnd()
    {
        // Add the track to the animation's tracklist.
        m_animsToLoad.back()->track(m_currTrackName, m_currTrack);
    }

    void CoreAnimation_XMLHandler::keyframeStart(const XMLAttributes& attributes)
    {
        m_currKeyframe = CoreKeyframePtr(new CoreKeyframe());
        m_currKeyframeTime = attributes.getValueAsFloat("TIME");
    }

    void CoreAnimation_XMLHandler::keyframeEnd()
    {
        // Add the keyframe to the current track.
        m_currTrack->add(m_currKeyframeTime, m_currKeyframe);
    }

} // namespace bouge
