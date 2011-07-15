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
#include <bouge/CoreMesh.hpp>
#include <bouge/Util.hpp>

#include <stdexcept>

namespace bouge
{

    CoreSubMesh::CoreSubMesh(std::string name)
        : m_sName(name)
        , m_bAllFacesAreTriangles(true)
    { }

    CoreSubMesh::~CoreSubMesh()
    { }

    std::string CoreSubMesh::name() const
    {
        return m_sName;
    }

    CoreSubMesh& CoreSubMesh::name(std::string name)
    {
        m_sName = name;
        return *this;
    }

    std::size_t CoreSubMesh::vertexCount() const
    {
        return m_verts.size();
    }

    std::size_t CoreSubMesh::faceCount() const
    {
        return m_faces.size();
    }

    CoreSubMesh& CoreSubMesh::addVertex(Vertex vert)
    {
        m_verts.push_back(vert);
        return *this;
    }

    Vertex CoreSubMesh::vertex(std::size_t idx) const
    {
        if(idx >= this->vertexCount())
            throw std::out_of_range("Get vertex " + to_s(idx) + " (max: " + to_s(this->vertexCount()) + ") in core mesh");

        return m_verts[idx];
    }

    CoreSubMesh& CoreSubMesh::vertex(std::size_t idx, Vertex vtx)
    {
        if(idx >= this->vertexCount())
            throw std::out_of_range("Set vertex " + to_s(idx) + " (max: " + to_s(this->vertexCount()) + ") in core mesh");

        m_verts[idx] = vtx;
        return *this;
    }

    bool CoreSubMesh::canAddFace(const Face& face, Face::index_t* out_pFailing) const
    {
        for(std::vector<Face::index_t>::const_iterator i = face.idxs().begin() ; i != face.idxs().end() ; ++i) {
            if(*i >= m_verts.size()) {
                if(out_pFailing) {
                    *out_pFailing = *i;
                }
                return false;
            }
        }

        return true;
    }

    CoreSubMesh& CoreSubMesh::addFace(Face face)
    {
        Face::index_t failing;
        if(!this->canAddFace(face, &failing))
            throw std::invalid_argument("Face containing a too big vertex index (" + to_s(failing) + ", max: " + to_s(this->vertexCount()) + ")");

        m_faces.push_back(face);

        // Now check if still all faces are polys.
        if(!face.isTriangle())
            m_bAllFacesAreTriangles = false;

        return *this;
    }

    Face CoreSubMesh::face(std::size_t idx) const
    {
        if(idx >= this->faceCount())
            throw std::out_of_range("Get face " + to_s(idx) + " (max: " + to_s(this->faceCount()) + ") in core mesh");

        return m_faces[idx];
    }

    CoreSubMesh& CoreSubMesh::face(std::size_t idx, Face face)
    {
        if(idx >= this->faceCount())
            throw std::out_of_range("Set face " + to_s(idx) + " (max: " + to_s(this->faceCount()) + ") in core mesh");

        m_faces[idx] = face;
        return *this;
    }

    bool CoreSubMesh::allFacesAreTriangles() const
    {
        return m_bAllFacesAreTriangles;
    }

    CoreMesh::CoreMesh(std::string name)
        : m_sName(name)
    { }

    CoreMesh::~CoreMesh()
    { }

    std::string CoreMesh::name() const
    {
        return m_sName;
    }

    CoreMesh& CoreMesh::name(std::string name)
    {
        m_sName = name;
        return *this;
    }

    std::size_t CoreMesh::vertexCount() const
    {
        std::size_t total = 0;
        for(const_iterator i = this->begin() ; i != this->end() ; ++i) {
            total += i->vertexCount();
        }
        return total;
    }

    std::size_t CoreMesh::faceCount() const
    {
        std::size_t total = 0;
        for(const_iterator i = this->begin() ; i != this->end() ; ++i) {
            total += i->faceCount();
        }
        return total;
    }

    std::size_t CoreMesh::submeshCount() const
    {
        return m_submeshes.size();
    }

    bool CoreMesh::allFacesAreTriangles() const
    {
        for(const_iterator i = this->begin() ; i != this->end() ; ++i) {
            // As soon as one isn't tri, all can't be anymore!
            if(!i->allFacesAreTriangles())
                return false;
        }

        // None weren't -> all are.
        return true;
    }

    CoreMesh& CoreMesh::add(CoreSubMeshPtr submesh)
    {
        m_submeshes[submesh->name()] = submesh;
        return *this;
    }

    bool CoreMesh::hasSubmesh(const std::string& name) const
    {
        return m_submeshes.find(name) != m_submeshes.end();
    }

    CoreSubMeshPtr CoreMesh::submesh(const std::string& name)
    {
        SubMeshMap::iterator i = m_submeshes.find(name);

        if(i == m_submeshes.end())
            throw std::invalid_argument("There is no submesh " + name + " in the mesh " + m_sName);

        return i->second;
    }

    CoreSubMeshPtrC CoreMesh::submesh(const std::string& name) const
    {
        SubMeshMap::const_iterator i = m_submeshes.find(name);

        if(i == m_submeshes.end())
            throw std::invalid_argument("There is no submesh " + name + " in the mesh " + m_sName);

        return i->second;
    }

    CoreMesh::iterator::iterator(SubMeshMap::iterator me)
        : myIter(me)
    { }

    CoreMesh::iterator::iterator()
    { }

    CoreMesh::iterator::~iterator()
    { }

    bool CoreMesh::iterator::operator==(iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreMesh::iterator::operator!=(iterator other) const
    {
        return myIter != other.myIter;
    }

    CoreMesh::iterator& CoreMesh::iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreMesh::iterator CoreMesh::iterator::operator++(int)
    {
        return iterator(myIter++);
    }

    CoreMesh::iterator& CoreMesh::iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreMesh::iterator CoreMesh::iterator::operator--(int)
    {
        return iterator(myIter--);
    }

    CoreSubMeshPtr CoreMesh::iterator::operator*()
    {
        return myIter->second;
    }

    CoreSubMesh* CoreMesh::iterator::operator->()
    {
        return myIter->second.operator->();
    }

    CoreMesh::iterator CoreMesh::begin()
    {
        return iterator(m_submeshes.begin());
    }

    CoreMesh::iterator CoreMesh::end()
    {
        return iterator(m_submeshes.end());
    }

    CoreMesh::const_iterator::const_iterator(SubMeshMap::const_iterator me)
        : myIter(me)
    { }

    CoreMesh::const_iterator::const_iterator()
    { }

    CoreMesh::const_iterator::~const_iterator()
    { }

    bool CoreMesh::const_iterator::operator==(const_iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreMesh::const_iterator::operator!=(const_iterator other) const
    {
        return myIter != other.myIter;
    }

    CoreMesh::const_iterator& CoreMesh::const_iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreMesh::const_iterator CoreMesh::const_iterator::operator++(int)
    {
        return const_iterator(myIter++);
    }

    CoreMesh::const_iterator& CoreMesh::const_iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreMesh::const_iterator CoreMesh::const_iterator::operator--(int)
    {
        return const_iterator(myIter--);
    }

    CoreSubMeshPtrC CoreMesh::const_iterator::operator*() const
    {
        return myIter->second;
    }

    const CoreSubMesh* CoreMesh::const_iterator::operator->() const
    {
        return myIter->second.operator->();
    }

    CoreMesh::const_iterator CoreMesh::begin() const
    {
        return const_iterator(m_submeshes.begin());
    }

    CoreMesh::const_iterator CoreMesh::end() const
    {
        return const_iterator(m_submeshes.end());
    }

} // namespace bouge
