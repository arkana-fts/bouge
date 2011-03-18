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
#include <bouge/Vertex.hpp>
#include <stdexcept>

namespace bouge {

    Influence::Influence(float w, const std::string& sBoneName)
        : w(w)
        , sBoneName(sBoneName)
    { }

    Vertex::Vertex(const Vector& pos)
        : m_pos(pos)
    { }

    Vertex::~Vertex()
    { }

    Vector Vertex::pos() const
    {
        return m_pos;
    }

    Vertex& Vertex::pos(const Vector& pos)
    {
        m_pos = pos;
        return *this;
    }

    std::size_t Vertex::influenceCount() const
    {
        return m_influences.size();
    }

    Influence Vertex::influence(std::size_t idx) const
    {
        if(idx >= this->influenceCount())
            throw std::out_of_range("Vertex influence");

        return m_influences[idx];
    }

    Vertex& Vertex::influence(std::size_t idx, const Influence& influence)
    {
        if(idx >= this->influenceCount())
            throw std::out_of_range("Vertex influence");

        m_influences[idx] = influence;
        return *this;
    }

    Vertex& Vertex::addInfluence(const Influence& influence)
    {
        m_influences.push_back(influence);
        return *this;
    }

    std::vector<float> Vertex::attrib(std::string name) const
    {
        GenericAttribs::const_iterator i = m_genericAttributes.find(name);
        if(i == m_genericAttributes.end())
            throw std::invalid_argument("Generic vertex attribute " + name + " not present");

        return i->second;
    }

    Vertex& Vertex::attrib(std::string name, const std::vector<float>& attrib)
    {
        m_genericAttributes[name] = attrib;
        return *this;
    }

    bool Vertex::hasAttrib(std::string name) const
    {
        return m_genericAttributes.find(name) != m_genericAttributes.end();
    }

    std::size_t Vertex::attribCount() const
    {
        return m_genericAttributes.size();
    }

    Vertex& Vertex::deleteAttrib(std::string name)
    {
        m_genericAttributes.erase(name);
        return *this;
    }

    Vertex::iterator::iterator(GenericAttribs::iterator me)
        : myIter(me)
    { }

    Vertex::iterator::iterator()
    { }

    Vertex::iterator::~iterator()
    { }

    bool Vertex::iterator::operator==(iterator other) const
    {
        return myIter == other.myIter;
    }

    bool Vertex::iterator::operator!=(iterator other) const
    {
        return myIter != other.myIter;
    }

    Vertex::iterator& Vertex::iterator::operator++()
    {
        return ++myIter, *this;
    }

    Vertex::iterator Vertex::iterator::operator++(int)
    {
        return iterator(myIter++);
    }

    Vertex::iterator& Vertex::iterator::operator--()
    {
        return --myIter, *this;
    }

    Vertex::iterator Vertex::iterator::operator--(int)
    {
        return iterator(myIter--);
    }

    std::string Vertex::iterator::name() const
    {
        return myIter->first;
    }

    std::vector<float>& Vertex::iterator::value()
    {
        return myIter->second;
    }

    Vertex::iterator Vertex::begin()
    {
        return iterator(m_genericAttributes.begin());
    }

    Vertex::iterator Vertex::end()
    {
        return iterator(m_genericAttributes.end());
    }

    Vertex::const_iterator::const_iterator(GenericAttribs::const_iterator me)
        : myIter(me)
    { }

    Vertex::const_iterator::const_iterator()
    { }

    Vertex::const_iterator::~const_iterator()
    { }

    bool Vertex::const_iterator::operator==(const_iterator other) const
    {
        return myIter == other.myIter;
    }

    bool Vertex::const_iterator::operator!=(const_iterator other) const
    {
        return myIter != other.myIter;
    }

    Vertex::const_iterator& Vertex::const_iterator::operator++()
    {
        return ++myIter, *this;
    }

    Vertex::const_iterator Vertex::const_iterator::operator++(int)
    {
        return const_iterator(myIter++);
    }

    Vertex::const_iterator& Vertex::const_iterator::operator--()
    {
        return --myIter, *this;
    }

    Vertex::const_iterator Vertex::const_iterator::operator--(int)
    {
        return const_iterator(myIter--);
    }

    std::string Vertex::const_iterator::name() const
    {
        return myIter->first;
    }

    const std::vector<float>& Vertex::const_iterator::value() const
    {
        return myIter->second;
    }

    Vertex::const_iterator Vertex::begin() const
    {
        return const_iterator(m_genericAttributes.begin());
    }

    Vertex::const_iterator Vertex::end() const
    {
        return const_iterator(m_genericAttributes.end());
    }

} // namespace bouge
