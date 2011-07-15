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
#include <bouge/CoreHardwareMesh.hpp>
#include <bouge/CoreMesh.hpp>
#include <bouge/Face.hpp>
#include <bouge/Util.hpp>

#include <stdexcept>
#include <set>
#include <vector>
#include <bouge/Exception.hpp>

namespace bouge {

    CoreHardwareSubMesh::CoreHardwareSubMesh(std::size_t startIdx, std::string submeshName)
        : m_faceCount(0)
        , m_startIdx(startIdx)
        , m_submeshName(submeshName)
    { }

    CoreHardwareSubMesh::~CoreHardwareSubMesh()
    { }

    std::string CoreHardwareSubMesh::submeshName() const
    {
        return m_submeshName;
    }

    size_t CoreHardwareSubMesh::faceCount() const
    {
        return m_faceCount;
    }

    std::size_t CoreHardwareSubMesh::startIndex() const
    {
        return m_startIdx;
    }

    std::size_t CoreHardwareSubMesh::boneCount() const
    {
        return m_boneIdInSubmeshToName.size();
    }

    bool CoreHardwareSubMesh::hasBone(const std::string& name) const
    {
        return m_boneNameToIdInSubmesh.find(name) != m_boneNameToIdInSubmesh.end();
    }

    unsigned int CoreHardwareSubMesh::boneId(const std::string& name) const
    {
        std::map<std::string, unsigned int>::const_iterator i = m_boneNameToIdInSubmesh.find(name);
        if(i == m_boneNameToIdInSubmesh.end())
            throw NotExistException("id of bone", name, "hardware submesh", m_submeshName, __FILE__, __LINE__);

        return i->second;
    }

    std::string CoreHardwareSubMesh::boneName(std::size_t idInSubmesh) const
    {
        if(idInSubmesh >= m_boneIdInSubmeshToName.size())
            throw NotExistException("bone name of bone with id", to_s(idInSubmesh), "hardware submesh", m_submeshName, __FILE__, __LINE__);

        return m_boneIdInSubmeshToName[idInSubmesh];
    }

    bool CoreHardwareSubMesh::canAddFace(Face f, CoreSubMeshPtrC coreSubMesh, unsigned int bonesPerMesh) const
    {
        // Small optimization (leaving out would just needlessly travel the loops)
        if(bonesPerMesh == 0)
            return true;

        // Check if all influences of this face's vertices are already in or at least can still be added.
        for(std::vector<Face::index_t>::const_iterator i = f.idxs().begin() ; i != f.idxs().end() ; ++i) {
            Vertex vtx = coreSubMesh->vertex(*i);
            for(std::size_t i = 0 ; i < vtx.influenceCount() ; ++i) {
                std::string sBoneName = vtx.influence(i).sBoneName;

                // In case the bone is not yet in, and we met our "bone quota", we can't add this face anymore.
                if(!this->hasBone(sBoneName) && m_boneIdInSubmeshToName.size() >= bonesPerMesh)
                    return false;
            }
        }

        // All influences fit in. go on.
        return true;
    }

    void CoreHardwareSubMesh::addFace(Face f, CoreSubMeshPtrC coreSubMesh)
    {
        // Add all the influencing bones to our list, in case they are not used yet.
        for(std::vector<Face::index_t>::const_iterator i = f.idxs().begin() ; i != f.idxs().end() ; ++i) {
            Vertex vtx = coreSubMesh->vertex(*i);
            for(std::size_t i = 0 ; i < vtx.influenceCount() ; ++i) {
                std::string sBoneName = vtx.influence(i).sBoneName;

                // Already in? ignore.
                if(this->hasBone(sBoneName))
                    continue;

                // Not yet in? assign a new ID to the bone name.
                m_boneNameToIdInSubmesh[sBoneName] = m_boneIdInSubmeshToName.size();
                m_boneIdInSubmeshToName.push_back(sBoneName);
            }
        }

        m_faceCount++;
    }

