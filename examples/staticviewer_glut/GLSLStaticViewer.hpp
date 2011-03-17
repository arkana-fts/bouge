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
#ifndef D_BOUGEEXAMPLE_GLSLSTATICVIEWER_HPP
#define D_BOUGEEXAMPLE_GLSLSTATICVIEWER_HPP

#include "Viewer.hpp"

namespace bougeExample {

    class GLSLStaticViewer : public Viewer
    {
    public:
        GLSLStaticViewer(std::vector<std::string>& args);
        virtual ~GLSLStaticViewer();

        virtual void init(std::vector<std::string>& args);
    protected:
        virtual void render(const Camera& cam, const bouge::AffineMatrix& model) const;
        virtual void keyboard(int key, int x, int y);

        void loadShaders();
        void loadMesh(std::string filename);
        void loadHardwareMesh(bool stride);

        void playNextAnim();
        void playPrevAnim();

        std::string makeInfoText() const;

        bool m_bStride;

        int m_nVertexComponents;
        int m_stride;

    private:
        bouge::CoreModelPtr m_model;
        bouge::StaticModelInstancePtr m_modelInst;
        bouge::CoreHardwareMeshPtr m_hwmesh;

        GLuint m_VBOIds[2];
        GLuint m_VAOIds[1];

        ShaderPtr m_shaderNoTexture;
        ShaderPtr m_shaderWithTexture;
        ShaderPtr m_shaderToUse;
    };

    Viewer* makeViewer(std::vector<std::string>& args);

}

#endif // D_BOUGEEXAMPLE_GLSLSTATICVIEWER_HPP
