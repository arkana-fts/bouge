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
#include "GLSLStaticViewer.hpp"

#include <bouge/bouge.hpp>
#include <bouge/IOModules/XML/Loader.hpp>
#include <bouge/IOModules/XMLParserCommon/XMLParserModules/TinyXMLParser.hpp>

#include <iostream>
#include <fstream> // Only to see if a file exists.
#include <stdexcept>

template<class T, size_t N> T decay_array_to_subtype(T (&a)[N]);
#define dimension_of(X) (sizeof(X)/sizeof(decay_array_to_subtype(X)))

using namespace bouge;

namespace bougeExample
{
    // This user-data is used to store the texture information along with the material.
    struct TextureUserData : public bouge::UserData {
        bicali::Bitmap* tex;

        TextureUserData(const std::string& filename)
            : tex(bicali::load_image(filename.c_str()))
        { }
        virtual ~TextureUserData()
        {
            bicali::unload_image(tex);
        }
    };

    Viewer* makeViewer(std::vector<std::string>& args)
    {
        return new GLSLStaticViewer(args);
    }

    GLSLStaticViewer::GLSLStaticViewer(std::vector<std::string>& args)
        : Viewer(args)
        , m_bStride(true)
        , m_nVertexComponents(3)
        , m_stride(0*sizeof(float))
    { }

    GLSLStaticViewer::~GLSLStaticViewer()
    {
        gl_DeleteBuffers(dimension_of(m_VBOIds), m_VBOIds);
        gl_DeleteVertexArrays(dimension_of(m_VAOIds), m_VAOIds);
    }

    void GLSLStaticViewer::init(std::vector<std::string>& args)
    {
        bougeExample::Viewer::init(args);

        if(args.size() <= 1) {
            std::cout << "Usage: " << args[0] << " MESH_FILE" << std::endl;
            std::cout << std::endl;
            std::cout << "Shows the mesh of the file MESH_FILE" << std::endl;
            exit(0);
        }

        this->loadShaders();
        this->loadMesh(args[1]);
        this->loadHardwareMesh(true);

        this->useInfoText(this->makeInfoText());
    }

    void GLSLStaticViewer::loadShaders()
    {
        m_shaderNoTexture = ShaderPtr(new bicali::detail::Shader(
            "#version 140\n"
            "\n"
            "uniform mat4 uModelViewProjectionMatrix;\n"
            "uniform mat4 uModelViewMatrix;\n"
            "uniform mat3 uNormalMatrix;\n"
            "\n"
            "in vec3 aVertex;\n"
            "in vec3 aNormal;\n"
            "\n"
            "out vec4 vPosition;\n"
            "out vec3 vNormal;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    vPosition = uModelViewMatrix * vec4(aVertex, 1.0);\n"
            "    vNormal = uNormalMatrix * aNormal;\n"
            "    gl_Position = uModelViewProjectionMatrix * vec4(aVertex, 1.0);\n"
            "}\n"
            ,
            "#version 140\n"
            "\n"
            "uniform vec3 uAmbient;\n"
            "uniform vec3 uDiffuse;\n"
            "uniform vec3 uSpecular;\n"
            "uniform float uShininess;\n"
            "\n"
            "in vec4 vPosition;\n"
            "in vec3 vNormal;\n"
            "\n"
            "out vec4 oFragColor;\n"
            "\n"
            "void main(void)\n"
            "{\n"
            "    vec3 n = normalize(vNormal);\n"
            "    vec3 p = normalize(vPosition.xyz);\n"
            "\n"
            "    // Ambient color.\n"
            "    vec4 Ca = vec4(uAmbient, 1.0);\n"
            "\n"
            "    // Diffuse color.\n"
//             "    vec3 l = vec3(0.0, 10.0, -10.0); // light position in eye space.\n"
//             "    vec3 negLightDir = normalize(l - p);\n"
            "    vec3 negLightDir = -normalize(vec3(0.0, 0.0, -1.0));\n"
            "    vec3 diff = uDiffuse;\n"
            "    vec4 Cd = vec4(diff * dot(n, negLightDir), 1.0);\n"
            "\n"
            "    // Specular color.\n"
            "    vec3 r = normalize(-reflect(negLightDir, n));\n"
            "    vec4 Csp = vec4(uSpecular, 1.0) * pow(clamp(dot(r, -p), 0.0, 1.0), uShininess);\n"
            "\n"
            "    oFragColor = clamp(Ca + Cd + Csp, 0.0, 1.0);\n"
            "}\n"
        ));

        m_shaderWithTexture = ShaderPtr(new bicali::detail::Shader(
            "#version 140\n"
            "\n"
            "uniform mat4 uModelViewProjectionMatrix;\n"
            "uniform mat4 uModelViewMatrix;\n"
            "uniform mat3 uNormalMatrix;\n"
            "\n"
            "in vec3 aVertex;\n"
            "in vec3 aNormal;\n"
            "in vec2 aTexCo;\n"
            "\n"
            "out vec4 vPosition;\n"
            "out vec3 vNormal;\n"
            "out vec2 vTexCo;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    vPosition = uModelViewMatrix * vec4(aVertex, 1.0);\n"
            "    vNormal = uNormalMatrix * aNormal;\n"
            "    vTexCo = aTexCo;\n"
            "    gl_Position = uModelViewProjectionMatrix * vec4(aVertex, 1.0);\n"
            "}\n"
            ,
            "#version 140\n"
            "\n"
            "uniform vec3 uAmbient;\n"
            "uniform vec3 uDiffuse;\n"
            "uniform vec3 uSpecular;\n"
            "uniform float uShininess;\n"
            "uniform sampler2DRect uDiffTex;\n"
            "\n"
            "in vec4 vPosition;\n"
            "in vec3 vNormal;\n"
            "in vec2 vTexCo;\n"
            "\n"
            "out vec4 oFragColor;\n"
            "\n"
            "void main(void)\n"
            "{\n"
            "    vec3 n = normalize(vNormal);\n"
            "    vec3 p = normalize(vPosition.xyz);\n"
            "\n"
            "    // Ambient color.\n"
            "    vec4 Ca = vec4(uAmbient, 1.0);\n"
            "\n"
            "    // Diffuse color.\n"
//             "    vec3 l = vec3(0.0, 10.0, -10.0); // light position in eye space.\n"
//             "    vec3 negLightDir = normalize(l - p);\n"
            "    vec3 negLightDir = -normalize(vec3(0.0, 0.0, -1.0));\n"
            "    vec3 diff = texture(uDiffTex, vTexCo*textureSize(uDiffTex, 0)).rgb;\n"
            "    vec4 Cd = vec4(diff * dot(n, negLightDir), 1.0);\n"
            "\n"
            "    // Specular color.\n"
            "    vec3 r = normalize(-reflect(negLightDir, n));\n"
            "    vec4 Csp = vec4(uSpecular, 1.0) * pow(clamp(dot(r, -p), 0.0, 1.0), uShininess);\n"
            "\n"
            "    oFragColor = clamp(Ca + Cd + Csp, 0.0, 1.0);\n"
            "}\n"
        ));
    }

