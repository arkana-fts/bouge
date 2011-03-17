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
#include <bouge/Face.hpp>
#include <bouge/Util.hpp>

#include <stdexcept>

namespace bouge {

    Face::Face(const std::vector<index_t>& vertexIndices)
        : m_vertexIndices(vertexIndices)
    {
    }

    Face::Face(index_t v1, index_t v2)
    {
        m_vertexIndices.push_back(v1);
        m_vertexIndices.push_back(v2);
    }

    Face::Face(index_t v1, index_t v2, index_t v3)
    {
        m_vertexIndices.push_back(v1);
        m_vertexIndices.push_back(v2);
        m_vertexIndices.push_back(v3);
    }

    Face::Face(index_t v1, index_t v2, index_t v3, index_t v4)
    {
        m_vertexIndices.push_back(v1);
        m_vertexIndices.push_back(v2);
        m_vertexIndices.push_back(v3);
        m_vertexIndices.push_back(v4);
    }

    bool Face::isTriangle() const
    {
        return m_vertexIndices.size() == 3;
    }

    Face::index_t Face::idx1() const
    {
        if(m_vertexIndices.size() < 1)
            throw std::out_of_range("Face index 1, but only " + to_s(m_vertexIndices.size()) + " indices available");

        return m_vertexIndices[0];
    }

    Face::index_t Face::idx2() const
    {
        if(m_vertexIndices.size() < 2)
            throw std::out_of_range("Face index 2, but only " + to_s(m_vertexIndices.size()) + " indices available");

        return m_vertexIndices[1];
    }

    Face::index_t Face::idx3() const
    {
        if(m_vertexIndices.size() < 3)
            throw std::out_of_range("Face index 3, but only " + to_s(m_vertexIndices.size()) + " indices available");

        return m_vertexIndices[2];
    }

    Face::index_t Face::idx4() const
    {
        if(m_vertexIndices.size() < 4)
            throw std::out_of_range("Face index 4, but only " + to_s(m_vertexIndices.size()) + " indices available");

        return m_vertexIndices[3];
    }

    const std::vector<Face::index_t>& Face::idxs() const
    {
        return m_vertexIndices;
    }

} // namespace bouge
