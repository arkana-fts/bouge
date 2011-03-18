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
#include <bouge/SkeletonInstance.hpp>
#include <bouge/CoreSkeleton.hpp>
#include <bouge/CoreBone.hpp>
#include <bouge/Exception.hpp>
#include <bouge/BoneInstance.hpp>
#include <bouge/Util.hpp>

namespace bouge {

    SkeletonInstance::SkeletonInstance(CoreSkeletonPtrC core)
        : m_core(core)
    {
        for(CoreSkeleton::const_iterator iCoreBone = core->begin() ; iCoreBone != core->end() ; ++iCoreBone) {
//             BoneId id = m_bones.size();
            m_bones.push_back(BoneInstancePtr(new BoneInstance(*iCoreBone/*, id*/)));
        }
    }

    SkeletonInstance::~SkeletonInstance()
    { }

    std::string SkeletonInstance::name() const
    {
        return this->core()->name();
    }

    CoreSkeletonPtrC SkeletonInstance::core() const
    {
        return m_core;
    }

    bool SkeletonInstance::hasBone(const std::string& name) const
    {
        try {
            return m_core->boneId(name) < m_bones.size();
        } catch(const NotExistException&) {
            return false;
        }
    }

    BoneInstancePtr SkeletonInstance::bone(const std::string& name)
    {
        return this->bone(m_core->boneId(name));
    }

    BoneInstancePtrC SkeletonInstance::bone(const std::string& name) const
    {
        return this->bone(m_core->boneId(name));
    }

    BoneInstancePtr SkeletonInstance::bone(const BoneId& id)
    {
        if(id >= m_bones.size())
            throw NotExistException("bone instance", to_s(id), "skeleton instance", this->name(), __FILE__, __LINE__);

        return m_bones[id];
    }

    BoneInstancePtrC SkeletonInstance::bone(const BoneId& id) const
    {
        if(id >= m_bones.size())
            throw NotExistException("bone instance", to_s(id), "skeleton instance", this->name(), __FILE__, __LINE__);

        return m_bones.at(id);
    }

    SkeletonInstance& SkeletonInstance::recalcAllBones()
    {
        SkeletonInstancePtr self = this->shared_from_this();
        for(root_iterator iRootBone = this->begin_root() ; iRootBone != this->end_root() ; ++iRootBone) {
            iRootBone->recalcRecursive(self);
        }

        return *this;
    }

    SkeletonInstance& SkeletonInstance::resetAllBones()
    {
        SkeletonInstancePtr self = this->shared_from_this();
        for(root_iterator iRootBone = this->begin_root() ; iRootBone != this->end_root() ; ++iRootBone) {
            iRootBone->resetRecursive(self);
        }

        return *this;
    }

    SkeletonInstance::BoneMap::size_type SkeletonInstance::boneCount() const
    {
        return m_bones.size();
    }

    std::size_t SkeletonInstance::rootBoneCount() const
    {
        return m_core->rootBoneCount();
    }

    SkeletonInstance::iterator::iterator(SkeletonInstance* me, CoreSkeleton::const_iterator iter)
        : me(me)
        , myIter(iter)
    { }

    SkeletonInstance::iterator::~iterator()
    { }

    bool SkeletonInstance::iterator::operator==(iterator other) const
    {
        return myIter == other.myIter && me == other.me;
    }

    bool SkeletonInstance::iterator::operator!=(iterator other) const
    {
        return myIter != other.myIter || me != other.me;
    }

    SkeletonInstance::iterator& SkeletonInstance::iterator::operator++()
    {
        return ++myIter, *this;
    }

    SkeletonInstance::iterator SkeletonInstance::iterator::operator++(int)
    {
        return iterator(me, myIter++);
    }

    SkeletonInstance::iterator& SkeletonInstance::iterator::operator--()
    {
        return --myIter, *this;
    }

    SkeletonInstance::iterator SkeletonInstance::iterator::operator--(int)
    {
        return iterator(me, myIter--);
    }

    BoneInstancePtr SkeletonInstance::iterator::operator*()
    {
        return me->bone(myIter->id());
    }

    BoneInstance* SkeletonInstance::iterator::operator->()
    {
        return me->bone(myIter->id()).operator->();
    }

    SkeletonInstance::iterator SkeletonInstance::begin()
    {
        return iterator(this, this->core()->begin());
    }

    SkeletonInstance::iterator SkeletonInstance::end()
    {
        return iterator(this, this->core()->end());
    }

    SkeletonInstance::const_iterator::const_iterator(const SkeletonInstance* me, CoreSkeleton::const_iterator iter)
        : me(me)
        , myIter(iter)
    { }

    SkeletonInstance::const_iterator::~const_iterator()
    { }

