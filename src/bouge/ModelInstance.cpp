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
#include <bouge/ModelInstance.hpp>
#include <bouge/CoreModel.hpp>
#include <bouge/CoreMaterialSet.hpp>
#include <bouge/Mixer.hpp>
#include <bouge/SkeletonInstance.hpp>
#include <bouge/Math/TimeFunction.hpp>
#include <bouge/CoreAnimation.hpp>
#include <bouge/Math/Util.hpp>

namespace bouge {

    ModelInstance::ModelInstance(CoreModelPtrC coremdl)
        : m_coremdl(coremdl)
        , m_skel(coremdl->skeleton() ? new SkeletonInstance(coremdl->skeleton()) : 0)
        , m_mixer(m_skel ? reinterpret_cast<Mixer*>(new DefaultMixer(m_skel)) : reinterpret_cast<Mixer*>(new DummyMixer()))
    {
        // Select the first material set if there are some.
        if(m_coremdl->begin_materialset() != m_coremdl->end_materialset()) {
            this->selectMatSet(m_coremdl->begin_materialset()->name());
        }
    }

    ModelInstance::~ModelInstance()
    { }

    std::string ModelInstance::currMatSet() const
    {
        return m_sMatSet;
    }

    ModelInstance& ModelInstance::selectMatSet(std::string matset)
    {
        m_sMatSet = matset;
        return *this;
    }

    CoreMaterialPtrC ModelInstance::materialForSubmesh(const std::string& submesh)
    {
        return m_coremdl->materialForSubmesh(submesh, m_sMatSet);
    }

    CoreMaterialPtrC ModelInstance::materialForSubmesh(const std::string& submesh) const
    {
        return m_coremdl->materialForSubmesh(submesh, m_sMatSet);
    }

    SkeletonInstancePtr ModelInstance::skeleton()
    {
        return m_skel;
    }

    SkeletonInstancePtrC ModelInstance::skeleton() const
    {
        return m_skel;
    }

    MixerPtr ModelInstance::mixer()
    {
        return m_mixer;
    }

    MixerPtrC ModelInstance::mixer() const
    {
        return m_mixer;
    }

    CoreModelPtrC ModelInstance::core() const
    {
        return m_coremdl;
    }

    AnimationPtr ModelInstance::playCycle(const std::string anim, float speed, float fadeInTime, float weight, TimeFunction* control)
    {
        CoreAnimationPtrC coreAnim = this->findAnimToUse(anim);

        if(nearZero(coreAnim->duration())) {
            return m_mixer->play(AnimationPtr(new Animation(coreAnim, speed)));
        }

        // Note: as the FadeInTF uses normalized-time, we need to divide our
        //       absolue time by the animation's duration in order to normalize it.
        TimeFunction* pWeightFun = new FadeInTF(fadeInTime/coreAnim->duration(), new ConstantTF(weight));
        return m_mixer->play(AnimationPtr(new Animation(coreAnim, speed, control, pWeightFun)));
    }

    AnimationPtr ModelInstance::playOneShot(const std::string anim, float speed, float fadeInTime, float fadeOutTime, TimeFunction* control)
    {
        CoreAnimationPtrC coreAnim = this->findAnimToUse(anim);

        if(nearZero(coreAnim->duration())) {
            return m_mixer->oneshot(AnimationPtr(new Animation(coreAnim, speed)));
        }

        // Note: same as in playCycle.
        TimeFunction* pWeightFun = new FadeOutTF(fadeOutTime/coreAnim->duration(), new FadeInTF(fadeInTime/coreAnim->duration(), new ConstantTF(1.0f)));
        return m_mixer->oneshot(AnimationPtr(new Animation(m_coremdl->animation(anim), speed, control, pWeightFun)));
    }

    ModelInstance& ModelInstance::stop(const std::string anim, float fadeOutTime)
    {
        m_mixer->stop(anim, fadeOutTime);
        return *this;
    }

    ModelInstance& ModelInstance::stop(AnimationPtr anim, float fadeOutTime)
    {
        m_mixer->stop(anim, fadeOutTime);
        return *this;
    }

    ModelInstance& ModelInstance::pause(const std::string anim)
    {
        m_mixer->pause(anim);
        return *this;
    }

    ModelInstance& ModelInstance::resume(const std::string anim)
    {
        m_mixer->resume(anim);
        return *this;
    }

    bool ModelInstance::paused(const std::string anim) const
    {
        return m_mixer->paused(anim);
    }

    ModelInstance& ModelInstance::stopAll(float fadeOutTime)
    {
        m_mixer->stopAll(fadeOutTime);
        return *this;
    }

    ModelInstance& ModelInstance::pauseAll()
    {
        m_mixer->pauseAll();
        return *this;
    }

    ModelInstance& ModelInstance::resumeAll()
    {
        m_mixer->resumeAll();
        return *this;
    }

    bool ModelInstance::paused() const
    {
        return m_mixer->paused();
    }

    float ModelInstance::speed() const
    {
        return m_mixer->speed();
    }

    ModelInstance& ModelInstance::speed(float s)
    {
        m_mixer->speed(s);
        return *this;
    }

    float ModelInstance::speed(const std::string anim) const
    {
        return m_mixer->speed(anim);
    }

    ModelInstance& ModelInstance::speed(const std::string anim, float s)
    {
        m_mixer->speed(anim, s);
        return *this;
    }

    ModelInstance& ModelInstance::attachAnimation(CoreAnimationPtrC anim)
    {
        m_attachedAnims[anim->name()] = anim;
        return *this;
    }

    ModelInstance& ModelInstance::detachAnimation(const std::string& name)
    {
        m_attachedAnims.erase(name);
        return *this;
    }

    ModelInstance& ModelInstance::attachAllAnimations(ModelInstancePtrC from)
    {
        // Push to the front to use the last added in case of duplicates.
        m_attachedAnimSources.push_front(from);
        return *this;
    }

    ModelInstance& ModelInstance::detachAllAnimations(ModelInstancePtrC from)
    {
        for(std::list<ModelInstancePtrC>::iterator i = m_attachedAnimSources.begin() ; i != m_attachedAnimSources.end() ; ++i) {
            if(*i == from) {
                m_attachedAnimSources.erase(i);
                return *this;
            }
        }

        return *this;
    }

    CoreAnimationPtrC ModelInstance::findAnimToUse(const std::string& name) const
    {
        CoreAnimationPtrC coreAnim;

        // First, look for the animation in the individual attached animations. Use that if found.
        std::map<std::string, CoreAnimationPtrC>::const_iterator iAnim = m_attachedAnims.find(name);
        if(iAnim != m_attachedAnims.end())
            return iAnim->second;

        // Next, look into the attached animation sources.
        for(std::list<ModelInstancePtrC>::const_iterator i = m_attachedAnimSources.begin() ; i != m_attachedAnimSources.end() ; ++i) {
            if((*i)->core()->hasAnimation(name))
                return (*i)->core()->animation(name);
        }

        // If not found elsewhere, use my own animation. (Or throw if even I don't have it.)
        return m_coremdl->animation(name);
    }

} // namespace bouge