    CoreHardwareMesh::CoreHardwareMesh(CoreMeshPtrC coremesh, unsigned int bonesPerMesh, unsigned char bonesPerVertex, unsigned char verticesPerFace)
        : m_weightsPerVertex(bonesPerVertex)
        , m_verticesPerFace(verticesPerFace)
    {
        // The hardware mesh needs all the vertices to have the same attributes,
        // and all the attributes to have the same amount of coordinates.
        // We need to check for this.
        //
        // Here, we just find _all_ generic attributes that are present in any
        // submesh. If some submesh has less attributes than another one, they
        // will be added to it.
        for(CoreMesh::const_iterator submesh = coremesh->begin() ; submesh != coremesh->end() ; ++submesh) {
            if(submesh->vertexCount() > 0) {
                Vertex vtx = submesh->vertex(0);

                for(Vertex::iterator i = vtx.begin() ; i != vtx.end() ; ++i) {
                    m_genericAttribCoordCount[i.name()] = i.value().size();
                }
            }
        }

        // Now prepare to create the hardware submeshes..
        // Let me briefly explain what this big piece of code (mud?) is about,
        // as it is quite difficult to grasp else.
        //
        // First, the problem: what if a submesh is influenced by, say, 60 bones,
        //    but our shader is only able to handle 20 bones at a time. This
        //    could happen because of hardware limits.
        // Then, the solution: we would need to split this submesh into several
        //    "hardware" submeshes, everyone of those only being influenced by
        //    at most 20 bones. This is what is being done here.
        // Additionally, we want to store all those hardware submeshes' data
        // into one single buffer, to avoid having tons of tiny buffers (bad).
        //
        // One important "detail" is that some vertices will be "at the split",
        // i.e. they will need to be included in both hardware submeshes of a
        // split. The problem is that the bones that influence such a vertex may
        // have completely different IDs in the two hardware submeshes. This
        // means that we need to duplicate such a vertex, so we have two of them.
        // In the end, they will both be transformed in the exact same way, as
        // they are influenced by the exact same bones, just in different
        // hardware submeshes, that is with different IDs.
        //
        // And here a high-level python-style overview of the algorithm:
        //
        // currentVertexId = 0
        // bmap = map FROM bone name TO bone id in hw mesh
        // vmap = map FROM old vertex id in model TO new vertex id in buffer
        // for submesh in model:
        //     hw = new hardware mesh
        //
        //     for face in submesh:
        //         if can't add face to hw:
        //             hw = new hardware mesh
        //             clear bmap, vmap
        //
        //         add face to hw # Also gives the bones used in face their "local" id
        //
        //         for vtxid in face:
        //             if vtxid not in vmap:    # vertex not in data, add!
        //                 for influence in vertex(vtxid):
        //                     boneid = get influence id in hw
        //                     add boneid to data
        //                 add vertex data
        //                 add to vmap: vtxid -> currentVertexId
        //                 currentVertexId += 1
        //
        //             vtxid = vmap[vtxid]      # vertex is in data, use!
        //
        // Hmm I hope this is more or less understandeable. Or at least that I
        // will be able to understand that later on :)
        //
        // The "test/splitting" model is a good test/example for this when setting
        // bones per mesh to two (2).

        // The index at which the FACES of the submesh start.
        std::size_t startIdx = 0;

        // We still take all the submeshes into the same buffers. It's just the
        // material and bone-palette that changes, that is shaders and uniforms.
        // All the vertex attributes are the same, so that's fine.
        for(CoreMesh::const_iterator iSubMesh = coremesh->begin() ; iSubMesh != coremesh->end() ; ++iSubMesh)  {

            // Add face-by-face to a submesh. If the submesh can't take the
            // next face (too many bones or different material), we create a new
            // submesh and start from that one.
            m_submeshes.push_back(CoreHardwareSubMesh(startIdx, iSubMesh->name()));

            // Maps an "original" vertex ID to a "new" vertex ID.
            std::map<Face::index_t, Face::index_t> vtxmap;

            for(std::size_t i = 0 ; i < iSubMesh->faceCount() ; ++i) {
                Face f = iSubMesh->face(i);

                // Skip all non-conforming faces.
                if(f.idxs().size() != verticesPerFace)
                    continue;

                // Also faces that have more different bone influences than we
                // can handle per hardware mesh are bad - they wouldn't fit into
                // a hardware mesh! There's nothing we could do about that.
                // (OK, we could split faces, but wtf...)
                std::size_t bonesInFace = this->totalDifferentBoneCount(f, *iSubMesh);
                if(bonesInFace > bonesPerMesh && bonesPerMesh > 0) {
                    throw BadDataException("We got a face (#" + to_s(i) + ") in the hardware mesh of " + coremesh->name() + " that has more bone influences (" + to_s(bonesInFace) + ") than we can handle in a hardware mesh (" + to_s(bonesPerMesh) + "). Either crank up the bone per hw mesh count or split that face.", __FILE__, __LINE__);
                }

                // Check if it still fits in the current submesh or if we need a new one.
                if(!m_submeshes.back().canAddFace(f, *iSubMesh, bonesPerMesh)) {
                    m_submeshes.push_back(CoreHardwareSubMesh(startIdx, iSubMesh->name()));

                    // When we start a new hardware submesh, we need not to have
                    // references to vertices from other hardware submeshes. We
                    // want them all new (even if copied).
                    vtxmap.clear();
                }

                // Add it to the latest submesh
                m_submeshes.back().addFace(f, *iSubMesh);

                // and then to our own data.
                for(unsigned char i = 0 ; i < verticesPerFace ; ++i) {
                    Face::index_t vtxId = f.idxs()[i];

                    // If we already added this vertex, get its new id:
                    if(vtxmap.find(vtxId) == vtxmap.end()) {
                        // Determine the id the vertex will have when added to
                        // our buffers:
                        Face::index_t effectiveId = this->vertexCount();

                        // Now, before we can add the vertex to our data, we
                        // probably have to adjust its influences ids.
                        // That is, we need to convert the bone Id from a name
                        // (string) to an index in the hardware mesh's bone palette.
                        Vertex vtx = iSubMesh->vertex(vtxId);
                        for(unsigned int iBone = 0 ; iBone < bonesPerVertex ; ++iBone) {
                            if(iBone < vtx.influenceCount()) {
                                // We can assume the submesh has that bone, as we
                                // just added the face a few lines earlier.
                                float boneId = (float)m_submeshes.back().boneId(vtx.influence(iBone).sBoneName);
                                m_boneIndices.push_back(boneId);
                            } else if(vtx.influenceCount() == 0) {
                                // In this case, we add one influence to it.
                                // We need to do that in order not to have all
                                // weights be 0.0 and thus the vertex ending up
                                // in the origin.
                                // Hopefully, ... TODO
                                throw BadDataException("We got a vertex (" + to_s(vtxId) + ") without any influences in submesh " + iSubMesh->name() + ". This is bad, please fix it, for example by creating a 'root bone' which influences it, but doesn't move.", __FILE__, __LINE__);
                                // But we'd have to add that bone to the skeleton ...
                                // How to solve this dilemma?
//                                 vtx.addInfluence(Influence(1.0f, "__bicali__dummy__"));
//                                 m_boneIndices.push_back(0.0f);
                            } else {
                                // In case the bone doesn't have that much bones,
                                // we just add '0' to the indices.
                                m_boneIndices.push_back(0.0f);
                            }
                        }

                        // Now add the vertex to our data:
                        this->addVertex(vtx, bonesPerVertex, iSubMesh->name());

                        // Need to add it to the map, in case another face
                        // references this vertex, so we don't add it twice.
                        vtxmap[vtxId] = effectiveId;
                    }

                    // Now we got the "real", "new" id of this vertex. That is
                    // its effective id in our buffers. Add that to the faces info.
                    m_faceIndices.push_back(vtxmap[vtxId]);
                    startIdx++;
                } // For all vertices of the face.

            } // for all faces

        } // for all submeshes
    }