    bool SkeletonInstance::const_iterator::operator==(const_iterator other) const
    {
        return myIter == other.myIter && me == other.me;
    }

    bool SkeletonInstance::const_iterator::operator!=(const_iterator other) const
    {
        return myIter != other.myIter || me != other.me;
    }

    SkeletonInstance::const_iterator& SkeletonInstance::const_iterator::operator++()
    {
        return ++myIter, *this;
    }

    SkeletonInstance::const_iterator SkeletonInstance::const_iterator::operator++(int)
    {
        return const_iterator(me, myIter++);
    }

    SkeletonInstance::const_iterator& SkeletonInstance::const_iterator::operator--()
    {
        return --myIter, *this;
    }

    SkeletonInstance::const_iterator SkeletonInstance::const_iterator::operator--(int)
    {
        return const_iterator(me, myIter--);
    }

    BoneInstancePtrC SkeletonInstance::const_iterator::operator*() const
    {
        return me->bone(myIter->id());
    }

    const BoneInstance* SkeletonInstance::const_iterator::operator->() const
    {
        return me->bone(myIter->id()).operator->();
    }

    SkeletonInstance::const_iterator SkeletonInstance::begin() const
    {
        return const_iterator(this, this->core()->begin());
    }

    SkeletonInstance::const_iterator SkeletonInstance::end() const
    {
        return const_iterator(this, this->core()->end());
    }

    SkeletonInstance::root_iterator::root_iterator(SkeletonInstance* me, CoreSkeleton::const_root_iterator iter)
        : me(me)
        , myIter(iter)
    { }

    SkeletonInstance::root_iterator::~root_iterator()
    { }

    bool SkeletonInstance::root_iterator::operator==(root_iterator other) const
    {
        return myIter == other.myIter && me == other.me;
    }

    bool SkeletonInstance::root_iterator::operator!=(root_iterator other) const
    {
        return myIter != other.myIter || me != other.me;
    }

    SkeletonInstance::root_iterator& SkeletonInstance::root_iterator::operator++()
    {
        return ++myIter, *this;
    }

    SkeletonInstance::root_iterator SkeletonInstance::root_iterator::operator++(int)
    {
        return root_iterator(me, myIter++);
    }

    SkeletonInstance::root_iterator& SkeletonInstance::root_iterator::operator--()
    {
        return --myIter, *this;
    }

    SkeletonInstance::root_iterator SkeletonInstance::root_iterator::operator--(int)
    {
        return root_iterator(me, myIter--);
    }

    BoneInstancePtr SkeletonInstance::root_iterator::operator*()
    {
        return me->bone(myIter->id());
    }

    BoneInstance* SkeletonInstance::root_iterator::operator->()
    {
        return me->bone(myIter->id()).operator->();
    }

    SkeletonInstance::root_iterator SkeletonInstance::begin_root()
    {
        return root_iterator(this, this->core()->begin_root());
    }

    SkeletonInstance::root_iterator SkeletonInstance::end_root()
    {
        return root_iterator(this, this->core()->end_root());
    }

    SkeletonInstance::const_root_iterator::const_root_iterator(const SkeletonInstance* me, CoreSkeleton::const_root_iterator iter)
        : me(me)
        , myIter(iter)
    { }

    SkeletonInstance::const_root_iterator::~const_root_iterator()
    { }

    bool SkeletonInstance::const_root_iterator::operator==(const_root_iterator other) const
    {
        return myIter == other.myIter && me == other.me;
    }

    bool SkeletonInstance::const_root_iterator::operator!=(const_root_iterator other) const
    {
        return myIter != other.myIter || me != other.me;
    }

    SkeletonInstance::const_root_iterator& SkeletonInstance::const_root_iterator::operator++()
    {
        return ++myIter, *this;
    }

    SkeletonInstance::const_root_iterator SkeletonInstance::const_root_iterator::operator++(int)
    {
        return const_root_iterator(me, myIter++);
    }

    SkeletonInstance::const_root_iterator& SkeletonInstance::const_root_iterator::operator--()
    {
        return --myIter, *this;
    }

    SkeletonInstance::const_root_iterator SkeletonInstance::const_root_iterator::operator--(int)
    {
        return const_root_iterator(me, myIter--);
    }

    BoneInstancePtrC SkeletonInstance::const_root_iterator::operator*() const
    {
        return me->bone(myIter->id());
    }

    const BoneInstance* SkeletonInstance::const_root_iterator::operator->() const
    {
        return me->bone(myIter->id()).operator->();
    }

    SkeletonInstance::const_root_iterator SkeletonInstance::begin_root() const
    {
        return const_root_iterator(this, this->core()->begin_root());
    }

    SkeletonInstance::const_root_iterator SkeletonInstance::end_root() const
    {
        return const_root_iterator(this, this->core()->end_root());
    }

}
