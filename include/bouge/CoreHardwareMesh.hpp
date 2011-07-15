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
#ifndef BOUGE_COREHARDWAREMESH_H
#define BOUGE_COREHARDWAREMESH_H

#include <bouge/bougefwd.hpp>

#include <vector>
#include <map>
#include <string>

namespace bouge {

    class BOUGE_API CoreHardwareSubMesh
    {
    public:
        CoreHardwareSubMesh(std::size_t startIdx, std::string submeshName);
        virtual ~CoreHardwareSubMesh();

        BOUGE_USER_DATA;

        /// \return The name of the original submesh this hardware submesh comes from.
        std::string submeshName() const;

        /// \return The number of faces this sub-mesh has.
        std::size_t faceCount() const;

        /// \return The index into the face indices buffer at which the first
        ///         face of this submesh is located.
        std::size_t startIndex() const;

        std::size_t boneCount() const;
        bool hasBone(const std::string& name) const;
        unsigned int boneId(const std::string& name) const;
        std::string boneName(std::size_t idInSubmesh) const;

        bool canAddFace(Face f, CoreSubMeshPtrC coreSubMesh, unsigned int bonesPerMesh) const;
        void addFace(Face f, CoreSubMeshPtrC coreSubMesh);

    private:
        std::map<std::string, unsigned int> m_boneNameToIdInSubmesh;
        std::vector<std::string> m_boneIdInSubmeshToName;
        std::size_t m_faceCount;
        std::size_t m_startIdx;
        std::string m_submeshName;
    };

    class BOUGE_API CoreHardwareMesh
    {
        typedef std::vector<CoreHardwareSubMesh> SubMeshContainer;
        typedef std::map<std::string, std::vector<float> > GenericAttribs;
        typedef std::map<std::string, std::size_t> GenericAttribsCoords;
    public:
        typedef shared_ptr<CoreHardwareMesh>::type Ptr;

        /// \NOTE If you want to create a hardware mesh that doesn't use bones
        ///       (that is, a static hardware mesh), just set both \a bonesPerMesh
        ///       and \a bonesPerVertex to 0. Yes, both. Yes, there is reason for that.
        CoreHardwareMesh(CoreMeshPtrC coremesh, unsigned int bonesPerMesh, unsigned char bonesPerVertex = 4, unsigned char verticesPerFace = 3);
        virtual ~CoreHardwareMesh();

        BOUGE_USER_DATA;

        // Connection to core (hardware?) skeleton?

        SubMeshContainer::size_type submeshCount() const;

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
            CoreHardwareSubMesh& operator*();
            CoreHardwareSubMesh* operator->();
        private:
            friend class CoreHardwareMesh;
            iterator(SubMeshContainer::iterator me);
            SubMeshContainer::iterator myIter;
        };

        iterator begin();
        iterator end();

        class BOUGE_API const_iterator {
        public:
            const_iterator();
            const_iterator(iterator me);
            ~const_iterator();

            bool operator==(const_iterator other) const;
            bool operator!=(const_iterator other) const;
            const_iterator& operator++();
            const_iterator operator++(int);
            const_iterator& operator--();
            const_iterator operator--(int);
            const CoreHardwareSubMesh& operator*() const;
            const CoreHardwareSubMesh* operator->() const;
        private:
            friend class CoreHardwareMesh;
            const_iterator(SubMeshContainer::const_iterator me);
            SubMeshContainer::const_iterator myIter;
        };

        const_iterator begin() const;
        const_iterator end() const;

        // Getters and setters

        /// \return The number of vertices there are in this mesh.
        std::size_t vertexCount() const;

        /// \return The number of faces there are in this mesh.
        std::size_t faceCount() const;

        /// \return The number of coordinates each vertex has. (Usually 3)
        std::size_t coordsPerVertex() const;

        /// Get a copy of the coordinates of the vertex at the specified index.
        /// \param idx The (0-based) number (index) of the vertex whose coordinates to get.
        /// \return A copy of that vertex's coordinates. Modifying the copy won't modify the original.
        /// \exception std::out_of_range in case there is no vertex at the specified index.
        Vector coord(std::size_t idx) const;

        /// Modify the coordinates of the vertex at the specified index.
        /// \param idx The (0-based) number (index) of the vertex whose coordinates to modify.
        /// \param vtx The new value to assign to that vertex's coordinates.
        /// \return A reference to the current object for chaining operation.
        /// \exception std::out_of_range in case there is no vertex at the specified index.
        CoreHardwareMesh& coord(std::size_t idx, const Vector& vtx);

        /// Get the whole list of vertex coordinates (can be modified).
        /// \return A reference to the whole list of raw vertex coordinates.
        std::vector<float>& coords();

        /// Get the whole list of read-only vertex coordinates. Should be sent to the graphics card.
        /// \return A read-only reference to the whole list of raw vertex coordinates.
        const std::vector<float>& coords() const;

