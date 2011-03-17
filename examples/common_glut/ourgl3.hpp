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
#ifndef D_OURGL3_HPP
#define D_OURGL3_HPP

#include <GL/freeglut.h>

// extension #defines, types and entries, avoiding a dependency on additional libraries like GLEW or the GL/glext.h header
#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER 0x8892
#endif

#ifndef GL_STATIC_DRAW
#define GL_STATIC_DRAW 0x88E4
#endif

#ifndef GL_DYNAMIC_DRAW
#define GL_DYNAMIC_DRAW 0x88E8
#endif

#ifndef GL_SHADING_LANGUAGE_VERSION
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#endif

typedef ptrdiff_t ourGLsizeiptr;

#if defined(WIN32)
#ifndef APIENTRY
#define APIENTRY
#endif

typedef void (APIENTRY *PFNGLGENBUFFERSPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY *PFNGLDELETEBUFFERSPROC) (GLsizei n, GLuint *buffers);
typedef void (APIENTRY *PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY *PFNGLBUFFERDATAPROC) (GLenum target, ourGLsizeiptr size, const GLvoid *data, GLenum usage);
typedef void (APIENTRY *PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void (APIENTRY *PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint *arrays);
typedef void (APIENTRY *PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint *arrays);

typedef void (APIENTRY *PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (APIENTRY *PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);

#ifndef GL_ELEMENT_ARRAY_BUFFER
    #define GL_ELEMENT_ARRAY_BUFFER           0x8893
#endif

typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALEXTPROC) (int interval);
#endif // defined(_WIN32)

extern PFNGLGENBUFFERSPROC gl_GenBuffers;
extern PFNGLDELETEBUFFERSPROC gl_DeleteBuffers;
extern PFNGLBINDBUFFERPROC gl_BindBuffer;
extern PFNGLBUFFERDATAPROC gl_BufferData;
extern PFNGLBINDVERTEXARRAYPROC gl_BindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC gl_DeleteVertexArrays;
extern PFNGLGENVERTEXARRAYSPROC gl_GenVertexArrays;

extern PFNGLENABLEVERTEXATTRIBARRAYPROC gl_EnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC gl_VertexAttribPointer;

#ifdef _WIN32
extern PFNWGLSWAPINTERVALEXTPROC wgl_SwapInterval;
#endif // defined(_WIN32)

#endif // D_OURGL3_HPP