    void GLSLStaticViewer::loadMesh(std::string filename)
    {
        XMLLoader loader(new TinyXMLParser());

        m_model.reset(new CoreModel(filename));
        m_model->mesh(loader.loadMesh(filename + ".bxmesh"));

        // Load the materials and material sets if there are.
        if(std::ifstream((filename + ".bxmset").c_str()) && std::ifstream((filename + ".bxmat").c_str())) {
            m_model->addMaterialSets(loader.loadMaterialSet(filename + ".bxmset"));

            // Note that here we assume that _all_ of the materials used in the
            // material sets are located in one single material file.
            m_model->addMaterials(loader.loadMaterial(filename + ".bxmat"));

            // You could add more materials or material sets from other files.

            std::set<std::string> missingMaterials = m_model->missingMaterials();
            if(!missingMaterials.empty()) {
                throw std::runtime_error("Missing the following materials in the model: " + to_s(missingMaterials.begin(), missingMaterials.end()));
            }

            std::set<std::string> missingMatsetSpecs = m_model->missingMatsetSpecs();
            if(!missingMatsetSpecs.empty()) {
                throw std::runtime_error("Missing the material set specifications for the following submeshes: " + to_s(missingMatsetSpecs.begin(), missingMatsetSpecs.end()));
            }

            // ... If you don't need the detailed message, you may instead just do it this way:
//             if(!m_model->isComplete()) {
//                 throw std::runtime_error("Hey, there's something missing in the mesh");
//             }
        } else {
            // No materials available, create our own default material set.
            CoreMaterialPtr pDefMat(new CoreMaterial("default"));
            pDefMat->proprety("ambient", "0.25 0.25 0.25");
            pDefMat->proprety("diffuse", "0.34 0.34 0.34");
            pDefMat->proprety("specular", "0.35 0.35 0.35");
            pDefMat->proprety("shininess", "1.0");
            m_model->addMaterial(pDefMat);

            CoreMaterialSetPtr pDefMatSet(new CoreMaterialSet("default"));
            for(CoreMesh::iterator iSubMesh = m_model->mesh()->begin() ; iSubMesh != m_model->mesh()->end() ; ++iSubMesh) {
                pDefMatSet->materialForMesh(iSubMesh->name(), "default");
            }
            m_model->addMaterialSet(pDefMatSet);
        }

        // Now, we need to load all textures associated to the materials:
        for(CoreModel::material_iterator iMat = m_model->begin_material() ; iMat != m_model->end_material() ; ++iMat) {
            if(!iMat->hasProprety("map"))
                continue;

            iMat->userData = bouge::UserDataPtr(new TextureUserData(iMat->proprety("map")));
        }

        m_modelInst = StaticModelInstancePtr(new StaticModelInstance(m_model));

        // If we don't select one here, a random one has already been selected.
        // You should not rely on which one it is, unless you only have one material set.
        if(m_model->hasMaterialSet("default")) {
            m_modelInst->selectMatSet("default");
        }

        checkError("loadMesh");
    }

