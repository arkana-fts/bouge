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
#ifndef BOUGE_MODELINSTANCE_HPP
#define BOUGE_MODELINSTANCE_HPP

#include <bouge/bougefwd.hpp>

#include <string>
#include <list>
#include <map>

namespace bouge {

    class BOUGE_API ModelInstance
    {
    public:
        ModelInstance(CoreModelPtrC coremdl);
        virtual ~ModelInstance();

        BOUGE_USER_DATA;

        std::string currMatSet() const;
        ModelInstance& selectMatSet(std::string matset);

        CoreMaterialPtrC materialForSubmesh(const std::string& submesh);
        CoreMaterialPtrC materialForSubmesh(const std::string& submesh) const;

        SkeletonInstancePtr skeleton();
        SkeletonInstancePtrC skeleton() const;

        MixerPtr mixer();
        MixerPtrC mixer() const;

        CoreModelPtrC core() const;

        // The following are just methods to make the API more user-friendly
        // than directly accessing the mixer. (Also try to fulfill Demeter's law.)

        AnimationPtr playCycle(const std::string anim, float speed = 1.0f, float fadeInTime = 0.3f, float weight = 1.0f, TimeFunction* control = 0);
        AnimationPtr playOneShot(const std::string anim, float speed = 1.0f, float fadeInTime = 0.0f, float fadeOutTime = 0.0f, TimeFunction* control = 0);
        ModelInstance& stop(const std::string anim, float fadeOutTime = 0.3f);
        ModelInstance& stop(AnimationPtr anim, float fadeOutTime = 0.3f);
        ModelInstance& pause(const std::string anim);
        ModelInstance& resume(const std::string anim);
        bool paused(const std::string anim) const;

        ModelInstance& stopAll(float fadeOutTime = 0.3f);
        ModelInstance& pauseAll();
        ModelInstance& resumeAll();
        bool paused() const;

        float speed() const;
        ModelInstance& speed(float s);
        float speed(const std::string anim) const;
        ModelInstance& speed(const std::string anim, float s);

        ModelInstance& attachAnimation(CoreAnimationPtrC anim);
        ModelInstance& detachAnimation(const std::string& name);

        ModelInstance& attachAllAnimations(ModelInstancePtrC from);
        ModelInstance& detachAllAnimations(ModelInstancePtrC from);

        // Does this even work?
//         ModelInstance& attachModel(ModelInstancePtrC model, std::string boneName);
//         ModelInstance& detachModel(ModelInstancePtrC model);
//         ModelInstance& detachModel(std::string boneName);

        // Does this even work?
//         ModelInstance& attachMesh(mesh);
//         ModelInstance& detachMesh();

    protected:
        CoreAnimationPtrC findAnimToUse(const std::string& name) const;

    private:
        CoreModelPtrC m_coremdl;
        SkeletonInstancePtr m_skel;
        MixerPtr m_mixer;
        std::string m_sMatSet;

        // animations attached individually.
        std::map<std::string, CoreAnimationPtrC> m_attachedAnims;

        // models whose animations to attach.
        std::list<ModelInstancePtrC> m_attachedAnimSources;
    };

} // namespace bouge

#endif // BOUGE_MODELINSTANCE_HPP
