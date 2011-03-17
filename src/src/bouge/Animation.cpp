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
#include <bouge/Animation.hpp>
#include <bouge/CoreAnimation.hpp>
#include <bouge/Math/Util.hpp>
#include <bouge/Math/TimeFunction.hpp>

namespace bouge {

    Animation::Animation(CoreAnimationPtrC core, float speed, TimeFunction* control, TimeFunction* weight)
        : m_timePercent(0.0f)
        , m_core(core)
        , m_speed(speed)
        , m_control(control)
        , m_weight(weight)
        , m_paused(false)
    {
        if(!m_control)
            m_control = core->preferredControl()->clone();
        if(!m_weight)
            m_weight = new ConstantTF(1.0f);
    }

    Animation::~Animation()
    {
        while(!m_endCbs.empty()) {
            delete m_endCbs.front();
            m_endCbs.pop_front();
        }

        while(!m_updateCbs.empty()) {
            delete m_endCbs.front();
            m_endCbs.pop_front();
        }

        delete m_control;
        delete m_weight;
    }

    std::string Animation::name() const
    {
        return this->core()->name();
    }

    void Animation::update(float deltaTime)
    {
        if(this->paused())
            return;

        if(nearZero(this->duration())) {
            m_timePercent = 0.0f;
        } else {
            m_timePercent += this->speed()*deltaTime/this->duration();
        }

        // Check for callbacks.
        for(std::list<AnimationCallback*>::iterator iCb = m_updateCbs.begin() ; iCb != m_updateCbs.end() ; ++iCb) {
            (*iCb)->operator()(*this);
        }

        if(!m_endCbs.empty() && m_timePercent >= 1.0f) {
            for(std::list<AnimationCallback*>::iterator iCb = m_endCbs.begin() ; iCb != m_endCbs.end() ; ++iCb) {
                (*iCb)->operator()(*this);
                // Once the "end" callbacks have been called, we may safely
                // delete them, as they are no more needed! (There is no rewind)
                delete *iCb;
            }

            m_endCbs.clear();
        }
    }

    Animation& Animation::pause()
    {
        m_paused = true;
        return *this;
    }

    bool Animation::paused() const
    {
        return m_paused;
    }

    Animation& Animation::resume()
    {
        m_paused = false;
        return *this;
    }

    float Animation::speed() const
    {
        return m_speed;
    }

    Animation& Animation::speed(float s)
    {
        m_speed = s;
        return *this;
    }

//     Animation& Animation::addCallback(AnimationCallback* cb, float absoluteTime)
//     {
//         m_arbitraryCbs.insert(std::make_pair(absoluteTime, cb));
//     }

    Animation& Animation::addEndCallback(AnimationCallback* cb)
    {
        m_endCbs.push_back(cb);
        return *this;
    }

    Animation& Animation::addUpdateCallback(AnimationCallback* cb)
    {
        m_updateCbs.push_back(cb);
        return *this;
    }

    float Animation::timePercent() const
    {
        return (*m_control)(m_timePercent);
    }

    float Animation::timePercentRaw() const
    {
        return m_timePercent;
    }

    float Animation::timeAbsolute() const
    {
        return this->timePercent() * this->duration();
    }

    float Animation::timeAbsoluteRaw() const
    {
        return this->timePercentRaw() * this->duration();
    }

    float Animation::duration() const
    {
        return m_core->duration();
    }

    float Animation::weight() const
    {
        return (*m_weight)(m_timePercent);
    }

    bool Animation::hasEnded() const
    {
        return m_timePercent >= 1.0f;
    }

    CoreAnimationPtrC Animation::core() const
    {
        return m_core;
    }

    const TimeFunction* Animation::controlFct() const
    {
        return m_control;
    }

    Animation& Animation::controlFct(TimeFunction* newFun)
    {
        delete m_control;
        m_control = newFun ? newFun : this->core()->preferredControl()->clone();
        return *this;
    }

    const TimeFunction* Animation::weightFct() const
    {
        return m_weight;
    }

    Animation& Animation::weightFct(TimeFunction* newFun)
    {
        delete m_weight;
        m_weight = newFun ? newFun : new ConstantTF(1.0f);
        return *this;
    }

} // namespace bouge
