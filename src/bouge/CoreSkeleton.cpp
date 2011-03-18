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
#include <bouge/CoreSkeleton.hpp>
#include <bouge/CoreBone.hpp>
#include <bouge/Exception.hpp>
#include <bouge/Util.hpp>

#include <stdexcept>

namespace bouge {

    CoreSkeleton::CoreSkeleton(std::string name)
        : m_sName(name)
    { }

    CoreSkeleton::~CoreSkeleton()
    { }

    std::string CoreSkeleton::name() const
    {
        return m_sName;
    }

    CoreSkeleton& CoreSkeleton::name(std::string name)
    {
        m_sName = name;
        return *this;
    }

    CoreSkeleton& CoreSkeleton::addRootBone(CoreBonePtr bone)
    {
        // Also need to recursively add all children of it to our bone map.
        m_rootBoneNames.push_back(this->addRecursively(bone));

        return *this;
    }

    bool CoreSkeleton::hasBone(const std::string& name) const
    {
        return m_boneNames.find(name) != m_boneNames.end();
    }

    BoneId CoreSkeleton::boneId(const std::string& name) const
    {
        BoneNamesMap::const_iterator i = m_boneNames.find(name);
        if(i == m_boneNames.end())
            throw NotExistException("core bone", name, "core skeleton", this->name(), __FILE__, __LINE__);
 
        return i->second;
    }

    CoreBonePtr CoreSkeleton::bone(const std::string& name)
    {
        return this->bone(this->boneId(name));
    }

    CoreBonePtrC CoreSkeleton::bone(const std::string& name) const
    {
        return this->bone(this->boneId(name));
    }

    CoreBonePtr CoreSkeleton::bone(const BoneId& id)
    {
        if(id >= m_allBones.size())
            throw NotExistException("core bone", to_s(id), "core skeleton", this->name(), __FILE__, __LINE__);

        return m_allBones[id];
    }

    CoreBonePtrC CoreSkeleton::bone(const BoneId& id) const
    {
        if(id >= m_allBones.size())
            throw NotExistException("core bone", to_s(id), "core skeleton", this->name(), __FILE__, __LINE__);

        return m_allBones.at(id);
    }

    BoneId CoreSkeleton::addRecursively(CoreBonePtr bone)
    {
        if(m_boneNames.find(bone->name()) != m_boneNames.end())
            throw std::invalid_argument("Adding bone " + bone->name() + " to skeleton which already has that bone.");

        bone->id(m_allBones.size());
        m_allBones.push_back(bone);
        m_boneNames.insert(std::make_pair(bone->name(), bone->id()));

        for(CoreBone::iterator i = bone->begin() ; i != bone->end() ; ++i) {
            this->addRecursively(*i);
        }

        return bone->id();
    }

    CoreSkeleton::BoneMap::size_type CoreSkeleton::boneCount() const
    {
        return m_allBones.size();
    }

    CoreSkeleton::RootBoneNames::size_type CoreSkeleton::rootBoneCount() const
    {
        return m_rootBoneNames.size();
    }

    //////////////
    // iterator //
    //////////////

    CoreSkeleton::iterator::iterator(BoneMap::iterator me)
        : myIter(me)
    { }

    CoreSkeleton::iterator::iterator()
    { }

    CoreSkeleton::iterator::~iterator()
    { }

