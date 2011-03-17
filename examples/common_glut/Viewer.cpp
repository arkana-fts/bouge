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
#include "Viewer.hpp"

#include <bouge/Math/Vector.hpp>
#include <bouge/Math/Util.hpp>
#include <bouge/Util.hpp>

#include <sstream>
#include <iostream>
#include <stdexcept>

PFNGLGENBUFFERSPROC gl_GenBuffers;
PFNGLDELETEBUFFERSPROC gl_DeleteBuffers;
PFNGLBINDBUFFERPROC gl_BindBuffer;
PFNGLBUFFERDATAPROC gl_BufferData;
PFNGLBINDVERTEXARRAYPROC gl_BindVertexArray;
PFNGLDELETEVERTEXARRAYSPROC gl_DeleteVertexArrays;
PFNGLGENVERTEXARRAYSPROC gl_GenVertexArrays;

PFNGLENABLEVERTEXATTRIBARRAYPROC gl_EnableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC gl_VertexAttribPointer;

#ifdef BOUGE_SYSTEM_WINDOWS
PFNWGLSWAPINTERVALEXTPROC wgl_SwapInterval;
#endif

using namespace bouge;

namespace bougeExample
{
    Camera::Camera()
        : m_orbitH(0.0f)
        , m_orbitV(0.0f)
        , m_zoom(0.0f)
    { }

    Camera::~Camera()
    { }

    Camera& Camera::resize(int w, int h)
    {
        if(w <= h) {
            m_Projection = General4x4Matrix::perspectiveProjection(45.0f, (float)h/(float)w, 0.01f, 100.0f);
        } else {
            m_Projection = General4x4Matrix::perspectiveProjection(45.0f, (float)w/(float)h, 0.01f, 100.0f);
        }

        return *this;
    }

    Camera& Camera::mouseRotHori(float radians)
    {
        m_orbitH += radians;
//         m_View *= AffineMatrix::rotationY(radians);
        return this->updateView();
    }

    Camera& Camera::mouseRotVert(float radians)
    {
        m_orbitV += radians;
//         m_View *= AffineMatrix::rotationX(radians);
        return this->updateView();
    }

    Camera& Camera::mouseZoom(float zoomIn)
    {
        m_zoom += zoomIn;
//         m_View = AffineMatrix::translation(0, 0, zoomIn) * m_View;
        return this->updateView();
    }

    Camera& Camera::updateView()
    {
        m_View = AffineMatrix::translation(0, 0, m_zoom) * AffineMatrix::rotationY(m_orbitH) * AffineMatrix::rotationX(m_orbitV);
        return *this;
    }

    General4x4Matrix Camera::proj() const
    {
        return m_Projection;
    }

    AffineMatrix Camera::view() const
    {
        return m_View;
    }

    General4x4Matrix Camera::viewproj() const
    {
        return m_Projection * m_View;
    }

    float Camera::orbitH() const
    {
        return m_orbitH;
    }

    float Camera::orbitV() const
    {
        return m_orbitV;
    }

    float Camera::zoom() const
    {
        return m_zoom;
    }

    void* my_getProcAddress(const char* name)
    {
        return bicali::getEntryPoint(name);
    }

    void initExtensionEntries()
    {
        gl_GenBuffers = (PFNGLGENBUFFERSPROC)my_getProcAddress("glGenBuffers");
        gl_DeleteBuffers = (PFNGLDELETEBUFFERSPROC)my_getProcAddress("glDeleteBuffers");
        gl_BindBuffer = (PFNGLBINDBUFFERPROC)my_getProcAddress("glBindBuffer");
        gl_BufferData = (PFNGLBUFFERDATAPROC)my_getProcAddress("glBufferData");
        gl_BindVertexArray = (PFNGLBINDVERTEXARRAYPROC)my_getProcAddress("glBindVertexArray");
        gl_DeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)my_getProcAddress("glDeleteVertexArrays");
        gl_GenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)my_getProcAddress("glGenVertexArrays");
        gl_EnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)my_getProcAddress("glEnableVertexAttribArray");
        gl_VertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)my_getProcAddress("glVertexAttribPointer");
#ifdef BOUGE_SYSTEM_WINDOWS
        wgl_SwapInterval = (PFNWGLSWAPINTERVALEXTPROC)my_getProcAddress("wglSwapIntervalEXT");
