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
#include <bouge/bouge.hpp>

#include <bouge/IOModules/XML/Loader.hpp>
#include <bouge/IOModules/XML/Saver.hpp>
#include <bouge/IOModules/XMLParserCommon/XMLParserModules/TinyXMLParser.hpp>

#include <iostream>

using namespace bouge;

int main(int argc, const char* argv[])
{
    if(argc <= 1) {
        std::cout << "Usage: " << argv[0] << " MESH1 [MESH2 ... MESHN]" << std::endl;
        std::cout << std::endl;
        std::cout << "Will read the contents of the file(s) one after" << std::endl;
        std::cout << "the other and print the default-XML tree they create" << std::endl;
    }

    XMLLoader loader(new TinyXMLParser());
    XMLSaver saver;

    for(int i = 1 ; i < argc ; ++i) {
        try {
            CoreMeshPtr mesh = loader.loadMesh(argv[i]);

            // Tricky printing, I agree it's not "nice" :)
            std::cout << (const char*)&(saver.saveMesh(mesh)[0]) << std::endl;
        } catch(const std::exception& err) {
            std::cerr << err.what() << std::endl;
        }
    }

    return 0;
}