        /// Writes the vertex coordinates into a buffer.
        /// \param where The buffer to write the vertex coordinates to.
        /// \param stride The stride, in bytes. That means that between one vertex's coordinates
        ///               and the next vertex's coordinates, there are \a stride bytes.
        ///               It's actually exactly the same as in OpenGL.
        /// \return A reference to the current object for chaining operation.
        const CoreHardwareMesh& writeCoords(float* where, std::size_t stride) const;

        /// \return A map mapping each generic attribute's name to the number
        ///         of coordinates that attrib holds per vertex.
        /// \note You can safely use this one inside a loop, the returned
        ///       reference will not just die. It will die along with this object.
        const GenericAttribsCoords& attribs() const;

        /// \return The number of \a name coordinates each vertex has. (Usually 3 or 4)
        /// \exception std::invalid_argument in case there is no such attribute.
        std::size_t attribCoordsPerVertex(std::string name) const;

        /// Checks if the attribute \a name is present in the hardware mesh and submeshes.
        /// \param name The name of the attribute to check presence for.
        /// \return true if the attribute is present.
        bool hasAttrib(std::string name) const;

        /// Get a copy of an attribute of the vertex at the specified index.
        /// \param name The name of the attribute you want to get.
        /// \param idx The (0-based) number (index) of the vertex whose attribute to get.
        /// \return A copy of that vertex's attribute. Modifying the copy won't modify the original.
        /// \exception std::out_of_range in case there is no vertex at the specified index.
        /// \exception std::invalid_argument in case there is no such attribute.
        std::vector<float> attrib(std::string name, std::size_t idx) const;

        /// Modify an attribute of the vertex at the specified index.
        /// \param name The name of the attribute you want to set.
        /// \param idx The (0-based) number (index) of the vertex whose attribute to modify.
        /// \param nor The new value to assign to that vertex's attribute.
        /// \return A reference to the current object for chaining operation.
        /// \exception std::out_of_range in case there is no vertex at the specified index.
        /// \exception std::invalid_argument in case there is no such attribute.
        CoreHardwareMesh& attrib(std::string name, std::size_t idx, const std::vector<float>& attrib);

        /// Get the whole list of a vertex attribute's coordinates (can be modified).
        /// \param name The name of the attribute you want to get.
        /// \return A reference to the whole list of raw vertex attribute's coordinates.
        /// \exception std::invalid_argument in case there is no such attribute.
        std::vector<float>& attrib(std::string name);

        /// Get the whole list of read-only vertex attribute's coordinates. Should be sent to the graphics card.
        /// \param name The name of the attribute you want to get.
        /// \return A read-only reference to the whole list of raw vertex attribute's coordinates.
        /// \exception std::invalid_argument in case there is no such attribute.
        const std::vector<float>& attrib(std::string name) const;

        /// Writes the vertex attribute's coordinates into a buffer.
        /// \param name The name of the attribute you want to write.
        /// \param where The buffer to write the vertex attribute's coordinates to.
        /// \param stride The stride, in bytes. That means that between one vertex's attribute
        ///               and the next vertex's attribute, there are \a stride bytes.
        ///               It's actually exactly the same as in OpenGL.
        /// \return A reference to the current object for chaining operation.
        /// \exception std::invalid_argument in case there is no such attribute.
        const CoreHardwareMesh& writeAttrib(std::string name, float* where, std::size_t stride) const;

        /// \return The number of weights each vertex has. (Usually 4)
        std::size_t weightsPerVertex() const;

        /// Get a copy of the weights of the vertex at the specified index.
        /// \param idx The (0-based) number (index) of the vertex whose weights to get.
        /// \return A copy of that vertex's weights. Modifying the copy won't modify the original.
        /// \exception std::out_of_range in case there is no vertex at the specified index.
        std::vector<float> weights(std::size_t idx) const;

        /// Modify the weights of the vertex at the specified index.
        /// \param idx The (0-based) number (index) of the vertex whose weights to modify.
        /// \param weights The new value to assign to that vertex's weights.
        /// \return A reference to the current object for chaining operation.
        /// \exception std::out_of_range in case there is no vertex at the specified index.
        CoreHardwareMesh& weights(std::size_t idx, const std::vector<float>& weights);

        /// Get the whole list of vertex weights (can be modified).
        /// \return A reference to the whole list of raw vertex weights.
        std::vector<float>& weights();

        /// Get the whole list of read-only vertex weights. Should be sent to the graphics card.
        /// \return A read-only reference to the whole list of raw vertex weights.
        const std::vector<float>& weights() const;

        /// Writes the vertex weights into a buffer.
        /// \param where The buffer to write the vertex weights to.
        /// \param stride The stride, in bytes. That means that between one vertex's weights
        ///               and the next vertex's weights, there are \a stride bytes.
        ///               It's actually exactly the same as in OpenGL.
        /// \return A reference to the current object for chaining operation.
        const CoreHardwareMesh& writeWeights(float* where, std::size_t stride) const;