    std::size_t CoreHardwareMesh::totalDifferentBoneCount(const Face& f, CoreSubMeshPtrC submesh)
    {
        std::set<std::string> influences;
        for(std::vector<Face::index_t>::const_iterator iVertex =  f.idxs().begin() ; iVertex != f.idxs().end() ; ++iVertex) {
            Vertex vtx = submesh->vertex(*iVertex);

            for(std::size_t iInfluence = 0 ; iInfluence < vtx.influenceCount() ; ++iInfluence) {
                influences.insert(vtx.influence(iInfluence).sBoneName);
            }
        }

        return influences.size();
    }

    void CoreHardwareMesh::addVertex(Vertex vtx, unsigned char bonesPerVertex, const std::string& submeshDiagnosticName)
    {
        m_verts.push_back(vtx.pos().x());
        m_verts.push_back(vtx.pos().y());
        m_verts.push_back(vtx.pos().z());

        // Copy the weights of the influences of each vertex.
        // If the vertex has less influences than specified, we add more with weight 0.0.
        // If it has more, we just ignore the redundant ones.
        for(unsigned int i = 0 ; i < bonesPerVertex ; ++i) {
            if(i < vtx.influenceCount())
                m_weights.push_back(vtx.influence(i).w);
            else
                m_weights.push_back(0.0f);
        }

        // Add all the generic attributes. We do several checks to ensure
        // we have the same amount of attributes in each vertex and they
        // have the same dimension across vertices.
        for(GenericAttribsCoords::iterator attribCoordCount = m_genericAttribCoordCount.begin() ; attribCoordCount != m_genericAttribCoordCount.end() ; ++attribCoordCount) {
            if(vtx.hasAttrib(attribCoordCount->first)) {
                std::vector<float> attrib = vtx.attrib(attribCoordCount->first);

                // This means the vertex's attribute has the wrong dimension! STOP HERE!
                if(attrib.size() != attribCoordCount->second)
                    throw BadDataException("Vertex with wrong-sized attribute '" + attribCoordCount->first + "' in submesh '" + submeshDiagnosticName + "' (dimension is " + to_s(attrib.size()) + " but should be " + to_s(attribCoordCount->second) +")", __FILE__, __LINE__);

                // If it got the correct size, copy over all the values.
                for(std::vector<float>::iterator i = attrib.begin() ; i != attrib.end() ; ++i) {
                    m_genericAttribs[attribCoordCount->first].push_back(*i);
                }

            } else {
                // If the vertex doesn't have the attribute, we just throw dummys in.
                for(std::size_t i = 0 ; i < attribCoordCount->second ; ++i) {
                    m_genericAttribs[attribCoordCount->first].push_back(0.0f);
                }
            }
        }
    }

