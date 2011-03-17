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
#ifndef BOUGE_CONFIG_HPP
#define BOUGE_CONFIG_HPP

// Note: This file is heavily inspired/copied from the SFML project.

////////////////////////////////////////////////////////////
// Some compile-time options on bouge.
////////////////////////////////////////////////////////////
// In case you don't use it and want to save some memory, define this to 0 in your preprocessor.
#ifndef BOUGE_USE_USERDATA
    #define BOUGE_USE_USERDATA 1
#endif

// Choose whatever you want here. The OpenGL docs say unsigned short might be faster..
// Choose by adding "BOUGE_FACE_INDEX_UINT" or not to your preprocessor.
#ifndef BOUGE_FACE_INDEX_TYPE
    #ifdef BOUGE_FACE_INDEX_UINT
        #define BOUGE_FACE_INDEX_TYPE unsigned int
    #else
        #define BOUGE_FACE_INDEX_TYPE unsigned short
    #endif
#endif

////////////////////////////////////////////////////////////
// Define the Bouge version
////////////////////////////////////////////////////////////
#define BOUGE_VERSION_MAJOR 0
#define BOUGE_VERSION_MINOR 5


////////////////////////////////////////////////////////////
// Identify the operating system
////////////////////////////////////////////////////////////
#if defined(_WIN32) || defined(__WIN32__)

    // Windows
    #define BOUGE_SYSTEM_WINDOWS
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif

#elif defined(linux) || defined(__linux)

    // Linux
    #define BOUGE_SYSTEM_LINUX

#elif defined(__APPLE__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh)

    // MacOS
    #define BOUGE_SYSTEM_MACOS

#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)

    // FreeBSD
    #define BOUGE_SYSTEM_FREEBSD

#else

    // Unsupported system
    #error This operating system is not supported by Bouge library

#endif


////////////////////////////////////////////////////////////
// Identify the endianess
////////////////////////////////////////////////////////////
#if defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || (defined(__MIPS__) && defined(__MISPEB__)) || \
    defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || defined(__sparc__) || defined(__hppa__)

    // Big endian
    #define BOUGE_ENDIAN_BIG

#else

    // Little endian
    #define BOUGE_ENDIAN_LITTLE

#endif


////////////////////////////////////////////////////////////
// Identify if C++0x can be used.
////////////////////////////////////////////////////////////
#ifdef _MSC_VER
    #if _MSC_VER >= 1600  // Visual Studio 2010 or newer

        #define BOUGE_CPP0X

    #endif

#elif defined(__GXX_EXPERIMENTAL_CXX0X__) // Thank you, Gcc

    #define BOUGE_CPP0X

#endif

////////////////////////////////////////////////////////////
// Define a portable debug macro
////////////////////////////////////////////////////////////
#if !defined(NDEBUG)

    #define BOUGE_DEBUG

#endif


////////////////////////////////////////////////////////////
// Define portable import / export macros
////////////////////////////////////////////////////////////
#if defined(BOUGE_SYSTEM_WINDOWS) && !defined(BOUGE_STATIC)

    #ifdef BOUGE_EXPORT

        // From DLL side, we must export
        #define BOUGE_API __declspec(dllexport)

    #else

        // From client application side, we must import
        #define BOUGE_API __declspec(dllimport)

    #endif

    // For Visual C++ compilers, we also need to turn off this annoying C4251 warning.
    // You can read lots ot different things about it, but the point is the code will
    // just work fine, and so the simplest way to get rid of this warning is to disable it
    #ifdef _MSC_VER

        #pragma warning(disable : 4251)

    #endif

#else

    // Other platforms and static build don't need these export macros
    #define BOUGE_API

#endif


////////////////////////////////////////////////////////////
// Include the STL shared (smart) pointer, either from C++0x or from the TR1.
////////////////////////////////////////////////////////////
#ifdef BOUGE_CPP0X
    // If C++0x is supported, it is easy:

    #include <memory>

    namespace bouge {
        template<class T>
        struct shared_ptr {
            typedef std::shared_ptr<T> type;
        };

        template<class T>
        struct enable_shared_from_this {
            typedef std::enable_shared_from_this<T> type;
        };
    }

#else
    // If not, it gets hairy.

    #ifdef _MSC_VER
        // Visual Studio 2008 has it in memory, but in the namespace std::tr1

        #include <memory>

        namespace bouge {
            template<class T>
            struct shared_ptr {
                typedef std::tr1::shared_ptr<T> type;
            };

            template<class T>
            struct enable_shared_from_this {
                typedef std::tr1::enable_shared_from_this<T> type;
            };
        }

    #elif __GNUC__ > 3 // This is probably a too simple version check.

        #include <tr1/memory>

        namespace bouge {
            template<class T>
            struct shared_ptr {
                typedef std::tr1::shared_ptr<T> type;
            };

            template<class T>
            struct enable_shared_from_this {
                typedef std::tr1::enable_shared_from_this<T> type;
            };
        }

    #else // If all this fails, damn this must be some old system! Try out boost.

        // If this line gives you compile errors, your system is OLD! or unsupported
        // Better try one of the above.
        #include <boost/shared_ptr.hpp>

        namespace bouge {
            template<class T>
            struct shared_ptr {
                typedef boost::shared_ptr<T> type;
            };

            template<class T>
            struct enable_shared_from_this {
                typedef boost::enable_shared_from_this<T> type;
            };
        }

    #endif


#endif

#endif // BOUGE_CONFIG_HPP
