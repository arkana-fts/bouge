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
#include <bouge/BoneInstance.hpp>
#include <bouge/SkeletonInstance.hpp>
#include <bouge/CoreBone.hpp>
#include <bouge/Exception.hpp>

namespace bouge {

    BoneInstance::BoneInstance(CoreBonePtrC core)
        : m_core(core)
        , m_scale(1.0f, 1.0f, 1.0f)
        , m_transl(/*core->relativeRootPosition()*/)
        , m_rotation(/*core->relativeBoneRotation()*/)
        , m_bDirty(true)
    { }

    BoneInstance::~BoneInstance()
    { }

    std::string BoneInstance::name() const
    {
        return this->core()->name();
    }

    CoreBonePtrC BoneInstance::core() const
    {
        return m_core;
    }

    bool BoneInstance::hasParent() const
    {
        return this->core()->hasParent();
    }

    BoneInstancePtr BoneInstance::parent(SkeletonInstancePtr skel)
    {
        try {
            return skel->bone(this->core()->parent()->id());
        } catch(...) {
            throw NoParentException("bone instance", this->name(), __FILE__, __LINE__);
        }
    }

    BoneInstancePtrC BoneInstance::parent(SkeletonInstancePtrC skel) const
    {
        try {
            return skel->bone(this->core()->parent()->id());
        } catch(...) {
            throw NoParentException("bone instance", this->name(), __FILE__, __LINE__);
        }
    }

    std::size_t BoneInstance::childCount() const
    {
        return this->core()->childCount();
    }

    bool BoneInstance::hasChild(const std::string& name, SkeletonInstancePtrC skel) const
    {
        return this->core()->hasChild(name) && skel->hasBone(name);
    }

    BoneInstancePtrC BoneInstance::child(const std::string& name, SkeletonInstancePtrC skel) const
    {
        return skel->bone(this->core()->child(name)->id());
    }

    BoneInstancePtr BoneInstance::child(const std::string& name, SkeletonInstancePtr skel)
    {
        return skel->bone(this->core()->child(name)->id());
    }

    BoneInstance::iterator::iterator(CoreBone::const_iterator me, SkeletonInstancePtr skel)
        : myIter(me)
        , mySkel(skel)
    { }

    BoneInstance::iterator::iterator()
    { }

    BoneInstance::iterator::~iterator()
    { }

    bool BoneInstance::iterator::operator==(iterator other) const
    {
        return myIter == other.myIter;
    }

    bool BoneInstance::iterator::operator!=(iterator other) const
    {
        return myIter != other.myIter;
    }

    BoneInstance::iterator& BoneInstance::iterator::operator++()
    {
        return ++myIter, *this;
    }

    BoneInstance::iterator BoneInstance::iterator::operator++(int)
    {
        return iterator(myIter++, mySkel);
    }

    BoneInstance::iterator& BoneInstance::iterator::operator--()
    {
        return --myIter, *this;
    }

    BoneInstance::iterator BoneInstance::iterator::operator--(int)
    {
        return iterator(myIter--, mySkel);
    }

    BoneInstancePtr BoneInstance::iterator::operator*()
    {
        return mySkel->bone(myIter->id());
    }

    BoneInstance* BoneInstance::iterator::operator->()
    {
        return mySkel->bone(myIter->id()).operator->();
    }

    BoneInstance::iterator BoneInstance::begin(SkeletonInstancePtr skel)
    {
        return iterator(this->core()->begin(), skel);
    }

    BoneInstance::iterator BoneInstance::end(SkeletonInstancePtr skel)
    {
        return iterator(this->core()->end(), skel);
    }

    BoneInstance::const_iterator::const_iterator(CoreBone::const_iterator me, SkeletonInstancePtrC skel)
        : myIter(me)
        , mySkel(skel)
    { }

    BoneInstance::const_iterator::const_iterator()
    { }

    BoneInstance::const_iterator::~const_iterator()
    { }

    bool BoneInstance::const_iterator::operator==(const_iterator other) const
    {
        return myIter == other.myIter;
    }

    bool BoneInstance::const_iterator::operator!=(const_iterator other) const
    {
        return myIter != other.myIter;
    }

    BoneInstance::const_iterator& BoneInstance::const_iterator::operator++()
    {
        return ++myIter, *this;
    }

    BoneInstance::const_iterator BoneInstance::const_iterator::operator++(int)
    {
        return const_iterator(myIter++, mySkel);
    }

    BoneInstance::const_iterator& BoneInstance::const_iterator::operator--()
    {
        return --myIter, *this;
    }

