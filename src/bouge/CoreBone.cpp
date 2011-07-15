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
#include <bouge/CoreBone.hpp>

#include <stdexcept>

namespace bouge
{

    CoreBone::CoreBone(std::string name, Vector relativeRootPosition, Quaternion relativeBoneRotation, float length)
        : m_sName(name)
        , m_relativeRootPosition(relativeRootPosition)
        , m_relativeBoneRotation(relativeBoneRotation)
        , m_boneLength(length)
    {
        this->updateAbsolute();
    }

    CoreBone::~CoreBone()
    { }

    CoreBone& CoreBone::addChild(CoreBonePtr child)
    {
        m_children[child->name()] = child;
        return *this;

        // This won't work here as we'll may end up having two shared pointers on this.
//         child->m_pParent = CoreBonePtr(this);

        // Done in setParent. Here it is useless as child may have no parent yet.
//         child->updateAbsoluteRecursive();
//         child->updateCacheRecursive();
    }

    CoreBone& CoreBone::removeChild(const std::string& name)
    {
        ChildrenMap::iterator child = m_children.find(name);
        if(child == m_children.end())
            return *this;

        // He becomes an orphan now!
        child->second->m_pParent.reset();

        m_children.erase(name);
        return *this;
    }

    std::size_t CoreBone::childCount() const
    {
        return m_children.size();
    }

    bool CoreBone::hasChild(const std::string& name) const
    {
        return m_children.find(name) != m_children.end();
    }

    CoreBonePtrC CoreBone::child(const std::string& name) const
    {
        ChildrenMap::const_iterator i = m_children.find(name);
        if(i == m_children.end())
            throw std::logic_error("Trying to get the inexistent child named " + name + " of the bone " + m_sName);

        return i->second;
    }

    CoreBonePtr CoreBone::child(const std::string& name)
    {
        ChildrenMap::iterator i = m_children.find(name);
        if(i == m_children.end())
            throw std::logic_error("Trying to get the inexistent child named " + name + " of the bone " + m_sName);

        return i->second;
    }

    bool CoreBone::hasParent() const
    {
        return m_pParent.get() != 0;
    }

    CoreBonePtrC CoreBone::parent() const
    {
        if(!this->hasParent())
            throw std::logic_error("Trying to get the parent of the root bone " + m_sName);

        return m_pParent;
    }

    CoreBonePtr CoreBone::parent()
    {
        if(!this->hasParent())
            throw std::logic_error("Trying to get the parent of the root bone " + m_sName);

        return m_pParent;
    }

    CoreBone& CoreBone::parent(CoreBonePtr parent)
    {
        m_pParent = parent;

        // Gotta update all my matrices now.
        this->updateAbsoluteRecursive();
        return *this;
    }

    std::string CoreBone::name() const
    {
        return m_sName;
    }

    CoreBone& CoreBone::name(std::string name)
    {
        // We also need to rename it in our parent.
        if(m_pParent) {
            m_pParent->removeChild(m_sName);
            m_pParent->addChild(CoreBonePtr(this));
        }

        // Finally, we can rename ourselves.
        m_sName = name;
        return *this;
    }

    BoneId CoreBone::id() const
    {
        return m_id;
    }

    CoreBone& CoreBone::id(BoneId id)
    {
        m_id = id;
        return *this;
    }

    float CoreBone::length() const
    {
        return m_boneLength;
    }

    CoreBone& CoreBone::length(float len)
    {
        m_boneLength = len;
        return *this;
    }

    Vector CoreBone::relativeRootPosition() const
    {
        return m_relativeRootPosition;
    }

    Quaternion CoreBone::relativeBoneRotation() const
    {
        return m_relativeBoneRotation;
    }

    Vector CoreBone::absoluteRootPosition() const
    {
        return m_absoluteRootPosition;
    }

    Quaternion CoreBone::absoluteBoneRotation() const
    {
        return m_absoluteBoneRotation;
    }

    AffineMatrix CoreBone::modelSpaceToBoneSpaceMatrix() const
    {
        return m_mBoneSpaceToModelSpace.inverse();
    }

    AffineMatrix CoreBone::boneSpaceToModelSpaceMatrix() const
    {
        return m_mBoneSpaceToModelSpace;
    }

    void CoreBone::updateAbsolute()
    {
        if(m_pParent) {
            m_absoluteBoneRotation = m_pParent->m_absoluteBoneRotation * m_relativeBoneRotation;
            m_absoluteRootPosition = m_pParent->m_absoluteRootPosition + m_pParent->m_absoluteBoneRotation.rotate(m_relativeRootPosition);
            m_mBoneSpaceToModelSpace = m_pParent->m_mBoneSpaceToModelSpace * AffineMatrix::transformation(this->relativeRootPosition(), this->relativeBoneRotation());
        } else {
            m_absoluteBoneRotation = m_relativeBoneRotation;
            m_absoluteRootPosition = m_relativeRootPosition;
            m_mBoneSpaceToModelSpace.setTransformation(this->relativeRootPosition(), this->relativeBoneRotation());
        }
    }

    void CoreBone::updateAbsoluteRecursive()
    {
        this->updateAbsolute();

        for(ChildrenMap::iterator i = m_children.begin() ; i != m_children.end() ; ++i) {
            i->second->updateAbsoluteRecursive();
        }
    }

    CoreBone::iterator::iterator(ChildrenMap::iterator me)
        : myIter(me)
    { }

    CoreBone::iterator::iterator()
    { }

    CoreBone::iterator::~iterator()
    { }

    bool CoreBone::iterator::operator==(iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreBone::iterator::operator!=(iterator other) const
    {
        return myIter != other.myIter;
    }

    CoreBone::iterator& CoreBone::iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreBone::iterator CoreBone::iterator::operator++(int)
    {
        return iterator(myIter++);
    }

    CoreBone::iterator& CoreBone::iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreBone::iterator CoreBone::iterator::operator--(int)
    {
        return iterator(myIter--);
    }

    CoreBonePtr CoreBone::iterator::operator*()
    {
        return myIter->second;
    }

    CoreBone* CoreBone::iterator::operator->()
    {
        return myIter->second.operator->();
    }

    CoreBone::iterator CoreBone::begin()
    {
        return iterator(m_children.begin());
    }

    CoreBone::iterator CoreBone::end()
    {
        return iterator(m_children.end());
    }

    CoreBone::const_iterator::const_iterator(ChildrenMap::const_iterator me)
        : myIter(me)
    { }

    CoreBone::const_iterator::const_iterator()
    { }

    CoreBone::const_iterator::~const_iterator()
    { }

    bool CoreBone::const_iterator::operator==(const_iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreBone::const_iterator::operator!=(const_iterator other) const
    {
        return myIter != other.myIter;
    }

    CoreBone::const_iterator& CoreBone::const_iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreBone::const_iterator CoreBone::const_iterator::operator++(int)
    {
        return const_iterator(myIter++);
    }

    CoreBone::const_iterator& CoreBone::const_iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreBone::const_iterator CoreBone::const_iterator::operator--(int)
    {
        return const_iterator(myIter--);
    }

    CoreBonePtrC CoreBone::const_iterator::operator*() const
    {
        return myIter->second;
    }

    const CoreBone* CoreBone::const_iterator::operator->() const
    {
        return myIter->second.operator->();
    }

    CoreBone::const_iterator CoreBone::begin() const
    {
        return const_iterator(m_children.begin());
    }

    CoreBone::const_iterator CoreBone::end() const
    {
        return const_iterator(m_children.end());
    }

}