        /// \return The number of bones influencing each vertex. (Usually 4)
        std::size_t boneIndicesPerVertex() const;

        /// Get a copy of the indices of the bones influencing the vertex at the specified index.
        /// \param idx The (0-based) number (index) of the vertex whose bone's indices to get.
        /// \return A copy of that vertex's bone's indices. Modifying the copy won't modify the original.
        /// \exception std::out_of_range in case there is no vertex at the specified index.
        std::vector<float> boneIndices(std::size_t idx) const;

        /// Modify the bones influencing the vertex at the specified index.
        /// \param idx The (0-based) number (index) of the vertex whose influencing bone indices to modify.
        /// \param indices The new bone indices to assign to that vertex.
        /// \return A reference to the current object for chaining operation.
        /// \exception std::out_of_range in case there is no vertex at the specified index.
        CoreHardwareMesh& boneIndices(std::size_t idx, const std::vector<float>& indices);

        /// Get the whole list of vertex bone indices (can be modified).
        /// \return A reference to the whole list of raw vertex bone indices.
        std::vector<float>& boneIndices();

        /// Get the whole list of read-only vertex bone indices. Should be sent to the graphics card.
        /// \return A read-only reference to the whole list of raw vertex bone indices.
        const std::vector<float>& boneIndices() const;

        /// Writes the vertex bone indices into a buffer.
        /// \param where The buffer to write the vertex bone indices to.
        /// \param stride The stride, in bytes. That means that between one vertex's bone indices
        ///               and the next vertex's bone indices, there are \a stride bytes.
        ///               It's actually exactly the same as in OpenGL.
        /// \return A reference to the current object for chaining operation.
        const CoreHardwareMesh& writeBoneIndices(float* where, std::size_t stride) const;

        /// \return The number of indices each face has. (Usually 3: triangles)
        std::size_t indicesPerFace() const;

        /// Get a copy of the indices of the face at the specified index.
        /// \param idx The (0-based) number (index) of the face whose indices to get.
        /// \return A copy of that face's indices. Modifying the copy won't modify the original.
        /// \exception std::out_of_range in case there is no face at the specified index.
        std::vector<BOUGE_FACE_INDEX_TYPE> faceIndices(std::size_t idx) const;

        /// Modify the indices of the face at the specified index.
        /// \param idx The (0-based) number (index) of the face whose indices to modify.
        /// \param indices The new value to assign to that face's indices.
        /// \return A reference to the current object for chaining operation.
        /// \exception std::out_of_range in case there is no face at the specified index.
        CoreHardwareMesh& faceIndices(std::size_t idx, const std::vector<BOUGE_FACE_INDEX_TYPE>& indices);

        /// Get the whole list of face indices (can be modified).
        /// \return A reference to the whole list of raw face indices.
        std::vector<BOUGE_FACE_INDEX_TYPE>& faceIndices();

        /// Get the whole list of read-only face indices. Should be sent to the graphics card.
        /// \return A read-only reference to the whole list of raw face indices.
        const std::vector<BOUGE_FACE_INDEX_TYPE>& faceIndices() const;

        /// Writes the face indices into a buffer.
        /// \param where The buffer to write the face indices to.
        /// \param stride The stride, in bytes. That means that between one face's indices
        ///               and the next face's indices, there are \a stride bytes.
        ///               It's actually exactly the same as in OpenGL.
        /// \return A reference to the current object for chaining operation.
        const CoreHardwareMesh& writeFaceIndices(BOUGE_FACE_INDEX_TYPE* where, std::size_t stride) const;

    protected:
        /// \internal
        static std::size_t totalDifferentBoneCount(const Face& f, CoreSubMeshPtrC submesh);

        /// \internal
        void addVertex(Vertex vtx, unsigned char bonesPerVertex, const std::string& submeshDiagnosticName);

    private:
        /// We might need to split this mesh into submeshes. Here they are.
        SubMeshContainer m_submeshes;

        /// All vertex coordinates of the whole mesh.
        std::vector<float> m_verts;

        /// All weights of the whole mesh. There are \a m_weightsPerVertex entries per vertex :)
        std::vector<float> m_weights;

        /// For each weight, this is the index of the bone palette it belongs to.
        std::vector<float> m_boneIndices;

        /// All the generic attributes of the vertices.
        GenericAttribs m_genericAttribs;

        /// The number of floats each generic attribute holds.
        GenericAttribsCoords m_genericAttribCoordCount;

        /// These are all the vertex indices of the triangle faces.
        std::vector<BOUGE_FACE_INDEX_TYPE> m_faceIndices;

        /// The amount of weights (bone influences) each vertex has.
        unsigned int m_weightsPerVertex;

        /// The amount of vertices each face has.
        unsigned int m_verticesPerFace;
    };

} // namespace bouge

#endif // BOUGE_COREHARDWAREMESH_H
