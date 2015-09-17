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
#ifndef BOUGE_ANIMATION_HPP
#define BOUGE_ANIMATION_HPP

#include <bouge/bougefwd.hpp>
#include <list>

namespace bouge {

    class BOUGE_API AnimationCallback
    {
    public:
        virtual void operator()(const Animation& anim) = 0;
        virtual ~AnimationCallback() {};
    };

    class BOUGE_API Animation
    {
    public:
        Animation(CoreAnimationPtrC core, float speed = 1.0f, TimeFunction* control = 0, TimeFunction* weight = 0);
        virtual ~Animation();

        BOUGE_USER_DATA;

        std::string name() const;

        void update(float deltaTime);

        Animation& pause();
        bool paused() const;
        Animation& resume();

        float speed() const;
        Animation& speed(float s);

//         Animation& addCallback(AnimationCallback* cb, float absoluteTime);
        Animation& addEndCallback(AnimationCallback* cb);
        Animation& addUpdateCallback(AnimationCallback* cb);

        float timePercent() const;
        float timePercentRaw() const;
        float timeAbsolute() const;
        float timeAbsoluteRaw() const;
        float duration() const;

        float weight() const;

        bool hasEnded() const;

        CoreAnimationPtrC core() const;

        const TimeFunction* controlFct() const;
        Animation& controlFct(TimeFunction* newFun);
        const TimeFunction* weightFct() const;
        Animation& weightFct(TimeFunction* newFun);

    private:
        float m_timePercent;

        CoreAnimationPtrC m_core;
        float m_speed;
        TimeFunction* m_control;
        TimeFunction* m_weight;
        bool m_paused;

//         std::multimap<float, AnimationCallback*> m_arbitraryCbs;
        std::list<AnimationCallback*> m_updateCbs;
        std::list<AnimationCallback*> m_endCbs;
    };

} // namespace bouge

#endif // BOUGE_ANIMATION_HPP
