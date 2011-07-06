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
#include <bouge/Mixer.hpp>
#include <bouge/Animation.hpp>
#include <bouge/SkeletonInstance.hpp>
#include <bouge/CoreAnimation.hpp>
#include <bouge/CoreTrack.hpp>
#include <bouge/BoneInstance.hpp>
#include <bouge/Math/Util.hpp>
#include <bouge/Math/TimeFunction.hpp>

namespace bouge {

    Mixer::Mixer()
        : m_speed(1.0f)
        , m_paused(false)
    { }

    Mixer::~Mixer()
    { }

    Mixer& Mixer::pauseAll()
    {
        m_paused = true;
        return *this;
    }

    Mixer& Mixer::resumeAll()
    {
        m_paused = false;
        return *this;
    }

    bool Mixer::paused() const
    {
        return m_paused;
    }

    float Mixer::speed() const
    {
        return m_speed;
    }

    Mixer& Mixer::speed(float speed)
    {
        float oldSpeed = m_speed;
        m_speed = speed;
        this->onSpeedChanged(oldSpeed, speed);
        return *this;
    }

    void Mixer::onSpeedChanged(float, float)
    { }

    AnimationPtr DummyMixer::play(AnimationPtr anim)
    {
        return AnimationPtr(new Animation(CoreAnimationPtrC(new CoreAnimation("Dummy"))));
    }

    AnimationPtr DummyMixer::oneshot(AnimationPtr anim)
    {
        return AnimationPtr(new Animation(CoreAnimationPtrC(new CoreAnimation("Dummy"))));
    }

    DefaultMixer::DefaultMixer(SkeletonInstancePtr skel)
        : m_skel(skel)
    { }

    DefaultMixer::~DefaultMixer()
    { }

    AnimationPtr DefaultMixer::play(AnimationPtr anim)
    {
        m_cyclingAnims.insert(anim);
        return anim;
    }

    AnimationPtr DefaultMixer::oneshot(AnimationPtr anim)
    {
        m_oneshotAnims.push_front(anim);
        return anim;
    }

    DefaultMixer& DefaultMixer::stop(AnimationPtr anim, float fadeOutTime)
    {
        // TODO: Verify this one!
        float normalizedFadeOutDuration = fadeOutTime/anim->duration();
        float totalWantedDurationPercent = anim->timePercentRaw() + normalizedFadeOutDuration;
        anim->weightFct(new FadeOutTF(totalWantedDurationPercent - normalizedFadeOutDuration, totalWantedDurationPercent, anim->weightFct()->clone()));

        // We need to keep track of this animation. As soon as its weight
        // reaches 0 or below, we can remove it.
        m_scheduledStop.insert(anim);
        return *this;
    }

    DefaultMixer& DefaultMixer::stop(const std::string& animName, float fadeOutTime)
    {
        for(std::set<AnimationPtr>::iterator i = m_cyclingAnims.begin() ; i != m_cyclingAnims.end() ; ++i) {
            if((*i)->name() == animName) {
                this->stop(*i, fadeOutTime);
            }
        }
        for(std::list<AnimationPtr>::iterator i = m_oneshotAnims.begin() ; i != m_oneshotAnims.end() ; ++i) {
            if((*i)->name() == animName) {
                this->stop(*i, fadeOutTime);
            }
        }
        return *this;
    }

    DefaultMixer& DefaultMixer::pause(const std::string& animName)
    {
        for(std::set<AnimationPtr>::iterator i = m_cyclingAnims.begin() ; i != m_cyclingAnims.end() ; ++i) {
            if((*i)->name() == animName) {
                (*i)->pause();
                break;
            }
        }
        for(std::list<AnimationPtr>::iterator i = m_oneshotAnims.begin() ; i != m_oneshotAnims.end() ; ++i) {
            if((*i)->name() == animName) {
                (*i)->pause();
                break;
            }
        }
        return *this;
    }

    DefaultMixer& DefaultMixer::resume(const std::string& animName)
    {
        for(std::set<AnimationPtr>::iterator i = m_cyclingAnims.begin() ; i != m_cyclingAnims.end() ; ++i) {
            if((*i)->name() == animName) {
                (*i)->resume();
                break;
            }
        }
        for(std::list<AnimationPtr>::iterator i = m_oneshotAnims.begin() ; i != m_oneshotAnims.end() ; ++i) {
            if((*i)->name() == animName) {
                (*i)->resume();
                break;
            }
        }
        return *this;
    }

    bool DefaultMixer::paused(const std::string& animName) const
    {
        for(std::set<AnimationPtr>::const_iterator i = m_cyclingAnims.begin() ; i != m_cyclingAnims.end() ; ++i) {
            if((*i)->name() == animName) {
                return (*i)->paused();
            }
        }
        for(std::list<AnimationPtr>::const_iterator i = m_oneshotAnims.begin() ; i != m_oneshotAnims.end() ; ++i) {
            if((*i)->name() == animName) {
                return (*i)->paused();
            }
        }
        return false;
    }