#endif
    }

    Viewer::Viewer(const std::vector<std::string>& args)
        : m_txtInfo(0)
        , m_bUseInfoTxt(true)
        , m_bDrawCSys(true)
        , m_bLeftMouseDown(false)
        , m_bMiddleMouseDown(false)
        , m_bRightMouseDown(false)
        , m_nFrames(0)
        , m_nMillisec(0)
        , m_nLastTick(0)
    {
        initExtensionEntries();

        std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
        checkError("dumpInfo");

        // Init the font system:
        bicali::init();

        // Zoom out by 10 meters.
        m_cam.mouseZoom(-10.0f);
//         m_cam.mouseRotVert(deg2rad*-90.0f);
    }

    Viewer::~Viewer()
    {
        gl_DeleteBuffers(1, &m_csysVBO);
        bicali::deinit();
        checkError("Viewer::~Viewer");
    }

    void Viewer::init(std::vector<std::string>& args)
    {
#ifdef _WIN32
        // Disable vsync which is on by default on windows...
        wgl_SwapInterval(0);
#endif

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        this->loadShaders();
        this->loadCsys();
        this->useInfoText("");

        m_nLastTick = m_nMillisec = glutGet(GLUT_ELAPSED_TIME);
    }

    void Viewer::loadShaders()
    {
        m_pColorOnlyShader = ShaderPtr(new bicali::detail::Shader(
            "#version 140\n"
            "uniform mat4 uModelViewProjectionMatrix;\n"
            "in vec3 aVertex;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = uModelViewProjectionMatrix * vec4(aVertex, 1.0);\n"
            "}\n"
            ,
            "#version 140\n"
            "uniform vec4 uColor;\n"
            "out vec4 oFragColor;\n"
            "void main(void)\n"
            "{\n"
            "   oFragColor = uColor;\n"
            "}\n"
            ));
    }

    void Viewer::loadCsys()
    {
        float fCsysData[] = {
             0.0f, 0.0f, 0.0f,
             10.0f, 0.0f, 0.0f,
             0.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
             0.0f, 0.0f, 0.0f,
             0.0f, 10.0f, 0.0f,
             0.0f, 0.0f, 0.0f,
             0.0f,-1.0f, 0.0f,
             0.0f, 0.0f, 0.0f,
             0.0f, 0.0f, 10.0f,
             0.0f, 0.0f, 0.0f,
             0.0f, 0.0f,-1.0f,
        };

        gl_GenBuffers(1, &m_csysVBO);
        gl_BindBuffer(GL_ARRAY_BUFFER, m_csysVBO);
        gl_BufferData(GL_ARRAY_BUFFER, sizeof(fCsysData), fCsysData, GL_STATIC_DRAW);
    }

    void Viewer::reshape(int w, int h)
    {
        m_cam.resize(w, h);
        bicali::viewport(/*0, 0,*/ w, h);
    }

    void Viewer::keyboard(int key, int x, int y)
    {
        if(key == GLUT_KEY_F1)
            m_bUseInfoTxt = !m_bUseInfoTxt;
        else if(key == GLUT_KEY_F2)
            m_bDrawCSys = !m_bDrawCSys;
    }

    void Viewer::mousedown(int button, int x, int y)
    {
        if(button == GLUT_LEFT_BUTTON) {
            m_bLeftMouseDown = true;
        } else if(button == GLUT_MIDDLE_BUTTON) {
            m_bMiddleMouseDown = true;
        } else if(button == GLUT_RIGHT_BUTTON) {
            m_bRightMouseDown = true;
        } else if(button == 3) {
            m_cam.mouseZoom(0.5f);
        } else if(button == 4) {
            m_cam.mouseZoom(-0.5f);
        } else {
            std::cout << "Mouse down: " << button << std::endl;
        }
    }

    void Viewer::mouseup(int button, int x, int y)
    {
        if(button == GLUT_LEFT_BUTTON) {
            m_bLeftMouseDown = false;
        } else if(button == GLUT_MIDDLE_BUTTON) {
            m_bMiddleMouseDown = false;
        } else if(button == GLUT_RIGHT_BUTTON) {
            m_bRightMouseDown = false;
        }
    }

    void Viewer::mousemove(int x, int y)
    {
        static int oldx = x, oldy = y;
        float dx = static_cast<float>(x - oldx);
        float dy = static_cast<float>(y - oldy);

        if(m_bLeftMouseDown) {
            m_cam.mouseRotHori(dx*deg2rad);
            m_cam.mouseRotVert(dy*deg2rad);
        }

        if(m_bMiddleMouseDown) {
            float scale = m_cam.zoom() * 0.001f;
            m_Model *= AffineMatrix::translation(-dx*scale*m_cam.view().right());
            m_Model *= AffineMatrix::translation(dy*scale*m_cam.view().up());
        }

        if(m_bRightMouseDown) {
            m_cam.mouseZoom(dy*0.1f);
        }

        oldx = x;
        oldy = y;
    }

    void Viewer::display()
    {
        unsigned int now = glutGet(GLUT_ELAPSED_TIME);
        float dt = (float)(now - m_nLastTick)*0.001f;
        m_nLastTick = now;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        this->update(dt);
        this->render(m_cam, m_Model);
        this->drawText();
        glFlush();

        // FPS:
        m_nFrames++;
        unsigned int msPassed = now - m_nMillisec;
        if(msPassed > 1000) {
            float fps = (float)m_nFrames / ((float)msPassed/1000.0f);
            std::cout << "FPS: " << fps << std::endl;

            m_nMillisec = glutGet(GLUT_ELAPSED_TIME);
            m_nFrames = 0;
        }

        checkError("display");
    }

    void Viewer::update(float)
    { }

    void Viewer::checkError(const std::string& where)
    {
        GLenum error;
        while( (error = glGetError()) != GL_NO_ERROR) {
            std::cerr << "GL error 0x" << std::hex << error << " detected in " << where << std::endl;
        }
    }

    void Viewer::useInfoText(const std::string& text)
    {
        bicali::default_small_font()->free(m_txtInfo);
        m_txtInfo = bicali::default_small_font()->bufferUtf8(text.c_str(), 5, 5);
    }

    void Viewer::drawText()
    {
        if(!m_bUseInfoTxt)
            return;

        m_txtInfo->draw();
    }

    std::string clearExt(std::string fname, std::string ext)
    {
        if(fname.length() > ext.length() && fname.substr(fname.length()-ext.length()) == ext) {
            return fname.substr(0, fname.length()-ext.length());
        }

        return fname;
    }

    std::string Viewer::clearBougeExt(std::string filename)
    {
        std::string ret = filename;
        ret = clearExt(ret, ".bxskel");
        ret = clearExt(ret, ".bxmesh");
        ret = clearExt(ret, ".bxmat");
        ret = clearExt(ret, ".bxmset");
        ret = clearExt(ret, ".bxanim");
        return ret;
    }

    void Viewer::render(const Camera& cam, const bouge::AffineMatrix& model) const
    {
        // This is a small, but effective, optimization that unfortunately GCC doesn't do.
        // Well, it can't do, as it isn't thread-safe!
        static const std::string uModelViewProjectionMatrix = "uModelViewProjectionMatrix";
        static const std::string uModelViewMatrix = "uModelViewMatrix";
        static const std::string aVertex = "aVertex";
        static const std::string uColor = "uColor";
        static const float red[] = {1.0f, 0.0f, 0.0f, 1.0f};
        static const float darkred[] = {0.5f, 0.0f, 0.0f, 1.0f};
        static const float green[] = {0.0f, 1.0f, 0.0f, 1.0f};
        static const float darkgreen[] = {0.0f, 0.5f, 0.0f, 1.0f};
        static const float blue[] = {0.0f, 0.0f, 1.0f, 1.0f};
        static const float darkblue[] = {0.0f, 0.0f, 0.5f, 1.0f};

        if(!m_bDrawCSys)
            return;

        m_pColorOnlyShader->use();

        m_pColorOnlyShader->uniformMatrix4fv(uModelViewProjectionMatrix, 1, false, cam.viewproj().array16f());
        m_pColorOnlyShader->uniformMatrix4fv(uModelViewMatrix, 1, false, cam.view().array16f());

        // VERY inefficient drawing of a coordinate system :)
        gl_BindBuffer(GL_ARRAY_BUFFER, m_csysVBO);
        gl_BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        gl_EnableVertexAttribArray(m_pColorOnlyShader->attrib(aVertex));
        gl_VertexAttribPointer(m_pColorOnlyShader->attrib(aVertex), 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
        m_pColorOnlyShader->uniform4fv(uColor, 1, red);
        glDrawArrays(GL_LINES, 0, 2);

        gl_EnableVertexAttribArray(m_pColorOnlyShader->attrib(aVertex));
        gl_VertexAttribPointer(m_pColorOnlyShader->attrib(aVertex), 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(6*sizeof(float)));
        m_pColorOnlyShader->uniform4fv(uColor, 1, darkred);
        glDrawArrays(GL_LINES, 0, 2);

        gl_EnableVertexAttribArray(m_pColorOnlyShader->attrib(aVertex));
        gl_VertexAttribPointer(m_pColorOnlyShader->attrib(aVertex), 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(12*sizeof(float)));
        m_pColorOnlyShader->uniform4fv(uColor, 1, green);
        glDrawArrays(GL_LINES, 0, 2);

        gl_EnableVertexAttribArray(m_pColorOnlyShader->attrib(aVertex));
        gl_VertexAttribPointer(m_pColorOnlyShader->attrib(aVertex), 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(18*sizeof(float)));
        m_pColorOnlyShader->uniform4fv(uColor, 1, darkgreen);
        glDrawArrays(GL_LINES, 0, 2);

        gl_EnableVertexAttribArray(m_pColorOnlyShader->attrib(aVertex));
        gl_VertexAttribPointer(m_pColorOnlyShader->attrib(aVertex), 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(24*sizeof(float)));
        m_pColorOnlyShader->uniform4fv(uColor, 1, blue);
        glDrawArrays(GL_LINES, 0, 2);

        gl_EnableVertexAttribArray(m_pColorOnlyShader->attrib(aVertex));
        gl_VertexAttribPointer(m_pColorOnlyShader->attrib(aVertex), 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(30*sizeof(float)));
        m_pColorOnlyShader->uniform4fv(uColor, 1, darkblue);
        glDrawArrays(GL_LINES, 0, 2);

        gl_BindBuffer(GL_ARRAY_BUFFER, 0);
        gl_BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        checkError("Viewer::render");
    }
}

