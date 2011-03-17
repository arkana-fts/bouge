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
#ifndef BOUGE_SAVER_HPP
#define BOUGE_SAVER_HPP

#include <bouge/bougefwd.hpp>

#include <vector>
#include <string>

namespace bouge {

    /// Override this class and implement all its pure virtuals if you want to
    /// load models from a different file format.
    class BOUGE_API Saver
    {
    public:
        /// Default destructor.
        virtual ~Saver();

        virtual void saveMesh(CoreMeshPtrC mesh, const std::string& sFileName);
        virtual std::vector<char> saveMesh(CoreMeshPtrC mesh) = 0;

        virtual void saveSkeleton(CoreSkeletonPtrC skel, const std::string& sFileName);
        virtual std::vector<char> saveSkeleton(CoreSkeletonPtrC skel) = 0;

        virtual void saveMaterial(CoreMaterialPtrC mat, const std::string& sFileName);
        virtual std::vector<char> saveMaterial(CoreMaterialPtrC mat) = 0;
        virtual void saveMaterials(const std::vector<CoreMaterialPtrC>& mats, const std::string& sFileName);
        virtual void saveMaterials(const std::vector<CoreMaterialPtr>& mats, const std::string& sFileName);
        virtual std::vector<char> saveMaterials(const std::vector<CoreMaterialPtrC>& mats) = 0;
        virtual std::vector<char> saveMaterials(const std::vector<CoreMaterialPtr>& mats) = 0;

        virtual void saveMaterialSet(CoreMaterialSetPtrC matset, const std::string& sFileName);
        virtual std::vector<char> saveMaterialSet(CoreMaterialSetPtrC matset) = 0;
        virtual void saveMaterialSets(const std::vector<CoreMaterialSetPtrC>& matsets, const std::string& sFileName);
        virtual void saveMaterialSets(const std::vector<CoreMaterialSetPtr>& matsets, const std::string& sFileName);
        virtual std::vector<char> saveMaterialSets(const std::vector<CoreMaterialSetPtrC>& matsets) = 0;
        virtual std::vector<char> saveMaterialSets(const std::vector<CoreMaterialSetPtr>& matsets) = 0;

        virtual void saveAnimation(CoreAnimationPtrC anim, const std::string& sFileName);
        virtual std::vector<char> saveAnimation(CoreAnimationPtrC anim) = 0;
        virtual void saveAnimations(const std::vector<CoreAnimationPtrC>& anims, const std::string& sFileName);
        virtual void saveAnimations(const std::vector<CoreAnimationPtr>& anims, const std::string& sFileName);
        virtual std::vector<char> saveAnimations(const std::vector<CoreAnimationPtrC>& anims) = 0;
        virtual std::vector<char> saveAnimations(const std::vector<CoreAnimationPtr>& anims) = 0;

    protected:
        Saver();

        std::vector<char> toData(const std::stringstream& ss);
        static void storeToFile(const std::string& sFileName, const std::vector<char>& data);
    };

} // namespace bouge

#endif // BOUGE_SAVER_HPP
