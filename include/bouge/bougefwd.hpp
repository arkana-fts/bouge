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
#ifndef BOUGE_FWD_HPP
#define BOUGE_FWD_HPP

#include <bouge/Config.hpp>
#include <bouge/UserData.hpp>

namespace bouge {

    class Base4x4Matrix;
    class AffineMatrix;
    class General4x4Matrix;
    class Quaternion;
    class Vector;
    class TimeFunction;

    class BoneInstance;
    typedef bouge::shared_ptr<BoneInstance>::type BoneInstancePtr;
    typedef bouge::shared_ptr<const BoneInstance>::type BoneInstancePtrC;
    typedef unsigned int BoneId;

    class CoreAnimation;
    typedef bouge::shared_ptr<CoreAnimation>::type CoreAnimationPtr;
    typedef bouge::shared_ptr<const CoreAnimation>::type CoreAnimationPtrC;

    class CoreBone;
    typedef bouge::shared_ptr<CoreBone>::type CoreBonePtr;
    typedef bouge::shared_ptr<const CoreBone>::type CoreBonePtrC;

    class CoreHardwareMesh;
    typedef bouge::shared_ptr<CoreHardwareMesh>::type CoreHardwareMeshPtr;
    typedef bouge::shared_ptr<const CoreHardwareMesh>::type CoreHardwareMeshPtrC;
    class CoreHardwareSubMesh;
    typedef bouge::shared_ptr<CoreHardwareSubMesh>::type CoreHardwareSubMeshPtr;
    typedef bouge::shared_ptr<const CoreHardwareSubMesh>::type CoreHardwareSubMeshPtrC;

    class CoreKeyframe;
    typedef bouge::shared_ptr<CoreKeyframe>::type CoreKeyframePtr;
    typedef bouge::shared_ptr<const CoreKeyframe>::type CoreKeyframePtrC;

    class CoreMaterial;
    typedef bouge::shared_ptr<CoreMaterial>::type CoreMaterialPtr;
    typedef bouge::shared_ptr<const CoreMaterial>::type CoreMaterialPtrC;

    class CoreMaterialSet;
    typedef bouge::shared_ptr<CoreMaterialSet>::type CoreMaterialSetPtr;
    typedef bouge::shared_ptr<const CoreMaterialSet>::type CoreMaterialSetPtrC;

    class CoreMesh;
    typedef bouge::shared_ptr<CoreMesh>::type CoreMeshPtr;
    typedef bouge::shared_ptr<const CoreMesh>::type CoreMeshPtrC;
    class CoreSubMesh;
    typedef bouge::shared_ptr<CoreSubMesh>::type CoreSubMeshPtr;
    typedef bouge::shared_ptr<const CoreSubMesh>::type CoreSubMeshPtrC;

    class CoreModel;
    typedef bouge::shared_ptr<CoreModel>::type CoreModelPtr;
    typedef bouge::shared_ptr<const CoreModel>::type CoreModelPtrC;

    class CoreSkeleton;
    typedef bouge::shared_ptr<CoreSkeleton>::type CoreSkeletonPtr;
    typedef bouge::shared_ptr<const CoreSkeleton>::type CoreSkeletonPtrC;

    class CoreTrack;
    typedef bouge::shared_ptr<CoreTrack>::type CoreTrackPtr;
    typedef bouge::shared_ptr<const CoreTrack>::type CoreTrackPtrC;

    class Exception;
    class NotExistException;
    class NoParentException;

    class Face;
    typedef bouge::shared_ptr<Face>::type FacePtr;
    typedef bouge::shared_ptr<const Face>::type FacePtrC;

    class ModelInstance;
    typedef bouge::shared_ptr<ModelInstance>::type ModelInstancePtr;
    typedef bouge::shared_ptr<const ModelInstance>::type ModelInstancePtrC;

    class SkeletonInstance;
    typedef bouge::shared_ptr<SkeletonInstance>::type SkeletonInstancePtr;
    typedef bouge::shared_ptr<const SkeletonInstance>::type SkeletonInstancePtrC;

    class StaticModelInstance;
    typedef bouge::shared_ptr<StaticModelInstance>::type StaticModelInstancePtr;
    typedef bouge::shared_ptr<const StaticModelInstance>::type StaticModelInstancePtrC;

    struct Influence;
    typedef bouge::shared_ptr<Influence>::type InfluencePtr;
    typedef bouge::shared_ptr<const Influence>::type InfluencePtrC;
    class Vertex;
    typedef bouge::shared_ptr<Vertex>::type VertexPtr;
    typedef bouge::shared_ptr<const Vertex>::type VertexPtrC;

    class Mixer;
    typedef bouge::shared_ptr<Mixer>::type MixerPtr;
    typedef bouge::shared_ptr<const Mixer>::type MixerPtrC;

    class Animation;
    typedef bouge::shared_ptr<Animation>::type AnimationPtr;
    typedef bouge::shared_ptr<const Animation>::type AnimationPtrC;

} // namespace bouge

#endif // BOUGE_FWD_HPP
