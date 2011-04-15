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
#include "GLSLMoulinViewer.hpp"

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
    static const unsigned int maxBonesPerMesh = 30;

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
        return new GLSLMoulinViewer(args);
    }

    GLSLMoulinViewer::GLSLMoulinViewer(std::vector<std::string>& args)
        : Viewer(args)
        , m_bStride(true)
        , m_nVertexComponents(3)
        , m_stride(0*sizeof(float))
        , m_bPaused(false)
    { }

    GLSLMoulinViewer::~GLSLMoulinViewer()
    {
        gl_DeleteBuffers(dimension_of(m_VBOIds), m_VBOIds);
        gl_DeleteVertexArrays(dimension_of(m_VAOIds), m_VAOIds);
    }

    void GLSLMoulinViewer::init(std::vector<std::string>& args)
    {
        bougeExample::Viewer::init(args);

        if(args.size() <= 1) {
            std::cout << "Usage: " << args[0] << " MESH_FILE" << std::endl;
            std::cout << std::endl;
            std::cout << "Shows the mesh of the file MESH_FILE" << std::endl;
            exit(0);
        }

        this->loadShaders();
        this->loadModel(args[1]);
        this->loadHardwareMesh(true);

        // We just start playing a random animation, if there is one.
        m_currAnim = m_model->end_animation();
        this->playNextAnim();
        this->useInfoText(this->makeInfoText());
    }

    void GLSLMoulinViewer::loadShaders()
    {
        m_shaderNoTexture = ShaderPtr(new bicali::detail::Shader(
            ("#version 140\n"
            "\n"
            "uniform mat4 uModelViewProjectionMatrix;\n"
            "uniform mat4 uModelViewMatrix;\n"
            "uniform mat3 uNormalMatrix;\n"
            "uniform mat4 uBonesPalette["+to_s(maxBonesPerMesh)+"];\n"
            "\n"
            "in vec3 aVertex;\n"
            "in vec3 aNormal;\n"
            "in vec4 aWeights;\n"
            "in vec4 aIndices;\n"
            "\n"
            "out vec4 vPosition;\n"
            "out vec3 vNormal;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    vec3 localPos = vec3(0.0), localNor = vec3(0.0);\n"
            "    for(int i = 0 ; i < 4 ; i++) {\n"
            "        localPos += (uBonesPalette[int(aIndices[i])] * vec4(aVertex, 1.0)).xyz * aWeights[i];\n"
            "    }\n"
            "    vPosition = uModelViewMatrix * vec4(localPos, 1.0);\n"
            "    vNormal = uNormalMatrix * aNormal;\n"
            "    gl_Position = uModelViewProjectionMatrix * vec4(localPos, 1.0);\n"
            "}\n").c_str()
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
            "    vec3 l = vec3(0.0, 1.0, 0.0); // light position in eye space.\n"
            "    vec3 lmp = normalize(l - p);\n"
            "    vec4 Cd = vec4(uDiffuse, 1.0) * dot(n, lmp);\n"
            "\n"
            "    // Specular color.\n"
            "    vec3 r = normalize(-reflect(lmp, n));\n"
            "    vec4 Csp = vec4(uSpecular, 1.0) * pow(clamp(dot(r, -p), 0.0, 1.0), uShininess);\n"
            "\n"
            "    oFragColor = clamp(Ca + Cd + Csp, 0.0, 1.0);\n"
            "}\n"
        ));

        m_shaderWithTexture = ShaderPtr(new bicali::detail::Shader(
            ("#version 140\n"
            "\n"
            "uniform mat4 uModelViewProjectionMatrix;\n"
            "uniform mat4 uModelViewMatrix;\n"
            "uniform mat3 uNormalMatrix;\n"
            "uniform mat4 uBonesPalette["+to_s(maxBonesPerMesh)+"];\n"
            "\n"
            "in vec3 aVertex;\n"
            "in vec3 aNormal;\n"
            "in vec2 aTexCo;\n"
            "in vec4 aWeights;\n"
            "in vec4 aIndices;\n"
            "\n"
            "out vec4 vPosition;\n"
            "out vec3 vNormal;\n"
            "out vec2 vTexCo;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    vec3 localPos = vec3(0.0), localNor = vec3(0.0);\n"
            "    for(int i = 0 ; i < 4 ; i++) {\n"
            "        localPos += (uBonesPalette[int(aIndices[i])] * vec4(aVertex, 1.0)).xyz * aWeights[i];\n"
            "    }\n"
            "    vPosition = uModelViewMatrix * vec4(localPos, 1.0);\n"
            "    vNormal = uNormalMatrix * aNormal;\n"
            "    vTexCo = aTexCo;\n"
            "    gl_Position = uModelViewProjectionMatrix * vec4(localPos, 1.0);\n"
            "}\n").c_str()
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
            "    vec3 l = vec3(0.0, 1.0, 0.0); // light position in eye space.\n"
            "    vec3 lmp = normalize(l - p);\n"
            "    vec3 diff = texture(uDiffTex, vTexCo*textureSize(uDiffTex, 0)).rgb;\n"
            "    vec4 Cd = vec4(diff * dot(n, lmp), 1.0);\n"
            "\n"
            "    // Specular color.\n"
            "    vec3 r = normalize(-reflect(lmp, n));\n"
            "    vec4 Csp = vec4(uSpecular, 1.0) * pow(clamp(dot(r, -p), 0.0, 1.0), uShininess);\n"
            "\n"
            "    oFragColor = clamp(Ca + Cd + Csp, 0.0, 1.0);\n"
            "}\n"
        ));
    }

    void GLSLMoulinViewer::loadModel(std::string filename)
    {
        XMLLoader loader(new TinyXMLParser());

        m_model.reset(new CoreModel(filename));
        m_model->mesh(loader.loadMesh(filename + ".bxmesh"));
        m_model->skeleton(loader.loadSkeleton(filename + ".bxskel"));
        m_model->addMaterialSets(loader.loadMaterialSet(filename + ".bxmset"));

        // Note that here we assume that _all_ of the materials used in the
        // material sets are located in one single material file.
        m_model->addMaterials(loader.loadMaterial(filename + ".bxmat"));

        // You could add more materials or material sets from other files.

        // The same goes for the animations.
        if(std::ifstream((filename + ".bxanim").c_str())) {
            m_model->addAnimations(loader.loadAnimation(filename + ".bxanim"));
        }

        // Check if the model is complete. This could also be done differently, see below ...
        std::set<std::string> missingBones = m_model->missingBones();
        if(!missingBones.empty()) {
            throw std::runtime_error("Missing the following bones in the skeleton: " + to_s(missingBones.begin(), missingBones.end()));
        }

        std::set<std::string> missingMaterials = m_model->missingMaterials();
        if(!missingMaterials.empty()) {
            throw std::runtime_error("Missing the following materials in the model: " + to_s(missingMaterials.begin(), missingMaterials.end()));
        }

        std::set<std::string> missingMatsetSpecs = m_model->missingMatsetSpecs();
        if(!missingMatsetSpecs.empty()) {
            throw std::runtime_error("Missing the material set specifications for the following submeshes: " + to_s(missingMatsetSpecs.begin(), missingMatsetSpecs.end()));
        }

        // ... If you don't need the detailed message, you may instead just do it this way:
//         if(!m_model->isComplete()) {
//             throw std::runtime_error("Hey, there's something missing in the mesh");
//         }

        m_modelInst = ModelInstancePtr(new ModelInstance(m_model));

        // If we don't select one here, a random one has already been selected.
        // You should not rely on which one it is, unless you only have one material set.
//         m_modelInst->selectMatSet("chromed red");

        checkError("loadModel");
    }

    void GLSLMoulinViewer::loadHardwareMesh(bool stride)
    {
        gl_GenBuffers(dimension_of(m_VBOIds), m_VBOIds);
        gl_GenVertexArrays(dimension_of(m_VAOIds), m_VAOIds);
        gl_BindVertexArray(m_VAOIds[0]);

        m_hwmesh = m_model->buildHardwareMesh(maxBonesPerMesh, 4);

        if(stride) {
            bool hasTexCo = m_hwmesh->hasAttrib("texcoord0");
            m_shaderToUse = hasTexCo ? m_shaderWithTexture : m_shaderNoTexture;

            std::size_t coordsOffset = 0;
            std::size_t normalsOffset = m_hwmesh->coordsPerVertex();
            std::size_t texCoOffset = normalsOffset + m_hwmesh->attribCoordsPerVertex("normal");
            std::size_t weightsOffset = texCoOffset + (hasTexCo ? m_hwmesh->attribCoordsPerVertex("texcoord0") : 0);
            std::size_t indicesOffset = weightsOffset + m_hwmesh->weightsPerVertex();
            std::size_t floatsPerVertex = indicesOffset + m_hwmesh->boneIndicesPerVertex();

            m_stride = floatsPerVertex * sizeof(float);

            // Here, we compile all the vertex data into a single interleaved buffer.
            std::vector<float> data(floatsPerVertex * m_hwmesh->vertexCount());
            m_hwmesh->writeCoords(&data[coordsOffset], m_stride);
            m_hwmesh->writeAttrib("normal", &data[normalsOffset], m_stride);
            if(hasTexCo)
                m_hwmesh->writeAttrib("texcoord0", &data[texCoOffset], m_stride);
            m_hwmesh->writeWeights(&data[weightsOffset], m_stride);
            m_hwmesh->writeBoneIndices(&data[indicesOffset], m_stride);

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

            if(m_shaderToUse->hasAttrib("aWeights")) {
                gl_EnableVertexAttribArray(m_shaderToUse->attrib("aWeights"));
                gl_VertexAttribPointer(m_shaderToUse->attrib("aWeights"), m_hwmesh->weightsPerVertex(), GL_FLOAT, GL_FALSE, m_stride, (const GLvoid*)(weightsOffset * sizeof(float)));
            }

            if(m_shaderToUse->hasAttrib("aIndices")) {
                gl_EnableVertexAttribArray(m_shaderToUse->attrib("aIndices"));
                gl_VertexAttribPointer(m_shaderToUse->attrib("aIndices"), m_hwmesh->boneIndicesPerVertex(), GL_FLOAT, GL_FALSE, m_stride, (const GLvoid*)(indicesOffset * sizeof(float)));
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

    void GLSLMoulinViewer::playNextAnim()
    {
        if(m_model->animationCount() < 1)
            return;

        if(m_currAnim == m_model->end_animation() || ++m_currAnim == m_model->end_animation())
            m_currAnim = m_model->begin_animation();

        /// \TODO Bypass the mixer here, when the model inst has a fine interface.
        m_modelInst->mixer()->play(AnimationPtr(new Animation(*m_currAnim, 1.0f)));
    }

    void GLSLMoulinViewer::playPrevAnim()
    {
        if(m_model->animationCount() < 1)
            return;

        if(m_currAnim == m_model->begin_animation()) {
            m_currAnim = m_model->end_animation();
        }

        m_currAnim--;

        /// \TODO Bypass the mixer here, when the model inst has a fine interface.
        m_modelInst->mixer()->play(AnimationPtr(new Animation(*m_currAnim, 1.0f)));
    }

    std::string GLSLMoulinViewer::makeInfoText() const
    {
        std::string animName = m_model->animationCount() > 0 ? m_currAnim->name() : "N/A";
        return "These are some informations about the skeleton. To hide them, press F1.\n"
               "Current animation (next: pgup, prev: pgdown): " + animName + " (#animations: " + to_s(m_model->animationCount()) + ")\n"
               "The mesh has a total of " + to_s(m_hwmesh->vertexCount()) + " vertices and " + to_s(m_hwmesh->faceCount()) + " faces in " + to_s(m_hwmesh->submeshCount()) + " hardware meshes.\n"
               "The skeleton has " + to_s(m_model->skeleton()->boneCount()) + " bones.\n"
        ;
    }

    void GLSLMoulinViewer::keyboard(int key, int x, int y)
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
        } else if(key == GLUT_KEY_PAGE_UP) {
            this->playNextAnim();
            this->useInfoText(this->makeInfoText());
        } else if(key == GLUT_KEY_PAGE_DOWN) {
            this->playPrevAnim();
            this->useInfoText(this->makeInfoText());
        } else if(key == 'p') {
            m_bPaused = !m_bPaused;
        }

    }

    void GLSLMoulinViewer::update(float dt)
    {
        bougeExample::Viewer::update(dt);

        // Update the model's animation state.
        /// \TODO Bypass the mixer as soon as the instance got a clean interface.
        if(!m_bPaused) {
            m_modelInst->mixer()->update(dt);
        }
    }

    static std::string uBonesPalette(std::size_t i)
    {
        static const std::string uBonesPalette[] = {
            "uBonesPalette[0]", "uBonesPalette[1]", "uBonesPalette[2]", "uBonesPalette[3]", "uBonesPalette[4]",
            "uBonesPalette[5]", "uBonesPalette[6]", "uBonesPalette[7]", "uBonesPalette[8]", "uBonesPalette[9]",
            "uBonesPalette[10]", "uBonesPalette[11]", "uBonesPalette[12]", "uBonesPalette[13]", "uBonesPalette[14]",
            "uBonesPalette[15]", "uBonesPalette[16]", "uBonesPalette[17]", "uBonesPalette[18]", "uBonesPalette[19]",
            "uBonesPalette[20]", "uBonesPalette[21]", "uBonesPalette[22]", "uBonesPalette[23]", "uBonesPalette[24]",
            "uBonesPalette[25]", "uBonesPalette[26]", "uBonesPalette[27]", "uBonesPalette[28]", "uBonesPalette[29]",
            "uBonesPalette[30]", "uBonesPalette[31]", "uBonesPalette[32]", "uBonesPalette[33]", "uBonesPalette[34]",
            "uBonesPalette[35]", "uBonesPalette[36]", "uBonesPalette[37]", "uBonesPalette[38]", "uBonesPalette[39]",
        };

        if(i < 40)
            return uBonesPalette[i];

        return "uBonesPalette[" + to_s(i) + "]";
    }

    static std::string uBonesPaletteInvTrans(std::size_t i)
    {
        static const std::string uBonesPaletteInvTrans[] = {
            "uBonesPaletteInvTrans[0]", "uBonesPaletteInvTrans[1]", "uBonesPaletteInvTrans[2]", "uBonesPaletteInvTrans[3]", "uBonesPaletteInvTrans[4]",
            "uBonesPaletteInvTrans[5]", "uBonesPaletteInvTrans[6]", "uBonesPaletteInvTrans[7]", "uBonesPaletteInvTrans[8]", "uBonesPaletteInvTrans[9]",
            "uBonesPaletteInvTrans[10]", "uBonesPaletteInvTrans[11]", "uBonesPaletteInvTrans[12]", "uBonesPaletteInvTrans[13]", "uBonesPaletteInvTrans[14]",
            "uBonesPaletteInvTrans[15]", "uBonesPaletteInvTrans[16]", "uBonesPaletteInvTrans[17]", "uBonesPaletteInvTrans[18]", "uBonesPaletteInvTrans[19]",
            "uBonesPaletteInvTrans[20]", "uBonesPaletteInvTrans[21]", "uBonesPaletteInvTrans[22]", "uBonesPaletteInvTrans[23]", "uBonesPaletteInvTrans[24]",
            "uBonesPaletteInvTrans[25]", "uBonesPaletteInvTrans[26]", "uBonesPaletteInvTrans[27]", "uBonesPaletteInvTrans[28]", "uBonesPaletteInvTrans[29]",
            "uBonesPaletteInvTrans[30]", "uBonesPaletteInvTrans[31]", "uBonesPaletteInvTrans[32]", "uBonesPaletteInvTrans[33]", "uBonesPaletteInvTrans[34]",
            "uBonesPaletteInvTrans[35]", "uBonesPaletteInvTrans[36]", "uBonesPaletteInvTrans[37]", "uBonesPaletteInvTrans[38]", "uBonesPaletteInvTrans[39]",
        };

        if(i < 40)
            return uBonesPaletteInvTrans[i];

        return "uBonesPaletteInvTrans[" + to_s(i) + "]";
    }

    void GLSLMoulinViewer::render(const Camera& cam, const bouge::AffineMatrix& model) const
    {
        // This is a small, but effective, optimization that unfortunately GCC doesn't do.
        // Well, it can't do, as it isn't thread-safe!
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

        m_shaderToUse->uniformMatrix4fv(uModelViewProjectionMatrix, 1, false, (cam.viewproj() * model).array16f());
        m_shaderToUse->uniformMatrix4fv(uModelViewMatrix, 1, false, (cam.view() * model).array16f());
        m_shaderToUse->uniformMatrix3fv(uNormalMatrix, 1, false, (cam.view() * model).array9fInverse());

        gl_BindVertexArray(m_VAOIds[0]);

        // Now, render each submesh of the mesh one after. It may need to get split
        // for example if it has too many bones.
        for(CoreHardwareMesh::iterator i = m_hwmesh->begin() ; i != m_hwmesh->end() ; ++i) {
            const CoreHardwareSubMesh& submesh = *i;

            // We set the per-submesh material options.

            // Probably the most important per-submesh option are the bone states
            for(std::size_t i = 0 ; i < submesh.boneCount() ; ++i) {
                /// \TODO Maybe optimize by storing the bone "ID in skeleton" in the hardware submesh, if possible?
                BoneInstancePtrC bone = m_modelInst->skeleton()->bone(submesh.boneName(i));
                m_shaderToUse->uniformMatrix4fv(uBonesPalette(i), 1, false, bone->transformMatrix().array16f());
                m_shaderToUse->uniformMatrix4fv(uBonesPaletteInvTrans(i), 1, false, bone->transformMatrix().array16fInverse());
            }

            // Take care if this happens. We actually *should* render it with
            // a shader that doesn't take skeletal animation into account.
            // But well, let's just deal with it the easy (unoptimized) way,
            // by setting the bone "0" to be the identity matrix.
            // You should actually use the "staticviewer" example to view a model
            // that has no skeleton/bones!
            if(submesh.boneCount() == 0) {
                m_shaderToUse->uniformMatrix4fv(uBonesPalette(0), 1, false, AffineMatrix().array16f());
                m_shaderToUse->uniformMatrix4fv(uBonesPaletteInvTrans(0), 1, false, AffineMatrix().array16fInverse());
            }

            // Here, we can assume the material exists, as we did the
            // "integrity checks" after the loading already.
            CoreMaterialPtrC pMat = m_modelInst->materialForSubmesh(submesh.submeshName());

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

            glDrawElements(GL_TRIANGLES, submesh.faceCount() * m_hwmesh->indicesPerFace(), BOUGE_FACE_INDEX_TYPE_GL, (const GLvoid*)(submesh.startIndex()*sizeof(BOUGE_FACE_INDEX_TYPE)));
        }

        gl_BindVertexArray(0);
        gl_BindBuffer(GL_ARRAY_BUFFER, 0);
        gl_BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        checkError("GLSLMoulinViewer::render");
    }

}
