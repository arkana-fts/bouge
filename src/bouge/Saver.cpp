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
#include <bouge/Saver.hpp>

#include <fstream>
#include <sstream>

namespace bouge
{

    Saver::Saver()
    { }

    Saver::~Saver()
    { }

    void Saver::saveMesh(CoreMeshPtrC mesh, const std::string& sFileName)
    {
        this->storeToFile(sFileName, this->saveMesh(mesh));
    }

    void Saver::saveSkeleton(CoreSkeletonPtrC skel, const std::string& sFileName)
    {
        this->storeToFile(sFileName, this->saveSkeleton(skel));
    }

    void Saver::saveMaterial(CoreMaterialPtrC mat, const std::string& sFileName)
    {
        this->storeToFile(sFileName, this->saveMaterial(mat));
    }

    void Saver::saveMaterials(const std::vector<CoreMaterialPtrC>& mats, const std::string& sFileName)
    {
        this->storeToFile(sFileName, this->saveMaterials(mats));
    }

    void Saver::saveMaterials(const std::vector<CoreMaterialPtr>& mats, const std::string& sFileName)
    {
        this->storeToFile(sFileName, this->saveMaterials(mats));
    }

    void Saver::saveMaterialSet(CoreMaterialSetPtrC matset, const std::string& sFileName)
    {
        this->storeToFile(sFileName, this->saveMaterialSet(matset));
    }

    void Saver::saveMaterialSets(const std::vector<CoreMaterialSetPtrC>& matsets, const std::string& sFileName)
    {
        this->storeToFile(sFileName, this->saveMaterialSets(matsets));
    }

    void Saver::saveMaterialSets(const std::vector<CoreMaterialSetPtr>& matsets, const std::string& sFileName)
    {
        this->storeToFile(sFileName, this->saveMaterialSets(matsets));
    }

    void Saver::saveAnimation(CoreAnimationPtrC anim, const std::string& sFileName)
    {
        this->storeToFile(sFileName, this->saveAnimation(anim));
    }

    void Saver::saveAnimations(const std::vector<CoreAnimationPtrC>& anims, const std::string& sFileName)
    {
        this->storeToFile(sFileName, this->saveAnimations(anims));
    }

    void Saver::saveAnimations(const std::vector<CoreAnimationPtr>& anims, const std::string& sFileName)
    {
        this->storeToFile(sFileName, this->saveAnimations(anims));
    }

    std::vector<char> Saver::toData(const std::stringstream& ss)
    {
        std::string outString = ss.str();
        std::vector<char> outVec(outString.size());
        outVec.assign(outString.begin(), outString.end());
        return outVec;
    }

    void Saver::storeToFile(const std::string& sFileName, const std::vector<char>& data)
    {
        std::ofstream(sFileName.c_str(), std::ios::binary).write(&data[0], data.size());
    }

} // namespace bouge
