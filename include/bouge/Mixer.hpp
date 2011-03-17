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
#ifndef BOUGE_MIXER_HPP
#define BOUGE_MIXER_HPP

#include <bouge/bougefwd.hpp>
#include <bouge/Animation.hpp>

#include <set>
#include <string>

namespace bouge {

    class BOUGE_API Mixer
    {
    public:
        virtual AnimationPtr play(AnimationPtr anim) = 0;
        virtual AnimationPtr oneshot(AnimationPtr anim) = 0;
        virtual Mixer& stop(AnimationPtr anim, float fadeOutTime = 0.0f) = 0;
        virtual Mixer& stop(const std::string& animName, float fadeOutTime = 0.0f) = 0;

        virtual Mixer& pauseAll() = 0;
        virtual Mixer& resumeAll() = 0;
        virtual Mixer& stopAll(float fadeOutTime = 0.0f) = 0;

        float speed() const;
        Mixer& speed(float speed);
        virtual Mixer& speed(const std::string& animName, float speed) = 0;

        virtual void update(float deltaTime) = 0;

        BOUGE_USER_DATA;

    protected:
        Mixer();
        virtual ~Mixer();

        virtual void onSpeedChanged(float oldspeed, float newspeed);

    private:
        float m_speed;
    };

    class BOUGE_API DefaultMixer : public Mixer
    {
    public:
        DefaultMixer(SkeletonInstancePtr skel);
        virtual ~DefaultMixer();

        AnimationPtr play(AnimationPtr anim);
        AnimationPtr oneshot(AnimationPtr anim);
        DefaultMixer& stop(AnimationPtr anim, float fadeOutTime = 0.0f);
        DefaultMixer& stop(const std::string& animName, float fadeOutTime = 0.0f);

        DefaultMixer& pauseAll();
        DefaultMixer& resumeAll();
        DefaultMixer& stopAll(float fadeOutTime = 0.0f);

        DefaultMixer& speed(const std::string& animName, float speed);

        virtual void update(float deltaTime);

    protected:
        SkeletonInstancePtr m_skel;

        std::set<AnimationPtr> m_cyclingAnims;
        std::list<AnimationPtr> m_oneshotAnims;
        std::set<AnimationPtr> m_scheduledStop;

        void doScheduledStops();
    };

} // namespace bouge

#endif // BOUGE_MIXER_HPP