    void GLSLStaticViewer::loadHardwareMesh(bool stride)
    {
        gl_GenBuffers(dimension_of(m_VBOIds), m_VBOIds);
        gl_GenVertexArrays(dimension_of(m_VAOIds), m_VAOIds);
        gl_BindVertexArray(m_VAOIds[0]);

        m_hwmesh = CoreHardwareMeshPtr(new CoreHardwareMesh(m_model->mesh(), 0, 0, 3));

        if(stride) {
            bool hasTexCo = m_hwmesh->hasAttrib("texcoord0");
            m_shaderToUse = hasTexCo ? m_shaderWithTexture : m_shaderNoTexture;

            std::size_t coordsOffset = 0;
            std::size_t normalsOffset = m_hwmesh->coordsPerVertex();
            std::size_t texCoOffset = normalsOffset + m_hwmesh->attribCoordsPerVertex("normal");
            std::size_t floatsPerVertex = texCoOffset + (hasTexCo ? m_hwmesh->attribCoordsPerVertex("texcoord0") : 0);

            m_stride = floatsPerVertex * sizeof(float);

            // Here, we compile all the vertex data into a single interleaved buffer.
            std::vector<float> data(floatsPerVertex * m_hwmesh->vertexCount());
            m_hwmesh->writeCoords(&data[coordsOffset], m_stride);
            m_hwmesh->writeAttrib("normal", &data[normalsOffset], m_stride);
            if(hasTexCo)
                m_hwmesh->writeAttrib("texcoord0", &data[texCoOffset], m_stride);

            // Upload that data into the VBO
            gl_BindBuffer(GL_ARRAY_BUFFER, m_VBOIds[1]);
            gl_BufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

            // And setup the vertex attribute positions.
            gl_EnableVertexAttribArray(m_shaderToUse->attrib("aVertex"));
            gl_VertexAttribPointer(m_shaderToUse->attrib("aVertex"), m_hwmesh->coordsPerVertex(), GL_FLOAT, GL_FALSE, m_stride, (const GLvoid*)(coordsOffset * sizeof(float)));

            if(m_shaderToUse->hasAttrib("aNormal")) {
                gl_EnableVertexAttribArray(m_shaderToUse->attrib("aNormal"));
                gl_VertexAttribPointer(m_shaderToUse->attrib("aNormal"), m_hwmesh->attribCoordsPerVertex("normal"), GL_FLOAT, GL_FALSE, m_stride, (const GLvoid*)(normalsOffset * sizeof(float)));
            }

            if(hasTexCo && m_shaderToUse->hasAttrib("aTexCo")) {
                gl_EnableVertexAttribArray(m_shaderToUse->attrib("aTexCo"));
                gl_VertexAttribPointer(m_shaderToUse->attrib("aTexCo"), m_hwmesh->attribCoordsPerVertex("texcoord0"), GL_FLOAT, GL_FALSE, m_stride, (const GLvoid*)(texCoOffset * sizeof(float)));
            }

        } else {
            // TODO use unstrided VBOs too.
        }

        // Faces work in the same way, regardless the vertex format.
        // Here, we only support triangles.
        gl_BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOIds[0]);
        gl_BufferData(GL_ELEMENT_ARRAY_BUFFER, m_hwmesh->faceCount() * m_hwmesh->indicesPerFace() * sizeof(BOUGE_FACE_INDEX_TYPE), &m_hwmesh->faceIndices()[0], GL_STATIC_DRAW);

        gl_BindVertexArray(0);

