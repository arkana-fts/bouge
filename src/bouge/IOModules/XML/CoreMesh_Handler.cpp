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
#include <bouge/IOModules/XML/CoreMesh_Handler.hpp>
#include <bouge/IOModules/XMLParserCommon/XMLAttributes.hpp>

#include <bouge/CoreMesh.hpp>
#include <bouge/Util.hpp>

#include <sstream>

namespace bouge {

    CoreMesh_XMLHandler::CoreMesh_XMLHandler(CoreMesh& meshToBuild)
        : m_meshToBuild(meshToBuild)
        , m_currVert(Vector())
    { }

    CoreMesh_XMLHandler::~CoreMesh_XMLHandler()
    { }

    bool CoreMesh_XMLHandler::wantsToEnter(const std::string& element)
    {
        static const std::string accepted[] = {"MESH", "SUBMESH", "VERTEX", "POS", "ATTRIB", "INFLUENCE", "FACE", "VERTEXID", ""};

        for(const std::string* i = &accepted[0] ; i->length() > 0 ; ++i) {
            if(element == *i)
                return true;
        }

        return false;
    }

    void CoreMesh_XMLHandler::elementStart(const std::string& element, const XMLAttributes& attributes)
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

    void CoreMesh_XMLHandler::elementEnd(const std::string& element)
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

    void CoreMesh_XMLHandler::textElement(const std::string& element, const XMLAttributes& attributes, const std::string& text)
    {
        if(element == "POS") {

            std::stringstream ssText(text);
            Vector v;
            ssText >> v[0] >> v[1] >> v[2];
            m_currVert.pos(v);

        } else if(element == "ATTRIB") {

            if(attributes.exists("TYPE")) {
                m_currVert.attrib(attributes.getValue("TYPE"), to< std::vector<float> >(text));
            }

        } else if(element == "INFLUENCE") {

            if(attributes.exists("BONE")) {
                m_currVert.addInfluence(Influence(to<float>(text), attributes.getValue("BONE")));
            }

        } else if(element == "VERTEXID") {

            m_faceIndices.push_back(to<Face::index_t>(text));

        } else {
            // Just ignore superfluous entries silently.
        }
    }

    void CoreMesh_XMLHandler::meshStart(const XMLAttributes& attributes)
    {
        m_meshToBuild.name(attributes.getValue("NAME"));
    }

    void CoreMesh_XMLHandler::meshEnd()
    {
        // Nothing special to do, we only support one mesh per file.
    }

    void CoreMesh_XMLHandler::submeshStart(const XMLAttributes& attributes)
    {
        // Clear the submesh to start from scratch.
        m_currSubMesh = CoreSubMeshPtr(new CoreSubMesh(attributes.getValue("NAME")));
    }

    void CoreMesh_XMLHandler::submeshEnd()
    {
        // Add the submesh to the mesh.
        m_meshToBuild.add(m_currSubMesh);
    }

    void CoreMesh_XMLHandler::vertStart(const XMLAttributes& attributes)
    {
        // Definition of a new vertex, clear the current one.
        m_currVert = Vertex(Vector());
    }

    void CoreMesh_XMLHandler::vertEnd()
    {
        // Protection against faulty files where the "submesh" is missing.
        if(!m_currSubMesh) {
            m_currSubMesh = CoreSubMeshPtr(new CoreSubMesh("Unnamed Submesh"));
        }

        // Done with this vertex, add it to the mesh.
        m_currSubMesh->addVertex(m_currVert);
    }

    void CoreMesh_XMLHandler::faceStart(const XMLAttributes& attributes)
    {
        // Just clear the history.
        m_faceIndices.clear();

        // The face may have the vertices as an attribute. This is mainly for
        // compatibility with Cal3D files.
        if(attributes.exists("VERTEXID")) {
            std::stringstream ss(attributes.getValue("VERTEXID"));

            Face::index_t idx = 0;
            while(ss >> idx)
                m_faceIndices.push_back(idx);
        }
    }

    void CoreMesh_XMLHandler::faceEnd()
    {
        // Protection against faulty files where the "submesh" is missing.
        if(!m_currSubMesh) {
            m_currSubMesh = CoreSubMeshPtr(new CoreSubMesh("Unnamed Submesh"));
        }

        // Create the face and add it to the mesh.
        m_currSubMesh->addFace(Face(m_faceIndices));
    }

} // namespace bouge
