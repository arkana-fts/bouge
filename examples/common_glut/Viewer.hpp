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
#ifndef D_BOUGEEXAMPLE_VIEWER_HPP
#define D_BOUGEEXAMPLE_VIEWER_HPP

#include "ourgl3.hpp"

#include <bouge/bougefwd.hpp>
#include <bicali/bicali.hpp>

#include <bouge/Math/Matrix.hpp>

#include <string>
#include <map>
#include <vector>

namespace bougeExample {

    typedef bouge::shared_ptr<bicali::detail::Shader>::type ShaderPtr;

    class Camera {
    public:
        Camera();
        ~Camera();

        Camera& resize(int w, int h);
        Camera& mouseRotHori(float radians);
        Camera& mouseRotVert(float radians);
        Camera& mouseZoom(float zoomIn);

        bouge::General4x4Matrix proj() const;
        bouge::AffineMatrix view() const;
        bouge::General4x4Matrix viewproj() const;

        float orbitH() const;
        float orbitV() const;
        float zoom() const;

    protected:
        Camera& updateView();

    private:
        bouge::General4x4Matrix m_Projection;
        bouge::AffineMatrix m_View;

        float m_orbitH;
        float m_orbitV;
        float m_zoom;
    };

    class Viewer
    {
    public:
        Viewer(const std::vector<std::string>& args);
        virtual ~Viewer();

        virtual void init(std::vector<std::string>& args);
        virtual void keyboard(int key, int x, int y);
        virtual void mousedown(int button, int x, int y);
        virtual void mouseup(int button, int x, int y);
        virtual void mousemove(int x, int y);
        void display();
        void reshape(int w, int h);

    protected:
        virtual void update(float dt);
        virtual void render(const Camera& cam, const bouge::AffineMatrix& model) const;

        static void checkError(const std::string& where);

        void useInfoText(const std::string& text);
        void drawText();

        static std::string clearBougeExt(std::string filename);

        ShaderPtr m_pColorOnlyShader;

    private:
        GLuint m_csysVBO;
        GLuint m_csysVAO;

        bicali::StaticText* m_txtInfo;
        bool m_bUseInfoTxt;
        bool m_bDrawCSys;

        void loadShaders();
        void loadCsys();

        bool m_bLeftMouseDown;
        bool m_bMiddleMouseDown;
        bool m_bRightMouseDown;

        Camera m_cam;
        bouge::AffineMatrix m_Model;

        // FPS stuff:
        unsigned int m_nFrames;
        unsigned int m_nMillisec;
        unsigned int m_nLastTick;
    };

    Viewer* makeViewer(std::vector<std::string>& args);

}

#endif // D_BOUGEEXAMPLE_VIEWER_HPP
