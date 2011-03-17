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
#include <bouge/Config.hpp>

#include <iostream>

using namespace bougeExample;

Viewer* g_pViewer = 0;

void display()
{
    g_pViewer->display();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    g_pViewer->reshape(w, h);
}

void keyboard(unsigned char key, int x, int y)
{
    g_pViewer->keyboard(key, x, y);
}

void keyboardSpecial(int key, int x, int y)
{
    g_pViewer->keyboard(key, x, y);
}

void mouse(int button, int state, int x, int y)
{
    if(state == GLUT_DOWN)
        g_pViewer->mousedown(button, x, y);
    else
        g_pViewer->mouseup(button, x, y);
}

void motion(int x, int y)
{
    g_pViewer->mousemove(x, y);
}

void byebye()
{
    delete g_pViewer;
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    try {
        std::vector<std::string> args;
        bool bClassic = false;
        for(int i = 0 ; i < argc ; ++i) {
            std::string arg = argv[i];

            // Gotta catch this one here already!
            if(arg == "classic")
                bClassic = true;

            args.push_back(arg);
        }


        if(!bClassic) {
            glutInitContextVersion(3, 1);
            glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
        }

        glutInitWindowSize(500, 500);
        glutInitWindowPosition(100, 100);
        glutCreateWindow(argv[0]);

        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutKeyboardFunc(keyboard);
        glutSpecialFunc(keyboardSpecial);
        glutMouseFunc(mouse);
        glutMotionFunc(motion);
        glutPassiveMotionFunc(motion);
        glutCloseFunc(byebye);
        glutIdleFunc(display);

        g_pViewer = makeViewer(args);
        g_pViewer->init(args);

        glutMainLoop();
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
#ifdef BOUGE_SYSTEM_WINDOWS
        system("pause");
#endif
    }

    return 0;
}