    DefaultMixer& DefaultMixer::pauseAll()
    {
        Mixer::pauseAll();
        for(std::set<AnimationPtr>::iterator i = m_cyclingAnims.begin() ; i != m_cyclingAnims.end() ; ++i) {
            (*i)->pause();
        }
        for(std::list<AnimationPtr>::iterator i = m_oneshotAnims.begin() ; i != m_oneshotAnims.end() ; ++i) {
            (*i)->pause();
        }
        return *this;
    }

    DefaultMixer& DefaultMixer::resumeAll()
    {
        Mixer::resumeAll();
        for(std::set<AnimationPtr>::iterator i = m_cyclingAnims.begin() ; i != m_cyclingAnims.end() ; ++i) {
            (*i)->resume();
        }
        for(std::list<AnimationPtr>::iterator i = m_oneshotAnims.begin() ; i != m_oneshotAnims.end() ; ++i) {
            (*i)->resume();
        }
        return *this;
    }

    DefaultMixer& DefaultMixer::stopAll(float fadeOutTime)
    {
        for(std::set<AnimationPtr>::iterator i = m_cyclingAnims.begin() ; i != m_cyclingAnims.end() ; ++i) {
            this->stop(*i, fadeOutTime);
        }

        for(std::list<AnimationPtr>::iterator i = m_oneshotAnims.begin() ; i != m_oneshotAnims.end() ; ++i) {
            this->stop(*i, fadeOutTime);
        }
        return *this;
    }

    float DefaultMixer::speed(const std::string& animName) const
    {
        for(std::set<AnimationPtr>::const_iterator i = m_cyclingAnims.begin() ; i != m_cyclingAnims.end() ; ++i) {
            if((*i)->name() == animName)
                return (*i)->speed();
        }
        for(std::list<AnimationPtr>::const_iterator i = m_oneshotAnims.begin() ; i != m_oneshotAnims.end() ; ++i) {
            if((*i)->name() == animName)
                return (*i)->speed();
        }

        return 1.0f;
    }

    DefaultMixer& DefaultMixer::speed(const std::string& animName, float speed)
    {
        for(std::set<AnimationPtr>::iterator i = m_cyclingAnims.begin() ; i != m_cyclingAnims.end() ; ++i) {
            if((*i)->name() == animName)
                (*i)->speed(speed);
        }
        for(std::list<AnimationPtr>::iterator i = m_oneshotAnims.begin() ; i != m_oneshotAnims.end() ; ++i) {
            if((*i)->name() == animName)
                (*i)->speed(speed);
        }

        return *this;
    }

    void DefaultMixer::doScheduledStops()
    {
        // We need to stop all marked animations whose weight is 0 or lower.

        for(std::set<AnimationPtr>::iterator iToStop = m_scheduledStop.begin() ; iToStop != m_scheduledStop.end() ; ) {
            // Not weighted out yet? Keep it going then...
            if((*iToStop)->weight() > D_BOUGE_EPSILON) {
                ++iToStop;
                continue;
            }

            bool bGotIt = false;
            if(m_cyclingAnims.find(*iToStop) != m_cyclingAnims.end()) {
                bGotIt = m_cyclingAnims.erase(*iToStop) > 0;
            }

            for(std::list<AnimationPtr>::iterator i = m_oneshotAnims.begin() ; !bGotIt && i != m_oneshotAnims.end() ; ++i) {
                if(*i == *iToStop) {
                    m_oneshotAnims.erase(i);
                    bGotIt = true;
                    break; // <- needed because of ++i.
                }
            }

            // Remove it from the set. (This works.)
            m_scheduledStop.erase(iToStop++);
        }
    }

    struct ZeroFloat {
        float v;

        ZeroFloat() : v(0.0f) {};
        operator float&() {return v;};
    };

