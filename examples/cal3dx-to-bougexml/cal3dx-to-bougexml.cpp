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

#include <bouge/IOModules/Cal3dX/Loader.hpp>
#include <bouge/IOModules/XML/Saver.hpp>
#include <bouge/IOModules/XMLParserCommon/XMLParserModules/TinyXMLParser.hpp>

#include <iostream>

using namespace bouge;

int main(int argc, const char* argv[])
{
    if(argc % 2 != 1) {
        std::cout << "Usage: " << argv[0] << " [-s SKELETONFILE] [-m MESHFILE] [-r MATERIALFILE] [-a ANIMATIONFILE]" << std::endl;
        std::cout << std::endl;
        std::cout << "Will read the contents of the file(s) in the Cal3dX format" << std::endl;
        std::cout << "and store them in Bouge's XML format in files with the same" << std::endl;
        std::cout << "name plus an added bouge extension." << std::endl;
        std::cout << "" << std::endl;
        std::cout << "It is recommended to put the skeleton file in front of the" << std::endl;
        std::cout << "others because in Cal3dX, the others need to resolve bone names." << std::endl;
        std::cout << "" << std::endl;
        std::cout << "You may also specify multiple groups of files to be exported." << std::endl;
        std::cout << "They will be worked on sequentially. For example the following is valid:" << std::endl;
        std::cout << "> cal3dx_to_bougexml -s warrior.xsf -m warrior.xmf -r warrior.xrf -a walk.xaf -a hit.xaf -s mage.xsf -m mage.xmf" << std::endl;
        return 0;
    }

    Cal3DXLoader loader(new TinyXMLParser());
    XMLSaver saver;

    std::vector<std::string> args;
    for(int i = 1 ; i < argc ; ++i) {
        args.push_back(argv[i]);
    }

    for(std::vector<std::string>::iterator i = args.begin() ; i != args.end() ; ) {
        std::string type = *i++;
        std::string fname = *i++;

        try {
            if(type == "-s") {
                // We want to load a skeleton.
                std::cout << "Skeleton: " << fname << " -> ";
                std::cout.flush();
                CoreSkeletonPtr skel = loader.loadSkeleton(fname);

                // Now put that into a file with an added extension.
                saver.saveSkeleton(skel, fname + ".bxskel");

                std::cout << fname << ".bxskel" << std::endl;
            } else if(type == "-m") {
                // We want to load a mesh.
                std::cout << "Mesh: " << fname << " -> ";
                std::cout.flush();
                CoreMeshPtr mesh = loader.loadMesh(fname);

                // Now put that into a file with an added extension.
                saver.saveMesh(mesh, fname + ".bxmesh");

                std::cout << fname << ".bxmesh" << std::endl;
            } else if(type == "-r") {
                // We want to load a mesh.
                std::cout << "Material: " << fname << " -> ";
                std::cout.flush();
                std::vector<CoreMaterialPtr> mats = loader.loadMaterial(fname);

                // Now put that into a file with an added extension.
                // It *should* be just one, but who knows...
                saver.saveMaterials(mats, fname + ".bxmat");

                std::cout << fname << ".bxmat" << std::endl;
            } else if(type == "-a") {
                // We want to load a mesh.
                std::cout << "Animation: " << fname << " -> ";
                std::cout.flush();
                std::vector<CoreAnimationPtr> anims = loader.loadAnimation(fname);

                // Now put that into a file with an added extension.
                // It *should* be just one, but who knows...
                saver.saveAnimations(anims, fname + ".bxanim");

                std::cout << fname << ".bxanim" << std::endl;
            }
        } catch(const std::exception& err) {
            std::cerr << "Error: " << err.what() << std::endl;
        }

        /// \TODO Material name either not stored or not created if inexistent!
        /// \TODO in the end, create a material set with all materials.
    }

    return 0;
}
