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
#ifndef D_BOUGEEXAMPLE_GLSLSKELETONVIEWER_HPP
#define D_BOUGEEXAMPLE_GLSLSKELETONVIEWER_HPP

#include "Viewer.hpp"

#include <vector>

namespace bougeExample {

    class GLSLSkeletonViewer : public Viewer
    {
    public:
        GLSLSkeletonViewer(std::vector<std::string>& args);
        virtual ~GLSLSkeletonViewer();

        virtual void init(std::vector<std::string>& args);
        virtual void keyboard(int key, int x, int y);

    protected:
        virtual void update(float dt);
        virtual void render(const Camera& cam, const bouge::AffineMatrix& model) const;

        void loadSkeleton(std::string filename);
        void loadAnimation(std::string filename);

        void playNextAnim();
        void playPrevAnim();

        std::string makeInfoText() const;

        int m_nVertexComponents;
        int m_stride;

    private:
        std::vector<bouge::CoreAnimationPtr> m_anim;
        std::vector<bouge::CoreAnimationPtr>::iterator m_currAnim;
        bouge::SkeletonInstancePtr m_skeleton;
        bouge::MixerPtr m_mixer;
        std::vector<float> m_VBOData;
        GLuint m_VBOId;
    };

    Viewer* makeViewer(std::vector<std::string>& args);

}

#endif // D_BOUGEEXAMPLE_GLSLSKELETONVIEWER_HPP
