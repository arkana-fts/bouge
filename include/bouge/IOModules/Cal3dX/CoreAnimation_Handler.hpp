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
#ifndef BOUGE_COREANIMATION_CAL3DXHANDLER_HPP
#define BOUGE_COREANIMATION_CAL3DXHANDLER_HPP

#include <bouge/IOModules/XMLParserCommon/XMLHandler.hpp>
#include <bouge/bougefwd.hpp>
#include <bouge/Math/Vector.hpp>

#include <vector>

namespace bouge {

    class CoreAnimation_Cal3dXHandler : public XMLHandler
    {
    public:
        CoreAnimation_Cal3dXHandler(std::vector<CoreAnimationPtr>& animsToLoad, CoreSkeletonPtr skelToUse, std::string animName);
        virtual ~CoreAnimation_Cal3dXHandler();

        virtual void elementStart(const std::string& element, const XMLAttributes& attributes);
        virtual void elementEnd(const std::string& element);
        virtual void textElement(const std::string& element, const XMLAttributes& attributes, const std::string& text);
        virtual bool wantsToEnter(const std::string& element);

    protected:
        void animationStart(const XMLAttributes& attributes);
        void animationEnd();

        void trackStart(const XMLAttributes& attributes);
        void trackEnd();

        void keyframeStart(const XMLAttributes& attributes);
        void keyframeEnd();

    private:
        CoreSkeletonPtr m_skelToUse;
        std::string m_animName;
        Vector m_lastTranslation;

        std::vector<CoreAnimationPtr>& m_animsToLoad;

        CoreTrackPtr m_currTrack;
        std::string m_currTrackName;
        CoreKeyframePtr m_currKeyframe;
        float m_currKeyframeTime;
    };

} // namespace bouge

#endif // BOUGE_COREANIMATION_CAL3DXHANDLER_HPP
