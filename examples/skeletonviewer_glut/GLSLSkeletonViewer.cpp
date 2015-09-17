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
#include "GLSLSkeletonViewer.hpp"

#include <bouge/bouge.hpp>
#include <bouge/IOModules/XML/Loader.hpp>
#include <bouge/IOModules/XMLParserCommon/XMLParserModules/TinyXMLParser.hpp>

#include <iostream>
#include <stdexcept>

using namespace bouge;

namespace bougeExample
{
    Viewer* makeViewer(std::vector<std::string>& args)
    {
        return new GLSLSkeletonViewer(args);
    }

    GLSLSkeletonViewer::GLSLSkeletonViewer(std::vector<std::string>& args)
        : Viewer(args)
        , m_nVertexComponents(3)
        , m_stride(0*sizeof(float))
    { }

    GLSLSkeletonViewer::~GLSLSkeletonViewer()
    {
        gl_DeleteBuffers(1, &m_VBOId);
    }

    void GLSLSkeletonViewer::init(std::vector<std::string>& args)
    {
        bougeExample::Viewer::init(args);

        if(args.size() <= 1) {
            std::stringstream ssUsage;
            ssUsage << "Usage: " << args[0] << " SKELETON_FILE [ANIMATIONFILE]" << std::endl;
            ssUsage << std::endl;
            ssUsage << "Shows the skeleton of the file SKELETON_FILE [ANIMATIONFILE]" << std::endl;
            throw std::runtime_error(ssUsage.str());
        }

        // Reuse the shader from Viewer.
        //this->loadShaders();
        this->loadSkeleton(args[1]);

        for(std::size_t i = 2 ; i < args.size() ; ++i) {
            this->loadAnimation(args[i]);
        }

        m_currAnim = m_anim.end();
        this->playNextAnim();
        this->useInfoText(this->makeInfoText());

/*
        if(args.size() > 2) {
            this->loadAnimation(args[2]);
            if(m_anim) {
//                 m_mixer->play(AnimationPtr(new Animation(m_anim, 1.0f, new bouge::ConstantTF(0.5f), new bouge::ConstantTF(1.0f))));
//                 m_mixer->play(AnimationPtr(new Animation(m_anim, 1.0f, new bouge::LinearTF(1.0f), new bouge::ConstantTF(1.0f))));
//                 m_mixer->play(AnimationPtr(new Animation(m_anim, 1.0f, new bouge::RepeatTF(new bouge::LinearTF(1.0f)), new bouge::ConstantTF(1.0f))));
//                 m_mixer->play(AnimationPtr(new Animation(m_anim, 1.0f, new bouge::CycleTF(new bouge::LinearTF(1.0f)), new bouge::ConstantTF(1.0f))));
//                 m_mixer->play(AnimationPtr(new Animation(m_anim, 1.0f, new bouge::HoldTF(new bouge::LinearTF(1.0f)), new bouge::ConstantTF(1.0f))));

//                 m_mixer->play(AnimationPtr(new Animation(m_anim, 1.0f, new bouge::CycleTF(new bouge::LinearTF(1.0f)), new bouge::ConstantTF(0.5f))));
//                 m_mixer->play(AnimationPtr(new Animation(m_anim, 1.0f, new bouge::CycleTF(new bouge::LinearTF(1.0f)), new bouge::FadeInTF(0.25f, new bouge::ConstantTF(1.0f)))));
//                 m_mixer->play(AnimationPtr(new Animation(m_anim, 1.0f, new bouge::RepeatTF(new bouge::LinearTF(1.0f)), new bouge::FadeOutTF(0.25f, new bouge::ConstantTF(1.0f)))));
                class QuitCB : public AnimationCallback {
                public:
                    virtual void operator()(const bouge::Animation& anim)
                    {
                        glutExit();
                        exit(0);
                    }
                };
                AnimationPtr pAnim(new Animation(m_anim, 1.0f));
                pAnim->addEndCallback(new QuitCB());
                m_mixer->play(pAnim);
            }
//             m_skeleton->play(m_anim);
        }
*/
        gl_EnableVertexAttribArray(m_pColorOnlyShader->attrib("aVertex"));

        // Prepare the VBO:
        gl_GenBuffers(1, &m_VBOId);
        gl_BindBuffer(GL_ARRAY_BUFFER, m_VBOId);
        m_VBOData.resize(m_skeleton->core()->boneCount() * 2 * m_nVertexComponents);
    }

    void GLSLSkeletonViewer::loadSkeleton(std::string filename)
    {
        XMLLoader loader(new TinyXMLParser());

        CoreSkeletonPtr pCoreSkel = loader.loadSkeleton(filename);

        // We create the instance right away, because we only have one model.
        m_skeleton.reset(new SkeletonInstance(pCoreSkel));
        m_mixer.reset(new bouge::DefaultMixer(m_skeleton));

        checkError("loadSkeleton");
    }

    void GLSLSkeletonViewer::loadAnimation(std::string filename)
    {
        XMLLoader loader(new TinyXMLParser());

        std::vector<bouge::CoreAnimationPtr> pAnims = loader.loadAnimation(filename);

        for(std::vector<bouge::CoreAnimationPtr>::iterator i = pAnims.begin() ; i != pAnims.end() ; ++i) {
            m_anim.push_back(*i);
        }

        checkError("loadAnimation");
    }