    bool CoreSkeleton::iterator::operator==(iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreSkeleton::iterator::operator!=(iterator other) const
    {
        return myIter != other.myIter;
    }

    CoreSkeleton::iterator& CoreSkeleton::iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreSkeleton::iterator CoreSkeleton::iterator::operator++(int)
    {
        return iterator(myIter++);
    }

    CoreSkeleton::iterator& CoreSkeleton::iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreSkeleton::iterator CoreSkeleton::iterator::operator--(int)
    {
        return iterator(myIter--);
    }

    CoreBonePtr CoreSkeleton::iterator::operator*()
    {
        return *myIter;
    }

    CoreBone* CoreSkeleton::iterator::operator->()
    {
        return myIter->operator->();
    }

    CoreSkeleton::iterator CoreSkeleton::begin()
    {
        return iterator(m_allBones.begin());
    }

    CoreSkeleton::iterator CoreSkeleton::end()
    {
        return iterator(m_allBones.end());
    }

    ////////////////////
    // const_iterator //
    ////////////////////

    CoreSkeleton::const_iterator::const_iterator(BoneMap::const_iterator me)
        : myIter(me)
    { }

    CoreSkeleton::const_iterator::const_iterator()
    { }

    CoreSkeleton::const_iterator::~const_iterator()
    { }

    bool CoreSkeleton::const_iterator::operator==(const_iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreSkeleton::const_iterator::operator!=(const_iterator other) const
    {
        return myIter != other.myIter;
    }


    CoreSkeleton::const_iterator& CoreSkeleton::const_iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreSkeleton::const_iterator CoreSkeleton::const_iterator::operator++(int)
    {
        return const_iterator(myIter++);
    }

    CoreSkeleton::const_iterator& CoreSkeleton::const_iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreSkeleton::const_iterator CoreSkeleton::const_iterator::operator--(int)
    {
        return const_iterator(myIter--);
    }

    CoreBonePtrC CoreSkeleton::const_iterator::operator*() const
    {
        return (*myIter);
    }

    const CoreBone* CoreSkeleton::const_iterator::operator->() const
    {
        return myIter->operator->();
    }

    CoreSkeleton::const_iterator CoreSkeleton::begin() const
    {
        return const_iterator(m_allBones.begin());
    }

    CoreSkeleton::const_iterator CoreSkeleton::end() const
    {
        return const_iterator(m_allBones.end());
    }

    ///////////////////
    // root_iterator //
    ///////////////////

    CoreSkeleton::root_iterator::root_iterator(CoreSkeleton& me, RootBoneNames::iterator iter)
        : me(me)
        , myIter(iter)
    { }

    CoreSkeleton::root_iterator::~root_iterator()
    { }

    bool CoreSkeleton::root_iterator::operator==(root_iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreSkeleton::root_iterator::operator!=(root_iterator other) const
    {
        return myIter != other.myIter;
    }

    CoreSkeleton::root_iterator& CoreSkeleton::root_iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreSkeleton::root_iterator CoreSkeleton::root_iterator::operator++(int)
    {
        return root_iterator(me, myIter++);
    }

    CoreSkeleton::root_iterator& CoreSkeleton::root_iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreSkeleton::root_iterator CoreSkeleton::root_iterator::operator--(int)
    {
        return root_iterator(me, myIter--);
    }

    CoreBonePtr CoreSkeleton::root_iterator::operator*()
    {
        return me.m_allBones[*myIter];
    }

    CoreBone* CoreSkeleton::root_iterator::operator->()
    {
        return me.m_allBones[*myIter].operator->();
    }

    CoreSkeleton::root_iterator CoreSkeleton::begin_root()
    {
        return root_iterator(*this, m_rootBoneNames.begin());
    }

    CoreSkeleton::root_iterator CoreSkeleton::end_root()
    {
        return root_iterator(*this, m_rootBoneNames.end());
    }

    /////////////////////////
    // const_root_iterator //
    /////////////////////////

    CoreSkeleton::const_root_iterator::const_root_iterator(const CoreSkeleton& me, RootBoneNames::const_iterator iter)
        : me(me)
        , myIter(iter)
    { }

    CoreSkeleton::const_root_iterator::~const_root_iterator()
    { }

    bool CoreSkeleton::const_root_iterator::operator==(const_root_iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreSkeleton::const_root_iterator::operator!=(const_root_iterator other) const
    {
        return myIter != other.myIter;
    }

    CoreSkeleton::const_root_iterator& CoreSkeleton::const_root_iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreSkeleton::const_root_iterator CoreSkeleton::const_root_iterator::operator++(int)
    {
        return const_root_iterator(me, myIter++);
    }

    CoreSkeleton::const_root_iterator& CoreSkeleton::const_root_iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreSkeleton::const_root_iterator CoreSkeleton::const_root_iterator::operator--(int)
    {
        return const_root_iterator(me, myIter--);
    }

    CoreBonePtrC CoreSkeleton::const_root_iterator::operator*() const
    {
        return me.m_allBones.at(*myIter);
    }

    const CoreBone* CoreSkeleton::const_root_iterator::operator->() const
    {
        return me.m_allBones.at(*myIter).operator->();
    }

    CoreSkeleton::const_root_iterator CoreSkeleton::begin_root() const
    {
        return const_root_iterator(*this, m_rootBoneNames.begin());
    }

    CoreSkeleton::const_root_iterator CoreSkeleton::end_root() const
    {
        return const_root_iterator(*this, m_rootBoneNames.end());
    }

}
