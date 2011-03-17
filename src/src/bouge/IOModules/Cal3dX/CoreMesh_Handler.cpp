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
#include <bouge/IOModules/Cal3dX/CoreMesh_Handler.hpp>
#include <bouge/IOModules/Cal3dX/CoreSkeleton_Handler.hpp>
#include <bouge/IOModules/XMLParserCommon/XMLAttributes.hpp>

#include <bouge/CoreMesh.hpp>
#include <bouge/CoreSkeleton.hpp>
#include <bouge/CoreBone.hpp>
#include <bouge/Exception.hpp>
#include <bouge/Util.hpp>

#include <sstream>

namespace bouge {

    CoreMesh_Cal3dXHandler::CoreMesh_Cal3dXHandler(CoreMesh& meshToBuild, CoreSkeletonPtr skelToUse)
        : m_meshToBuild(meshToBuild)
        , m_skelToUse(skelToUse)
        , m_currVert(Vector())
        , m_iCurrTexcoordNumber(0)
        , m_iCurrVertexId(0)
        , m_iCurrSubMeshId(0)
    { }

    CoreMesh_Cal3dXHandler::~CoreMesh_Cal3dXHandler()
    { }

    bool CoreMesh_Cal3dXHandler::wantsToEnter(const std::string& element)
    {
        static const std::string accepted[] = {"MESH", "SUBMESH", "VERTEX", "POS", "NORM", "TEXCOORD", "INFLUENCE", "FACE", ""};

        for(const std::string* i = &accepted[0] ; i->length() > 0 ; ++i) {
            if(element == *i)
                return true;
        }

        return false;
    }

    void CoreMesh_Cal3dXHandler::elementStart(const std::string& element, const XMLAttributes& attributes)
    {
        if(element == "MESH") {
            this->meshStart(attributes);
        } else if(element == "SUBMESH") {
            this->submeshStart(attributes);
        } else if(element == "VERTEX") {
            this->vertStart(attributes);
        } else if(element == "FACE") {
            this->faceStart(attributes);
        } else {
            // Just ignore superfluous entries silently.
        }
    }

    void CoreMesh_Cal3dXHandler::elementEnd(const std::string& element)
    {
        if(element == "MESH") {
            this->meshEnd();
        } else if(element == "SUBMESH") {
            this->submeshEnd();
        } else if(element == "VERTEX") {
            this->vertEnd();
        } else if(element == "FACE") {
            this->faceEnd();
        } else {
            // Just ignore superfluous entries silently.
        }
    }

    void CoreMesh_Cal3dXHandler::textElement(const std::string& element, const XMLAttributes& attributes, const std::string& text)
    {
        std::stringstream ssText(text);

        if(element == "POS") {

            Vector v;
            ssText >> v[0] >> v[1] >> v[2];
            m_currVert.pos(v);

        } else if(element == "NORM") {

            std::vector<float> norm(3);
            ssText >> norm[0] >> norm[1] >> norm[2];
            m_currVert.attrib("normal", norm);

        } else if(element == "TEXCOORD") {

            std::vector<float> texco(2);
            ssText >> texco[0] >> texco[1];
            m_currVert.attrib("texcoord" + to_s(m_iCurrTexcoordNumber), texco);

        } else if(element == "INFLUENCE") {

            if(!attributes.exists("ID"))
                BadDataException("Influence withoud ID", __FILE__, __LINE__);

            // If no skeleton was given, we completely ignore the influences...

            std::string boneName = to_s(attributes.getValueAsInteger("ID"));

            /// \TODO If this is really a performance bottleneck (measure!),
            ///       we could build a cache for it...
            if(m_skelToUse) {
                for(CoreSkeleton::iterator iBone = m_skelToUse->begin() ; iBone !=  m_skelToUse->end() ; ++iBone) {
                    // Find the bone with the given ID and add it to the vertex's influences.
                    Cal3dXBoneIdUD* pBoneId = dynamic_cast<Cal3dXBoneIdUD*>(iBone->userData.get());
                    if(pBoneId && pBoneId->id == attributes.getValueAsInteger("ID")) {
                        boneName = iBone->name();
                        break;
                    }
                }
            }

            float w = 0.0f;
            ssText >> w;
            m_currVert.addInfluence(Influence(w, boneName));

        } else {
            // Just ignore superfluous entries silently.
        }
    }

    void CoreMesh_Cal3dXHandler::meshStart(const XMLAttributes& attributes)
    {
        // Nothing special to do, we only support one mesh per file.
    }

    void CoreMesh_Cal3dXHandler::meshEnd()
    {
        // Nothing special to do, we only support one mesh per file.
    }

    void CoreMesh_Cal3dXHandler::submeshStart(const XMLAttributes& attributes)
    {
        // Just numerate the submeshes.
        m_currSubMesh = CoreSubMeshPtr(new CoreSubMesh(std::string("submesh") + to_s(m_iCurrSubMeshId)));
        m_iCurrSubMeshId++;
    }

    void CoreMesh_Cal3dXHandler::submeshEnd()
    {
        m_meshToBuild.add(m_currSubMesh);
    }

    void CoreMesh_Cal3dXHandler::vertStart(const XMLAttributes& attributes)
    {
        // Definition of a new vertex, clear the current one.
        m_currVert = Vertex(Vector());
        m_iCurrTexcoordNumber = 0;

        // And store the ID it should have.
        m_iCurrVertexId = to<unsigned int>(attributes.getValue("ID"));
    }

    void CoreMesh_Cal3dXHandler::vertEnd()
    {
        // Done with this vertex, add it to the vertex map.
        m_vertices.insert(std::make_pair(m_iCurrVertexId, m_currVert));
    }

    void CoreMesh_Cal3dXHandler::faceStart(const XMLAttributes& attributes)
    {
        // As the first face starts, we have to put all vertices into the mesh.
        if(m_vertices.size() > 0) {
            unsigned int iPreviousVertexId = 0;
            for(std::map<unsigned int, Vertex>::iterator i = m_vertices.begin() ; i != m_vertices.end() ; ++i) {
                // Add dummy vertices for "empty id" ranges.
                for(unsigned j = 1 ; j < i->first - iPreviousVertexId ; ++j) {
                    m_currSubMesh->addVertex(Vertex(Vector()));
                }

                iPreviousVertexId = i->first;
                m_currSubMesh->addVertex(i->second);
            }

            // We only need to do this once, of course!
            m_vertices.clear();
        }

        // The face has the vertices as an attribute.
        if(attributes.exists("VERTEXID")) {
            std::stringstream ss(attributes.getValue("VERTEXID"));

            std::vector<Face::index_t> idxs;
            Face::index_t idx = 0;
            while(ss >> idx)
                idxs.push_back(idx);

            m_currSubMesh->addFace(Face(idxs));
        }
    }

    void CoreMesh_Cal3dXHandler::faceEnd()
    {
        // Nothing to do here, it has been added straight on creation.
    }

} // namespace bouge
