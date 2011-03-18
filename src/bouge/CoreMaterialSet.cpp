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
#include <bouge/CoreMaterialSet.hpp>

#include <stdexcept>

namespace bouge
{

    CoreMaterialSet::CoreMaterialSet(std::string name)
        : m_sName(name)
    { }

    CoreMaterialSet::~CoreMaterialSet()
    { }

    std::string CoreMaterialSet::name() const
    {
        return m_sName;
    }

    CoreMaterialSet& CoreMaterialSet::name(std::string name)
    {
        m_sName = name;
        return *this;
    }

    std::string CoreMaterialSet::materialForMesh(const std::string& meshname) const
    {
        AssossiationsMap::const_iterator i = m_sAssossiations.find(meshname);
        if(i == m_sAssossiations.end())
            throw std::invalid_argument("No assossiation for mesh " + meshname + " in material set " + m_sName);

        return i->second;
    }

    CoreMaterialSet& CoreMaterialSet::materialForMesh(std::string meshname, std::string matname)
    {
        m_sAssossiations[meshname] = matname;
        return *this;
    }

    CoreMaterialSet::iterator::iterator(AssossiationsMap::iterator me)
        : myIter(me)
    { }

    CoreMaterialSet::iterator::iterator()
    { }

    CoreMaterialSet::iterator::~iterator()
    { }

    bool CoreMaterialSet::iterator::operator==(iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreMaterialSet::iterator::operator!=(iterator other) const
    {
        return myIter != other.myIter;
    }

    CoreMaterialSet::iterator& CoreMaterialSet::iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreMaterialSet::iterator CoreMaterialSet::iterator::operator++(int)
    {
        return iterator(myIter++);
    }

    CoreMaterialSet::iterator& CoreMaterialSet::iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreMaterialSet::iterator CoreMaterialSet::iterator::operator--(int)
    {
        return iterator(myIter--);
    }

    std::string CoreMaterialSet::iterator::meshname() const
    {
        return myIter->first;
    }

    std::string CoreMaterialSet::iterator::matname() const
    {
        return myIter->second;
    }

    void CoreMaterialSet::iterator::matname(std::string value)
    {
        myIter->second = value;
    }

    CoreMaterialSet::iterator CoreMaterialSet::begin()
    {
        return iterator(m_sAssossiations.begin());
    }

    CoreMaterialSet::iterator CoreMaterialSet::end()
    {
        return iterator(m_sAssossiations.end());
    }

    CoreMaterialSet::const_iterator::const_iterator(AssossiationsMap::const_iterator me)
        : myIter(me)
    { }

    CoreMaterialSet::const_iterator::const_iterator()
    { }

    CoreMaterialSet::const_iterator::~const_iterator()
    { }

    bool CoreMaterialSet::const_iterator::operator==(const_iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreMaterialSet::const_iterator::operator!=(const_iterator other) const
    {
        return myIter != other.myIter;
    }

    CoreMaterialSet::const_iterator& CoreMaterialSet::const_iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreMaterialSet::const_iterator CoreMaterialSet::const_iterator::operator++(int)
    {
        return const_iterator(myIter++);
    }

    CoreMaterialSet::const_iterator& CoreMaterialSet::const_iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreMaterialSet::const_iterator CoreMaterialSet::const_iterator::operator--(int)
    {
        return const_iterator(myIter--);
    }

    std::string CoreMaterialSet::const_iterator::meshname() const
    {
        return myIter->first;
    }

    std::string CoreMaterialSet::const_iterator::matname() const
    {
        return myIter->second;
    }

    CoreMaterialSet::const_iterator CoreMaterialSet::begin() const
    {
        return const_iterator(m_sAssossiations.begin());
    }

    CoreMaterialSet::const_iterator CoreMaterialSet::end() const
    {
        return const_iterator(m_sAssossiations.end());
    }

} // namespace bouge
