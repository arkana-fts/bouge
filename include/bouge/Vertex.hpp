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
#ifndef BOUGE_VERTEX_H
#define BOUGE_VERTEX_H

#include <bouge/bougefwd.hpp>
#include <bouge/Math/Vector.hpp>

#include <string>
#include <vector>
#include <map>

namespace bouge {

    /// This class represents a vertex influence, that is how much the
    /// vertex "belongs to" or "follows" a given bone.
    struct BOUGE_API Influence
    {
        float w; ///< The weight of this bone.
        std::string sBoneName; ///< The name of the bone influencing the vertex.

        BOUGE_USER_DATA;

        /// Creates a new influence.
        /// \param w The weight value.
        /// \param sBoneName The name of the bone influencing the vertex.
        Influence(float w, const std::string& sBoneName);
    };

    /// This class represents a Vertex and holds all necessary per-vertex states.
    class BOUGE_API Vertex
    {
        typedef std::map<std::string, std::vector<float> > GenericAttribs;
    public:
        /// Create a vertex with specified coordinates.
        /// \param pos The coordinates of the vertex.
        Vertex(const Vector& pos);

        /// Default destructor.
        virtual ~Vertex();

        BOUGE_USER_DATA;

        /// Get a copy of the vertex's coordinates.
        /// \return A copy of this vertex's coordinates. Modifying the copy won't modify the vertex.
        Vector pos() const;

        /// Modify the coordinates of the vertex.
        /// \param pos The new value to assign to this vertex's coordinates.
        /// \return A reference to the current object for chaining operation.

        Vertex& pos(const Vector& pos);

        /// Get the amount of influences this vertex has.
        /// \return The amount of influences this vertex has.
        std::size_t influenceCount() const;

        /// Get a copy of one of the vertex's influences.
        /// \return A copy of this vertex's \a idx th influence. Modifying the copy won't modify the vertex.
        /// \param idx The (0-based) index of the influence to get.
        /// \exception std::out_of_range in case there is no influence at the specified index.
        Influence influence(std::size_t idx) const;

        /// Modify one of the vertex's influences.
        /// \param idx The (0-based) index of the influence to modify.
        /// \param influence The new value to assign to this vertex's \a idx th influence.
        /// \return A reference to the current object for chaining operation.
        /// \exception std::out_of_range in case there is no influence at the specified index.
        Vertex& influence(std::size_t idx, const Influence& influence);

        /// Add a new influence to this vertex.
        /// \param influence The new influence to add to this vertex. It will be added at the end.
        /// \return A reference to the current object for chaining operation.
        Vertex& addInfluence(const Influence& influence);

        /// Get a copy of a vertex's attribute.
        /// \param name The name of the attribute you need a copy of.
        /// \return A copy of this vertex's attribute called \a name. Modifying the copy won't modify the vertex!
        /// \exception std::invalid_argument in case there is no attribute with that name.
        std::vector<float> attrib(std::string name) const;

        /// Modify an attribute of the vertex.
        /// \param name The name of the attribute you need to modify.
        /// \param attrib The new value to assign to this vertex's attribute.
        /// \return A reference to the current object for chaining operation.
        /// \note If an attribute with that name doesn't exist yet, it is created.
        Vertex& attrib(std::string name, const std::vector<float>& attrib);

        /// Verify if there is an attribute called \a name.
        /// \param name The name of the attribute you need to check for.
        /// \return True if an attribute with such a name exists, false if not.
        bool hasAttrib(std::string name) const;

        /// \return The number of generic attributes this vertex has.
        std::size_t attribCount() const;

        /// Deletes an attribute of this vertex.
        /// \param name The name of the attribute you want to delete.
        /// \return A reference to the current object for chaining operation.
        /// \note If such an attribute doesn't exist, this is a no-op.
        Vertex& deleteAttrib(std::string name);

        class BOUGE_API iterator {
        public:
            iterator();
            ~iterator();

            bool operator==(iterator other) const;
            bool operator!=(iterator other) const;
            iterator& operator++();
            iterator operator++(int);
            iterator& operator--();
            iterator operator--(int);

            std::string name() const;
            std::vector<float>& value();
        private:
            friend class Vertex;
            iterator(GenericAttribs::iterator me);
            GenericAttribs::iterator myIter;
        };

        iterator begin();
        iterator end();

        class BOUGE_API const_iterator {
        public:
            const_iterator();
            ~const_iterator();

            bool operator==(const_iterator other) const;
            bool operator!=(const_iterator other) const;
            const_iterator& operator++();
            const_iterator operator++(int);
            const_iterator& operator--();
            const_iterator operator--(int);

            std::string name() const;
            const std::vector<float>& value() const;
        private:
            friend class Vertex;
            const_iterator(GenericAttribs::const_iterator me);
            GenericAttribs::const_iterator myIter;
        };

        const_iterator begin() const;
        const_iterator end() const;

    private:
        Vector m_pos; ///< The vertex's 3D position.
        std::vector<Influence> m_influences; /// The vertex's influences.

        /// Generic vertex attributes, for example color, normal, texco, ...
        GenericAttribs m_genericAttributes;
    };

} // namespace bouge

#endif // BOUGE_VERTEX_H