    CoreHardwareMesh::~CoreHardwareMesh()
    { }

    CoreHardwareMesh::SubMeshContainer::size_type CoreHardwareMesh::submeshCount() const
    {
        return m_submeshes.size();
    }

    CoreHardwareMesh::iterator::iterator(SubMeshContainer::iterator me)
        : myIter(me)
    { }

    CoreHardwareMesh::iterator::iterator()
    { }

    CoreHardwareMesh::iterator::~iterator()
    { }

    bool CoreHardwareMesh::iterator::operator==(iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreHardwareMesh::iterator::operator!=(iterator other) const
    {
        return myIter != other.myIter;
    }

    CoreHardwareMesh::iterator& CoreHardwareMesh::iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreHardwareMesh::iterator CoreHardwareMesh::iterator::operator++(int)
    {
        return iterator(myIter++);
    }

    CoreHardwareMesh::iterator& CoreHardwareMesh::iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreHardwareMesh::iterator CoreHardwareMesh::iterator::operator--(int)
    {
        return iterator(myIter--);
    }

    CoreHardwareSubMesh& CoreHardwareMesh::iterator::operator*()
    {
        return *myIter;
    }

    CoreHardwareSubMesh* CoreHardwareMesh::iterator::operator->()
    {
        return myIter.operator->();
    }

    CoreHardwareMesh::iterator CoreHardwareMesh::begin()
    {
        return iterator(m_submeshes.begin());
    }

    CoreHardwareMesh::iterator CoreHardwareMesh::end()
    {
        return iterator(m_submeshes.end());
    }

    CoreHardwareMesh::const_iterator::const_iterator(SubMeshContainer::const_iterator me)
        : myIter(me)
    { }

    CoreHardwareMesh::const_iterator::const_iterator()
    { }

    CoreHardwareMesh::const_iterator::const_iterator(iterator me)
        : myIter(me.myIter)
    { }

    CoreHardwareMesh::const_iterator::~const_iterator()
    { }

    bool CoreHardwareMesh::const_iterator::operator==(const_iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreHardwareMesh::const_iterator::operator!=(const_iterator other) const
    {
        return myIter != other.myIter;
    }

    CoreHardwareMesh::const_iterator& CoreHardwareMesh::const_iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreHardwareMesh::const_iterator CoreHardwareMesh::const_iterator::operator++(int)
    {
        return const_iterator(myIter++);
    }

    CoreHardwareMesh::const_iterator& CoreHardwareMesh::const_iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreHardwareMesh::const_iterator CoreHardwareMesh::const_iterator::operator--(int)
    {
        return const_iterator(myIter--);
    }

    const CoreHardwareSubMesh& CoreHardwareMesh::const_iterator::operator*() const
    {
        return *myIter;
    }

    const CoreHardwareSubMesh* CoreHardwareMesh::const_iterator::operator->() const
    {
        return myIter.operator->();
    }

    CoreHardwareMesh::const_iterator CoreHardwareMesh::begin() const
    {
        return const_iterator(m_submeshes.begin());
    }

    CoreHardwareMesh::const_iterator CoreHardwareMesh::end() const
    {
        return const_iterator(m_submeshes.end());
    }

    std::size_t CoreHardwareMesh::vertexCount() const
    {
        return m_verts.size()/this->coordsPerVertex();
    }

    std::size_t CoreHardwareMesh::faceCount() const
    {
        return m_faceIndices.size()/this->indicesPerFace();
    }

    /// \return The number of coordinates each vertex has. (Usually 3)
    std::size_t CoreHardwareMesh::coordsPerVertex() const
    {
        /// \TODO implement
        return 3;
    }

    Vector CoreHardwareMesh::coord(std::size_t idx) const
    {
        std::size_t iFirstIdx = 3*idx;

        if(iFirstIdx + 3 >= m_verts.size())
            throw std::out_of_range("Vertex " + to_s(idx) + " doesn't exist, there are only " + to_s(this->vertexCount()));

        return Vector(&m_verts[iFirstIdx], &m_verts[iFirstIdx+3]);
    }

    CoreHardwareMesh& CoreHardwareMesh::coord(std::size_t idx, const Vector& vtx)
    {
        std::size_t iFirstIdx = 3*idx;

        if(iFirstIdx + 3 >= m_verts.size())
            throw std::out_of_range("Vertex " + to_s(idx) + " doesn't exist, there are only " + to_s(this->vertexCount()));

        m_verts[iFirstIdx+0] = vtx.x();
        m_verts[iFirstIdx+1] = vtx.y();
        m_verts[iFirstIdx+2] = vtx.z();

        return *this;
    }

    std::vector<float>& CoreHardwareMesh::coords()
    {
        return m_verts;
    }

    const std::vector<float>& CoreHardwareMesh::coords() const
    {
        return m_verts;
    }

    const CoreHardwareMesh& CoreHardwareMesh::writeCoords(float* where, std::size_t stride) const
    {
        // There is a little pointer magic involved to get the stride right...
        char* p = (char*)where;
        for(std::vector<float>::const_iterator iVert = m_verts.begin() ; iVert != m_verts.end() ; ) {
            // For every vertex, we write its X coordinates straight one after the other.
            for(std::size_t jVertCoord = 0 ; jVertCoord < this->coordsPerVertex() && iVert != m_verts.end() ; ++jVertCoord) {
                ((float*)p)[jVertCoord] = *iVert;
                ++iVert;
            }

            // Then, we skip "stride" bytes.
            p += stride;
        }
        return *this;
    }

    const CoreHardwareMesh::GenericAttribsCoords& CoreHardwareMesh::attribs() const
    {
        return m_genericAttribCoordCount;
    }

    std::size_t CoreHardwareMesh::attribCoordsPerVertex(std::string name) const
    {
        GenericAttribsCoords::const_iterator i = m_genericAttribCoordCount.find(name);
        if(i == m_genericAttribCoordCount.end())
            throw std::invalid_argument("No generic attribute '" + name + "' in the mesh");

        return i->second;
    }

    bool CoreHardwareMesh::hasAttrib(std::string name) const
    {
        return m_genericAttribs.find(name) != m_genericAttribs.end() && m_genericAttribCoordCount.find(name) != m_genericAttribCoordCount.end();
    }

    std::vector<float> CoreHardwareMesh::attrib(std::string name, std::size_t idx) const
    {
        GenericAttribs::const_iterator iAttrib = m_genericAttribs.find(name);
        GenericAttribsCoords::const_iterator iAttribCoordCount = m_genericAttribCoordCount.find(name);
        if(iAttrib == m_genericAttribs.end() || iAttribCoordCount == m_genericAttribCoordCount.end())
            throw std::invalid_argument("No generic attribute '" + name + "' in the mesh");

        std::size_t iAttribSize = iAttribCoordCount->second;
        std::size_t iFirstIdx = iAttribSize*idx;

        if(iFirstIdx + iAttribSize >= iAttrib->second.size())
            throw std::out_of_range("Vertex " + to_s(idx) + " doesn't exist, there are only " + to_s(this->vertexCount()));

        std::vector<float> ret;
        for(std::size_t i = 0 ; i < iAttribSize ; ++i) {
            ret.push_back(iAttrib->second[iFirstIdx+i]);
        }

        return ret;
    }

    CoreHardwareMesh& CoreHardwareMesh::attrib(std::string name, std::size_t idx, const std::vector<float>& attrib)
    {
        GenericAttribs::iterator iAttrib = m_genericAttribs.find(name);
        if(iAttrib == m_genericAttribs.end())
            throw std::invalid_argument("No generic attribute '" + name + "' in the mesh");

        std::size_t iAttribSize = this->attribCoordsPerVertex(name);
        std::size_t iFirstIdx = iAttribSize*idx;

        if(iFirstIdx + iAttribSize >= iAttrib->second.size())
            throw std::out_of_range("Vertex " + to_s(idx) + " doesn't exist, there are only " + to_s(this->vertexCount()));

        for(std::size_t i = 0 ; i < iAttribSize ; ++i) {
            iAttrib->second[iFirstIdx+i] = attrib[i];
        }

        return *this;
    }

    std::vector<float>& CoreHardwareMesh::attrib(std::string name)
    {
        GenericAttribs::iterator i = m_genericAttribs.find(name);
        if(i == m_genericAttribs.end())
            throw std::invalid_argument("No generic attribute '" + name + "' in the mesh");

        return i->second;
    }

    const std::vector<float>& CoreHardwareMesh::attrib(std::string name) const
    {
        GenericAttribs::const_iterator i = m_genericAttribs.find(name);
        if(i == m_genericAttribs.end())
            throw std::invalid_argument("No generic attribute '" + name + "' in the mesh");

        return i->second;
    }

    const CoreHardwareMesh& CoreHardwareMesh::writeAttrib(std::string name, float* where, std::size_t stride) const
    {
        GenericAttribs::const_iterator iAttrib = m_genericAttribs.find(name);
        GenericAttribsCoords::const_iterator iAttribCoordCount = m_genericAttribCoordCount.find(name);
        if(iAttrib == m_genericAttribs.end() || iAttribCoordCount == m_genericAttribCoordCount.end())
            throw std::invalid_argument("No generic attribute '" + name + "' in the mesh");

        // There is a little pointer magic involved to get the stride right...
        char* p = (char*)where;
        for(std::vector<float>::const_iterator iVert = iAttrib->second.begin() ; iVert != iAttrib->second.end() ; ) {
            // For every vertex, we write its X normal coordinates straight one after the other.
            for(std::size_t jAttribCoord = 0 ; jAttribCoord < iAttribCoordCount->second && iVert != iAttrib->second.end() ; ++jAttribCoord) {
                ((float*)p)[jAttribCoord] = *iVert;
                ++iVert;
            }

            // Then, we skip "stride" bytes.
            p += stride;
        }
        return *this;
    }

    std::size_t CoreHardwareMesh::weightsPerVertex() const
    {
        return m_weightsPerVertex;
    }

    std::vector<float> CoreHardwareMesh::weights(std::size_t idx) const
    {
        std::size_t startIdx = idx * this->weightsPerVertex();
        std::size_t endIdx = startIdx + this->weightsPerVertex() - 1;

        if(endIdx >= m_weights.size()) {
            throw std::out_of_range("Get vertex weights " + to_s(idx) + " (max: " + to_s(this->vertexCount()) + ") in core hardware mesh");
        }

        std::vector<float> ret(this->weightsPerVertex());
        std::copy(&m_weights[startIdx], &m_weights[endIdx], ret.begin());
        return ret;
    }

    CoreHardwareMesh& CoreHardwareMesh::weights(std::size_t idx, const std::vector<float>& weights)
    {
        std::size_t iFirstIdx = this->weightsPerVertex()*idx;

        if(iFirstIdx + this->weightsPerVertex() >= m_weights.size())
            throw std::out_of_range("Vertex " + to_s(idx) + " doesn't exist, there are only " + to_s(this->vertexCount()));

        for(std::size_t i = 0 ; i < this->weightsPerVertex() && i < weights.size() ; ++i) {
            m_weights[iFirstIdx+i] = weights[i];
        }

        return *this;
    }

    std::vector<float>& CoreHardwareMesh::weights()
    {
        return m_weights;
    }

    const std::vector<float>& CoreHardwareMesh::weights() const
    {
        return m_weights;
    }

    const CoreHardwareMesh& CoreHardwareMesh::writeWeights(float* where, std::size_t stride) const
    {
        // There is a little pointer magic involved to get the stride right...
        char* p = (char*)where;
        for(std::vector<float>::const_iterator iVert = m_weights.begin() ; iVert != m_weights.end() ; ) {
            // For every vertex, we write its X weights straight one after the other.
            for(std::size_t jVertWeight = 0 ; jVertWeight < this->weightsPerVertex() && iVert != m_weights.end() ; ++jVertWeight) {
                ((float*)p)[jVertWeight] = *iVert;
                ++iVert;
            }

            // Then, we skip "stride" bytes.
            p += stride;
        }
        return *this;
    }

    std::size_t CoreHardwareMesh::boneIndicesPerVertex() const
    {
        return m_weightsPerVertex;
    }

    std::vector<float> CoreHardwareMesh::boneIndices(std::size_t idx) const
    {
        std::size_t startIdx = idx * this->boneIndicesPerVertex();
        std::size_t endIdx = startIdx + this->boneIndicesPerVertex() - 1;

        if(endIdx >= m_boneIndices.size()) {
            throw std::out_of_range("Get vertex bone indices " + to_s(idx) + " (max: " + to_s(this->vertexCount()) + ") in core hardware mesh");
        }

        std::vector<float> ret(this->boneIndicesPerVertex());
        std::copy(&m_boneIndices[startIdx], &m_boneIndices[endIdx], ret.begin());
        return ret;
    }

    CoreHardwareMesh& CoreHardwareMesh::boneIndices(std::size_t idx, const std::vector<float>& indices)
    {
        std::size_t iFirstIdx = this->boneIndicesPerVertex()*idx;

        if(iFirstIdx + this->boneIndicesPerVertex() >= m_boneIndices.size())
            throw std::out_of_range("Vertex " + to_s(idx) + " doesn't exist, there are only " + to_s(this->vertexCount()));

        for(std::size_t i = 0 ; i < this->boneIndicesPerVertex() && i < indices.size() ; ++i) {
            m_boneIndices[iFirstIdx+i] = indices[i];
        }

        return *this;
    }

    std::vector<float>& CoreHardwareMesh::boneIndices()
    {
        return m_boneIndices;
    }

    const std::vector<float>& CoreHardwareMesh::boneIndices() const
    {
        return m_boneIndices;
    }

    const CoreHardwareMesh& CoreHardwareMesh::writeBoneIndices(float* where, std::size_t stride) const
    {
        // There is a little pointer magic involved to get the stride right...
        char* p = (char*)where;
        for(std::vector<float>::const_iterator iVert = m_boneIndices.begin() ; iVert != m_boneIndices.end() ; ) {
            // For every vertex, we write its X weights straight one after the other.
            for(std::size_t jVertBoneIndex = 0 ; jVertBoneIndex < this->boneIndicesPerVertex() && iVert != m_boneIndices.end() ; ++jVertBoneIndex) {
                ((float*)p)[jVertBoneIndex] = *iVert;
                ++iVert;
            }

            // Then, we skip "stride" bytes.
            p += stride;
        }
        return *this;
    }

    std::size_t CoreHardwareMesh::indicesPerFace() const
    {
        return m_verticesPerFace;
    }

    std::vector<BOUGE_FACE_INDEX_TYPE> CoreHardwareMesh::faceIndices(std::size_t idx) const
    {
        std::size_t startIdx = idx * this->indicesPerFace();
        std::size_t endIdx = startIdx + this->indicesPerFace() - 1;

        if(endIdx >= m_faceIndices.size()) {
            throw std::out_of_range("Get face indices " + to_s(idx) + " (max: " + to_s(this->faceCount()) + ") in core hardware mesh");
        }

        std::vector<BOUGE_FACE_INDEX_TYPE> ret(this->indicesPerFace());
        std::copy(&m_faceIndices[startIdx], &m_faceIndices[endIdx], ret.begin());
        return ret;
    }

    CoreHardwareMesh& CoreHardwareMesh::faceIndices(std::size_t idx, const std::vector<BOUGE_FACE_INDEX_TYPE>& indices)
    {
        std::size_t iFirstIdx = this->indicesPerFace()*idx;

        if(iFirstIdx + this->indicesPerFace() >= m_faceIndices.size())
            throw std::out_of_range("Face " + to_s(idx) + " doesn't exist, there are only " + to_s(this->faceCount()));

        for(std::size_t i = 0 ; i < this->indicesPerFace() && i < indices.size() ; ++i) {
            m_faceIndices[iFirstIdx+i] = indices[i];
        }

        return *this;
    }

    std::vector<BOUGE_FACE_INDEX_TYPE>& CoreHardwareMesh::faceIndices()
    {
        return m_faceIndices;
    }

    const std::vector<BOUGE_FACE_INDEX_TYPE>& CoreHardwareMesh::faceIndices() const
    {
        return m_faceIndices;
    }

    const CoreHardwareMesh& CoreHardwareMesh::writeFaceIndices(BOUGE_FACE_INDEX_TYPE* where, std::size_t stride) const
    {
        // There is a little pointer magic involved to get the stride right...
        char* p = (char*)where;
        for(std::vector<BOUGE_FACE_INDEX_TYPE>::const_iterator iFace = m_faceIndices.begin() ; iFace != m_faceIndices.end() ; ++iFace) {
            // For every face, we write its X indices straight one after the other.
            for(std::size_t jFaceIndex = 0 ; jFaceIndex < this->indicesPerFace() ; ++jFaceIndex) {
                ((BOUGE_FACE_INDEX_TYPE*)p)[jFaceIndex] = *iFace;
                ++iFace;
            }

            // Then, we skip "stride" bytes.
            p += stride;
        }
        return *this;
    }

} // namespace bouge