        std::cout << "The hardware mesh has " << m_hwmesh->vertexCount() << " vertices and " << m_hwmesh->faceCount() << " faces." << std::endl;
        std::cout << "It has been subdivided in the following " << m_hwmesh->submeshCount() << " submeshes:" << std::endl;
        for(CoreHardwareMesh::iterator i = m_hwmesh->begin() ; i != m_hwmesh->end() ; ++i) {
            std::cout << "  * submesh with " << i->faceCount() << " faces, " << i->boneCount() << " bones, start index: " << i->startIndex() << std::endl;
        }
    }

    std::string GLSLStaticViewer::makeInfoText() const
    {
        return "These are some informations about the mesh. To hide them, press F1.\n"
               "The mesh has a total of " + to_s(m_hwmesh->vertexCount()) + " vertices and " + to_s(m_hwmesh->faceCount()) + " faces in " + to_s(m_hwmesh->submeshCount()) + " hardware meshes.\n"
               "The selected skin (" + to_s(m_model->materialSetCount()) + " available, space key to change) is: " + m_modelInst->currMatSet()
        ;
    }

    void GLSLStaticViewer::keyboard(int key, int x, int y)
    {
        bougeExample::Viewer::keyboard(key, x, y);

        if(key == ' ') {
            // Cycle through the material sets.
            std::string sMatSet;
            for(CoreModel::materialset_iterator iMatSet = m_model->begin_materialset() ; iMatSet != m_model->end_materialset() ; ++iMatSet) {
                if(iMatSet->name() == m_modelInst->currMatSet()) {
                    if(++iMatSet != m_model->end_materialset()) {
                        sMatSet = iMatSet->name();
                    } else {
                        sMatSet = m_model->begin_materialset()->name();
                    }
                    break;
                }
            }
            m_modelInst->selectMatSet(sMatSet);
            this->useInfoText(this->makeInfoText());
        }
    }

    void GLSLStaticViewer::render(const Camera& cam, const bouge::AffineMatrix& model) const
    {
        // This is a small, but effective, optimization that unfortunately GCC doesn't do.
        static const std::string uModelViewProjectionMatrix = "uModelViewProjectionMatrix";
        static const std::string uModelViewMatrix = "uModelViewMatrix";
        static const std::string uNormalMatrix = "uNormalMatrix";
        static const std::string uAmbient = "uAmbient";
        static const std::string ambient = "ambient";
        static const std::string uDiffuse = "uDiffuse";
        static const std::string diffuse = "diffuse";
        static const std::string uSpecular = "uSpecular";
        static const std::string specular = "specular";
        static const std::string uShininess = "uShininess";
        static const std::string shininess = "shininess";
        static const std::string uDiffTex = "uDiffTex";

        Viewer::render(cam, model);

        m_shaderToUse->use();

        AffineMatrix mv = cam.view() * model;
        AffineMatrix id = mv * mv.inverse();
        m_shaderToUse->uniformMatrix4fv(uModelViewProjectionMatrix, 1, false, (cam.viewproj() * model).array16f());
        m_shaderToUse->uniformMatrix4fv(uModelViewMatrix, 1, false, mv.array16f());
        m_shaderToUse->uniformMatrix3fv(uNormalMatrix, 1, true, mv.array9fInverse());

        gl_BindVertexArray(m_VAOIds[0]);

        // Now, render each submesh of the mesh one after. It may need to get split
        // for example if it has too many bones.
        for(CoreHardwareMesh::iterator i = m_hwmesh->begin() ; i != m_hwmesh->end() ; ++i) {
            const CoreHardwareSubMesh& submesh = *i;

            // We set the per-submesh material options.

            // Here, we can assume the material exists, as we did the
            // "integrity checks" after the loading already.
            CoreMaterialPtr pMat = m_modelInst->materialForSubmesh(submesh.submeshName());

            if(pMat->hasProprety(ambient)) {
                m_shaderToUse->uniform3fv(uAmbient, 1, &pMat->propretyAsFvec(ambient)[0]);
            }

            if(pMat->hasProprety(diffuse)) {
                m_shaderToUse->uniform3fv(uDiffuse, 1, &pMat->propretyAsFvec(diffuse)[0]);
            }

            if(pMat->hasProprety(specular)) {
                m_shaderToUse->uniform3fv(uSpecular, 1, &pMat->propretyAsFvec(specular)[0]);
            }

            if(pMat->hasProprety(shininess)) {
                m_shaderToUse->uniformf(uShininess, pMat->propretyAsFvec(shininess)[0]);
            }

            if(pMat->userData) {
                static_cast<TextureUserData*>(pMat->userData.get())->tex->selectTexture(0);
                m_shaderToUse->uniformi(uDiffTex, 0);
            }

#ifdef BOUGE_FACE_INDEX_UINT
            glDrawElements(GL_TRIANGLES, submesh.faceCount() * m_hwmesh->indicesPerFace(), GL_UNSIGNED_INT, (const GLvoid*)(submesh.startIndex()*sizeof(BOUGE_FACE_INDEX_TYPE)));
#else
            glDrawElements(GL_TRIANGLES, submesh.faceCount() * m_hwmesh->indicesPerFace(), GL_UNSIGNED_SHORT, (const GLvoid*)(submesh.startIndex()*sizeof(BOUGE_FACE_INDEX_TYPE)));
#endif
        }

        gl_BindVertexArray(0);
        gl_BindBuffer(GL_ARRAY_BUFFER, 0);
        gl_BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        checkError("GLSLStaticViewer::render");
    }

}