    BoneInstance::const_iterator BoneInstance::const_iterator::operator--(int)
    {
        return const_iterator(myIter--, mySkel);
    }

    BoneInstancePtrC BoneInstance::const_iterator::operator*() const
    {
        return mySkel->bone(myIter->id());
    }

    const BoneInstance* BoneInstance::const_iterator::operator->() const
    {
        return mySkel->bone(myIter->id()).operator->();
    }

    BoneInstance::const_iterator BoneInstance::begin(SkeletonInstancePtrC skel) const
    {
        return const_iterator(this->core()->begin(), skel);
    }

    BoneInstance::const_iterator BoneInstance::end(SkeletonInstancePtrC skel) const
    {
        return const_iterator(this->core()->end(), skel);
    }

    BoneInstance& BoneInstance::recalcRecursive(SkeletonInstancePtr skel, bool bParentWasDirty)
    {
        bool bNeedUpdate = bParentWasDirty || m_bDirty;
        if(bNeedUpdate) {
            if(this->hasParent())
                this->recalc(this->parent(skel));
            else
                this->recalc();
        }

        for(iterator iChild = this->begin(skel) ; iChild != this->end(skel) ; ++iChild) {
            iChild->recalcRecursive(skel, bNeedUpdate);
        }
        return *this;
    }

    BoneInstance& BoneInstance::recalc(BoneInstancePtrC parent)
    {
        // The total rotation is dictated by, applied in order:
        //   -> The parent's rotation,
        //   -> The rotation of the bone in rest pose,
        //   -> The animated bone rotation, relative to its rest pose.
//         m_absoluteBoneRot = parent->m_absoluteBoneRot * this->rot();
        m_absoluteBoneRot = parent->m_absoluteBoneRot * this->core()->relativeBoneRotation() * this->rot();

        // Similar situation for the root position:
        //   -> The parent's root position,
        //   -> The current animated position.
        //
        // The latter is affected by:
        //    -> The position in rest pose,
        //    -> The current animated position, relative to the rest pose,
        //    -> The parent's rotation first,
        //    -> The parent's scale then.
//         m_absoluteRootPos = parent->m_absoluteRootPos + parent->m_scale * parent->m_absoluteBoneRot.rotate(this->trans());
        m_absoluteRootPos = parent->m_absoluteRootPos + parent->m_scale * parent->m_absoluteBoneRot.rotate(this->core()->relativeRootPosition() + this->trans());

        return this->recalcMatrixCache();
    }

    BoneInstance& BoneInstance::recalc()
    {
        // Just use the bone's current state when there is no parent.
        // i.e. absolute state == relative state
        m_absoluteRootPos = this->core()->relativeRootPosition() + this->trans();
        m_absoluteBoneRot = this->core()->relativeBoneRotation() * this->rot();

        return this->recalcMatrixCache();
    }

    BoneInstance& BoneInstance::recalcMatrixCache()
    {
        // No one-liner in order to save temporaries
        m_transformationMatrix.setTransformation(m_absoluteRootPos, this->scale(), m_absoluteBoneRot);
        m_transformationMatrix *= this->core()->modelSpaceToBoneSpaceMatrix();
        m_bDirty = false;
        return *this;
    }

    void BoneInstance::resetRecursive(SkeletonInstancePtr skel)
    {
        this->reset();
        for(iterator iChild = this->begin(skel) ; iChild != this->end(skel) ; ++iChild) {
            iChild->resetRecursive(skel);
        }
    }

    void BoneInstance::reset()
    {
        this->rot(Quaternion());
        this->trans(Vector());
        this->scale(Vector(1.0f, 1.0f, 1.0f));
    }

    Vector BoneInstance::scale() const
    {
        return m_scale;
    }

    BoneInstance& BoneInstance::scale(Vector v)
    {
        m_scale = v;
        m_bDirty = true;
        return *this;
    }

    Vector BoneInstance::trans() const
    {
        return m_transl;
    }

    BoneInstance& BoneInstance::trans(Vector v)
    {
        m_transl = v;
        m_bDirty = true;
        return *this;
    }

    Quaternion BoneInstance::rot() const
    {
        return m_rotation;
    }

    BoneInstance& BoneInstance::rot(Quaternion q)
    {
        m_rotation = q;
        m_bDirty = true;
        return *this;
    }

    float BoneInstance::length() const
    {
        return this->scale().y() * this->core()->length();
    }

    AffineMatrix BoneInstance::transformMatrix() const
    {
        return m_transformationMatrix;
    }

//     AffineMatrix BoneInstance::modelSpaceToBoneSpaceMatrix() const
//     {
//         return this->matrixBoneSpaceToModelSpace().inverse();
//     }

}