    void GLSLSkeletonViewer::playNextAnim()
    {
        if(m_anim.empty())
            return;

        if(m_currAnim == m_anim.end() || ++m_currAnim == m_anim.end())
            m_currAnim = m_anim.begin();

        m_mixer->play(AnimationPtr(new Animation(*m_currAnim, 1.0f)));
    }

    void GLSLSkeletonViewer::playPrevAnim()
    {
        if(m_anim.empty())
            return;

        if(m_currAnim == m_anim.begin()) {
            m_currAnim = m_anim.end();
        }

        m_currAnim--;

        m_mixer->play(AnimationPtr(new Animation(*m_currAnim, 1.0f)));
    }

    std::string GLSLSkeletonViewer::makeInfoText() const
    {
        std::string animName = m_anim.size() > 0 ? (*m_currAnim)->name() : "N/A";
        return "These are some informations about the skeleton.\n"
               "To hide them, press F1.\n"
               "Current animation (next: pgup, prev: pgdown): " + animName + "\n"
               "Animation count: " + to_s(m_anim.size()) + "\n"
               "Bone count: " + to_s(m_skeleton->boneCount());
    }

    void GLSLSkeletonViewer::keyboard(int key, int x, int y)
    {
        bougeExample::Viewer::keyboard(key, x, y);

        if(key == GLUT_KEY_PAGE_UP) {
            this->playNextAnim();
        } else if(key == GLUT_KEY_PAGE_DOWN) {
            this->playPrevAnim();
        }

        this->useInfoText(this->makeInfoText());
    }

    void GLSLSkeletonViewer::update(float dt)
    {
        bougeExample::Viewer::update(dt);

        m_mixer->update(dt);

        // Update the bone positions in software, for now.
        int j = 0;
        for(bouge::SkeletonInstance::iterator iBone = m_skeleton->begin() ; iBone != m_skeleton->end() ; ++iBone) {
            Vector vDefaultBoneHead(0.0f, 0.0f, 0.0f);
            Vector vDefaultBoneTail(0.0f, iBone->length(), 0.0f);

            // This could be done in a vertex-shader. Later on.
//             Vector vBoneHead = iBone->matrixBoneSpaceToModelSpace() * vDefaultBoneHead;
//             Vector vBoneTail = iBone->matrixBoneSpaceToModelSpace() * vDefaultBoneTail;
            Vector vBoneHead = iBone->m_absoluteRootPos;
            Vector vBoneTail = iBone->m_absoluteRootPos + iBone->m_absoluteBoneRot.rotate(Vector(0.0f, iBone->length(), 0.0f));
//             Vector vBoneHead = iBone->animationMatrix() * vDefaultBoneHead;
//             Vector vBoneTail = iBone->animationMatrix() * vDefaultBoneTail;

            m_VBOData[j++] = vBoneHead.x();
            m_VBOData[j++] = vBoneHead.y();
            m_VBOData[j++] = vBoneHead.z();
            m_VBOData[j++] = vBoneTail.x();
            m_VBOData[j++] = vBoneTail.y();
            m_VBOData[j++] = vBoneTail.z();
        }
    }

    void GLSLSkeletonViewer::render(const Camera& cam, const bouge::AffineMatrix& model) const
    {
        Viewer::render(cam, model);

        static const float white[] = {1.0f, 1.0f, 1.0f, 1.0f};
        static const float blue[] = {0.0f, 0.0f, 1.0f, 1.0f};
        static const float red[] = {1.0f, 0.0f, 0.0f, 1.0f};

        m_pColorOnlyShader->uniformMatrix4fv("uModelViewProjectionMatrix", 1, false, (cam.viewproj() * model).array16f());
        m_pColorOnlyShader->uniformMatrix4fv("uModelViewMatrix", 1, false, (cam.view() * model).array16f());
        m_pColorOnlyShader->uniform4fv("uColor", 1, white);

        gl_BindBuffer(GL_ARRAY_BUFFER, m_VBOId);
        gl_BufferData(GL_ARRAY_BUFFER, m_VBOData.size() * sizeof(float), &m_VBOData[0], GL_DYNAMIC_DRAW);

        glEnable(GL_LINE_SMOOTH);
//         glLineWidth(2.0f);

        gl_VertexAttribPointer(m_pColorOnlyShader->attrib("aVertex"), m_nVertexComponents, GL_FLOAT, GL_FALSE, m_stride, 0);
        glDrawArrays(GL_LINES, 0, m_VBOData.size()/m_nVertexComponents);

//         glLineWidth(1.0f);
        glDisable(GL_LINE_SMOOTH);

        // Now come the head and tail points...

        // first, the heads points in blue
        GLsizei stride = (m_stride == 0 ? m_nVertexComponents * 2 * sizeof(float) : m_stride * 2);
        gl_VertexAttribPointer(m_pColorOnlyShader->attrib("aVertex"), m_nVertexComponents, GL_FLOAT, GL_FALSE, stride, 0);

        m_pColorOnlyShader->uniform4fv("uColor", 1, blue);

        glPointSize(7.0f);
        glDrawArrays(GL_POINTS, 0, m_VBOData.size()/m_nVertexComponents/2);

        // then, the tail points in red.
        gl_VertexAttribPointer(m_pColorOnlyShader->attrib("aVertex"), m_nVertexComponents, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)((long)stride/2));

        m_pColorOnlyShader->uniform4fv("uColor", 1, red);

        glPointSize(3.0f);
        glDrawArrays(GL_POINTS, 0, m_VBOData.size()/m_nVertexComponents/2);

        checkError("render");
    }

}
