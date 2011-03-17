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
#ifndef BOUGE_FACE_HPP
#define BOUGE_FACE_HPP

#include <bouge/bougefwd.hpp>

#include <vector>

namespace bouge {

    /// This class defines a face in bouge, in an abstract way. That is it does
    /// not need to be a triangle, it could be a line or a quad or whatever else
    /// too. Note, though, that it is definitely smarter to use only triangles.
    class BOUGE_API Face
    {
    public:
        typedef std::size_t index_t;

        BOUGE_USER_DATA;

        /// Creates an arbitrary polygon (aka N-Gon) using the given vertex indices.
        /// \param vertexIndices The indices of the vertexes to use.
        Face(const std::vector<index_t>& vertexIndices);

        /// Creates a line using the given vertex indices.
        /// \param vertexIndex1 The index of the first vertex of the line.
        /// \param vertexIndex2 The index of the second vertex of the line.
        Face(index_t vertexIndex1, index_t vertexIndex2);

        /// Creates a triangle using the given vertex indices.
        /// \param vertexIndex1 The index of the first vertex of the triangle.
        /// \param vertexIndex2 The index of the second vertex of the triangle.
        /// \param vertexIndex3 The index of the third vertex of the triangle.
        Face(index_t vertexIndex1, index_t vertexIndex2, index_t vertexIndex3);

        /// Creates a quad using the given vertex indices.
        /// \param vertexIndex1 The index of the first vertex of the quad.
        /// \param vertexIndex2 The index of the second vertex of the quad.
        /// \param vertexIndex3 The index of the third vertex of the quad.
        /// \param vertexIndex4 The index of the fourth vertex of the quad.
        Face(index_t vertexIndex1, index_t vertexIndex2, index_t vertexIndex3, index_t vertexIndex4);

        /// \return true if this face is a triangle, false else.
        bool isTriangle() const;

        index_t idx1() const;
        index_t idx2() const;
        index_t idx3() const;
        index_t idx4() const;
        const std::vector<index_t>& idxs() const;

    private:
        std::vector<index_t> m_vertexIndices; ///< The indices of the vertexes that make up this face.
    };

} // namespace bouge

#endif // BOUGE_FACE_HPP
