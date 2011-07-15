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
#ifndef BOUGE_COREMESH_H
#define BOUGE_COREMESH_H

#include <bouge/bougefwd.hpp>
#include <bouge/Vertex.hpp>
#include <bouge/Face.hpp>

#include <vector>

namespace bouge {

    /// The core submesh class holds all "mesh" information of a part of a model
    /// that shares a common material, that is all its vertices and faces and
    /// some more properties. You should not use it to render your model,
    /// it is only useful for higher-level processing.
    /// To render your model, create a CoreHardwareMesh.
    class BOUGE_API CoreSubMesh
    {
    public:
        CoreSubMesh(std::string name);
        virtual ~CoreSubMesh();

        BOUGE_USER_DATA;

        std::string name() const;
        CoreSubMesh& name(std::string name);

        std::size_t vertexCount() const;
        std::size_t faceCount() const;

        CoreSubMesh& addVertex(Vertex vert);
        Vertex vertex(std::size_t idx) const;
        CoreSubMesh& vertex(std::size_t idx, Vertex vtx);

        bool canAddFace(const Face& face, Face::index_t* out_pFailing = 0) const;
        CoreSubMesh& addFace(Face face);
        Face face(std::size_t idx) const;
        CoreSubMesh& face(std::size_t idx, Face vtx);

        bool allFacesAreTriangles() const;

    private:
        std::string m_sName;

        std::vector<Vertex> m_verts;
        std::vector<Face> m_faces;

        bool m_bAllFacesAreTriangles;
    };

    /// The core mesh class in fact is just a container of \a CoreSubMesh objects.
    /// These objects are the real "parts" of the mesh. The whole mesh is divided
    /// into a submesh per material. You should not use it to render your model,
    /// it is only useful for higher-level processing.
    /// To render your model, create a CoreHardwareMesh.
    class BOUGE_API CoreMesh
    {
        typedef std::map<std::string, CoreSubMeshPtr> SubMeshMap;
    public:
        CoreMesh(std::string name);
        virtual ~CoreMesh();

        BOUGE_USER_DATA;

        std::string name() const;
        CoreMesh& name(std::string name);

        std::size_t vertexCount() const;
        std::size_t faceCount() const;
        bool allFacesAreTriangles() const;

        CoreMesh& add(CoreSubMeshPtr submesh);
        std::size_t submeshCount() const;
        bool hasSubmesh(const std::string& name) const;
        CoreSubMeshPtr submesh(const std::string& name);
        CoreSubMeshPtrC submesh(const std::string& name) const;

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
            CoreSubMeshPtr operator*();
            CoreSubMesh* operator->();
        private:
            friend class CoreMesh;
            iterator(SubMeshMap::iterator me);
            SubMeshMap::iterator myIter;
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
            CoreSubMeshPtrC operator*() const;
            const CoreSubMesh* operator->() const;
        private:
            friend class CoreMesh;
            const_iterator(SubMeshMap::const_iterator me);
            SubMeshMap::const_iterator myIter;
        };

        const_iterator begin() const;
        const_iterator end() const;

    private:
        std::string m_sName;

        SubMeshMap m_submeshes;
    };

} // namespace bouge

#endif // BOUGE_COREMESH_H