    void DefaultMixer::update(float deltaTime)
    {
        deltaTime *= Mixer::speed();

        this->doScheduledStops();

        // First of all, we need to clear all the bones that come in play.
        // But I guess (not profiled!) that it's faster just clearing them all,
        // as this will prevent looking up all animated bones by name and most
        // anims animate a lot of bones.
        m_skel->resetAllBones();

        // A one-shot animation replaces all others IF it has come to a weight of
        // 100% or above. Else, it uses exactly its weight, not normalized with
        // the other animations. Other animations get normalized to 1-w.

        // Example: we got that model of a dude, having a cyclic walk animation.
        // He also has a wave animation so he can say goodbye to his friends.
        //
        // He is walking, with a 100% weighted walk animation. The walk animation
        // influences his arms movement a bit, too.
        // Now you want him to play a one-time "wave" action. In order not to have
        // his arm suddenly jump into the wave position, you'd want to have a "fade"
        // in between the positions. That's the fade-in of the action. It changes
        // the action's weight from 0 to 1 during the fade-in period.
        // When the action's weight is set to 0, the arm should be fully controlled
        // by the "walk", while when it's at 1, the arm should be fully controlled
        // by the "wave" animation.

        std::map<std::string, ZeroFloat> bonesOneShot;

        // We first apply the one-shot animations following the 'last added overrides' principle.
        // We do this first as it might save us from updating some bones below.
        for(std::list<AnimationPtr>::iterator iAnim = m_oneshotAnims.begin() ; iAnim != m_oneshotAnims.end() ; ++iAnim) {
            AnimationPtr anim = *iAnim;
            anim->update(deltaTime);

            float t = anim->timeAbsolute();
            float w = anim->weight();

            // Done with that one-shot action.
            if(anim->hasEnded())
                m_scheduledStop.insert(anim);

            if(nearZero(w))
                continue;

            for(CoreAnimation::const_iterator iTrack = anim->core()->begin() ; iTrack != anim->core()->end() ; ++iTrack) {
                bonesOneShot[iTrack.bone()] += w;

                BoneInstancePtr bone = m_skel->bone(iTrack.bone());
                if(iTrack->hasRotation()) {
                    bone->rot(bone->rot() * Quaternion().nlerp(iTrack->rotation(t), w));
                }
                if(iTrack->hasTranslation()) {
                    bone->trans(bone->trans() + Vector().lerp(iTrack->translation(t), w));
                }
                if(iTrack->hasScale()) {
                    bone->scale(bone->scale() * Vector(1.0f, 1.0f, 1.0f).lerp(iTrack->scale(t), w));
                }
            }
        }

        // And then, first prepare the "usual" animations, that get blended together.
        std::map<std::string, ZeroFloat> totalWeightPerBone;
        for(std::set<AnimationPtr>::iterator iAnim = m_cyclingAnims.begin() ; iAnim != m_cyclingAnims.end() ; ++iAnim) {
            AnimationPtr anim = *iAnim;
            anim->update(deltaTime);

            // We need the total of the weights first in order to normalize them later.
            float w = anim->weight();
            for(CoreAnimation::const_iterator iTrack = anim->core()->begin() ; iTrack != anim->core()->end() ; ++iTrack) {
                totalWeightPerBone[iTrack.bone()] += w;
            }
        }

        // Finally, apply the "usual" animations, that get blended together.
        for(std::set<AnimationPtr>::iterator iAnim = m_cyclingAnims.begin() ; iAnim != m_cyclingAnims.end() ; ++iAnim) {
            AnimationPtr anim = *iAnim;
            float t = anim->timeAbsolute();
            float animW = anim->weight();

            if(nearZero(animW))
                continue;

            for(CoreAnimation::const_iterator iTrack = anim->core()->begin() ; iTrack != anim->core()->end() ; ++iTrack) {
                // Use per-bone weights so that only bones used by both (or more) cycling
                // animations get blended, while those being used by say just one
                // of the animations still has its full movement.

                // Imagine a chicken which has one "fly" animation moving his wings
                // and one walk animation moving his legs and head.
                // If we want the chicken to walk around also moving his wings as
                // if it'd try to fly, we can just play both cycles with a 100%
                // weight. The bones used by both animations will be blended to
                // 50% each, while the ones used by only one animation will be
                // fully animated by that one.
                // This is solved by the following equation:
                //    bone-anim-weight = anim-weight / total-bone-weight
                // In our example, for a bone influenced by both:
                //         0.5         =    1        /          2
                // and for a bone influenced by just one:
                //          1          =    1        /          1

                // Yet, there's more to be covered. Imagine one plays just one animation
                // but set its weight to 50% (or: a bone is influenced by just one
                // animation whose weight is 50%). For example, say the walk is played
                // at 100% but we don't want the chicken to fly that hard, thus set
                // the fly to only 50%.
                // the above formula won't do for bones animated only by "fly":
                //          1          =    0.5      /         0.5
                //    (but we'd expect the result to be 0.5!)
                // This is because of the "totalWeightPerBone", which normalizes.
                // But we only want to normalize in case the weight sums to more
                // than one.
                float tot = totalWeightPerBone[iTrack.bone()];
                float boneW = (tot <= 1.0f ? animW : animW / tot);

                // Regarding this one, read the big comment above the one-shot action loop above.
                boneW *= 1.0f - bonesOneShot[iTrack.bone()];

                BoneInstancePtr bone = m_skel->bone(iTrack.bone());
                if(iTrack->hasRotation()) {
                    // Here, nlerp is fine (and fast), because w should't ever go above 1.0!
                    bone->rot(bone->rot() * Quaternion().nlerp(iTrack->rotation(t), boneW));
                }
                if(iTrack->hasTranslation()) {
                    bone->trans(bone->trans() + Vector().lerp(iTrack->translation(t), boneW));
                }
                if(iTrack->hasScale()) {
                    bone->scale(bone->scale() * Vector(1.0f, 1.0f, 1.0f).lerp(iTrack->scale(t), boneW));
                }
            }
        }

        // Finally, let all the bones update their matrices.
        m_skel->recalcAllBones();
    }

} // namespace bouge
