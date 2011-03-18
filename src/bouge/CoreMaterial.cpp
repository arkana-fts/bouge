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
#include <bouge/CoreMaterial.hpp>

#include <stdexcept>
#include <sstream>
#include <bouge/Util.hpp>

namespace bouge
{
    CoreMaterial::PropValue::PropValue(std::string val)
        : m_val(val)
    { }

    const std::string& CoreMaterial::PropValue::val() const
    {
        return m_val;
    }

    CoreMaterial::PropValue& CoreMaterial::PropValue::val(std::string val)
    {
        m_val = val;
        m_valAsFvecCache.clear(); // Invalidates the cache.
        return *this;
    }

    const std::vector<float>& CoreMaterial::PropValue::valAsFvec() const
    {
        // Need to construct the cache for the first time since the last set.
        if(m_valAsFvecCache.empty()) {
            m_valAsFvecCache = to< std::vector<float> >(this->val());
        }

        return m_valAsFvecCache;
    }

    CoreMaterial::CoreMaterial(std::string name)
        : m_sName(name)
    { }

    CoreMaterial::~CoreMaterial()
    { }

    std::string CoreMaterial::name() const
    {
        return m_sName;
    }

    CoreMaterial& CoreMaterial::name(std::string name)
    {
        m_sName = name;
        return *this;
    }

    bool CoreMaterial::hasProprety(const std::string& name) const
    {
        return m_propreties.find(name) != m_propreties.end();
    }

    const std::string& CoreMaterial::proprety(const std::string& name) const
    {
        static std::string emptystr;
        PropretyMap::const_iterator i = m_propreties.find(name);
        return i != m_propreties.end() ? i->second.val() : emptystr;
    }

    const std::vector<float>& CoreMaterial::propretyAsFvec(const std::string& name) const
    {
        PropretyMap::const_iterator i = m_propreties.find(name);
        if(i == m_propreties.end())
            throw std::invalid_argument("No proprety named " + name + " found in the material " + m_sName);

        return i->second.valAsFvec();
    }

    CoreMaterial& CoreMaterial::proprety(const std::string& name, const std::string value)
    {
        m_propreties[name].val(value);
        return *this;
    }

    CoreMaterial& CoreMaterial::proprety(const std::string& name, const std::vector<float> value)
    {
        m_propreties[name].val(to_s(value));
        return *this;
    }

    CoreMaterial::iterator::iterator(PropretyMap::iterator me)
        : myIter(me)
    { }

    CoreMaterial::iterator::iterator()
    { }

    CoreMaterial::iterator::~iterator()
    { }

    bool CoreMaterial::iterator::operator==(iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreMaterial::iterator::operator!=(iterator other) const
    {
        return myIter != other.myIter;
    }

    CoreMaterial::iterator& CoreMaterial::iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreMaterial::iterator CoreMaterial::iterator::operator++(int)
    {
        return iterator(myIter++);
    }

    CoreMaterial::iterator& CoreMaterial::iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreMaterial::iterator CoreMaterial::iterator::operator--(int)
    {
        return iterator(myIter--);
    }

    const std::string& CoreMaterial::iterator::name() const
    {
        return myIter->first;
    }

    const std::string& CoreMaterial::iterator::value() const
    {
        return myIter->second.val();
    }

    const std::vector<float>& CoreMaterial::iterator::valueAsFvec() const
    {
        return myIter->second.valAsFvec();
    }

    void CoreMaterial::iterator::value(std::string value)
    {
        myIter->second.val(value);
    }

    void CoreMaterial::iterator::value(std::vector<float> value)
    {
        this->value(to_s(value));
    }

    CoreMaterial::iterator CoreMaterial::begin()
    {
        return iterator(m_propreties.begin());
    }

    CoreMaterial::iterator CoreMaterial::end()
    {
        return iterator(m_propreties.end());
    }

    CoreMaterial::const_iterator::const_iterator(PropretyMap::const_iterator me)
        : myIter(me)
    { }

    CoreMaterial::const_iterator::const_iterator()
    { }

    CoreMaterial::const_iterator::~const_iterator()
    { }

    bool CoreMaterial::const_iterator::operator==(const_iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreMaterial::const_iterator::operator!=(const_iterator other) const
    {
        return myIter != other.myIter;
    }

    CoreMaterial::const_iterator& CoreMaterial::const_iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreMaterial::const_iterator CoreMaterial::const_iterator::operator++(int)
    {
        return const_iterator(myIter++);
    }

    CoreMaterial::const_iterator& CoreMaterial::const_iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreMaterial::const_iterator CoreMaterial::const_iterator::operator--(int)
    {
        return const_iterator(myIter--);
    }

    const std::string& CoreMaterial::const_iterator::name() const
    {
        return myIter->first;
    }

    const std::string& CoreMaterial::const_iterator::value() const
    {
        return myIter->second.val();
    }

    const std::vector<float>& CoreMaterial::const_iterator::valueAsFvec() const
    {
        return myIter->second.valAsFvec();
    }

    CoreMaterial::const_iterator CoreMaterial::begin() const
    {
        return const_iterator(m_propreties.begin());
    }

    CoreMaterial::const_iterator CoreMaterial::end() const
    {
        return const_iterator(m_propreties.end());
    }

} // namespace bouge
