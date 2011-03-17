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
#ifndef BOUGE_XMLSAVER_HPP
#define BOUGE_XMLSAVER_HPP

#include <bouge/Saver.hpp>

namespace bouge {

    /// Override this class and implement all its pure virtuals if you want to
    /// load models from a different file format.
    class BOUGE_API XMLSaver : public Saver
    {
    public:
        XMLSaver();

        /// Default destructor.
        virtual ~XMLSaver();

        virtual void saveMesh(CoreMeshPtrC mesh, const std::string& sFileName);
        virtual std::vector<char> saveMesh(CoreMeshPtrC mesh);

        virtual void saveSkeleton(CoreSkeletonPtrC skel, const std::string& sFileName);
        virtual std::vector<char> saveSkeleton(CoreSkeletonPtrC skel);

        virtual void saveMaterial(CoreMaterialPtrC mat, const std::string& sFileName);
        virtual std::vector<char> saveMaterial(CoreMaterialPtrC mat);
        virtual void saveMaterials(const std::vector<CoreMaterialPtrC>& mats, const std::string& sFileName);
        virtual void saveMaterials(const std::vector<CoreMaterialPtr>& mats, const std::string& sFileName);
        virtual std::vector<char> saveMaterials(const std::vector<CoreMaterialPtrC>& mats);
        virtual std::vector<char> saveMaterials(const std::vector<CoreMaterialPtr>& mats);

        virtual void saveMaterialSet(CoreMaterialSetPtrC matset, const std::string& sFileName);
        virtual std::vector<char> saveMaterialSet(CoreMaterialSetPtrC matset);
        virtual void saveMaterialSets(const std::vector<CoreMaterialSetPtrC>& matsets, const std::string& sFileName);
        virtual void saveMaterialSets(const std::vector<CoreMaterialSetPtr>& matsets, const std::string& sFileName);
        virtual std::vector<char> saveMaterialSets(const std::vector<CoreMaterialSetPtrC>& matsets);
        virtual std::vector<char> saveMaterialSets(const std::vector<CoreMaterialSetPtr>& matsets);

        virtual void saveAnimation(CoreAnimationPtrC anim, const std::string& sFileName);
        virtual std::vector<char> saveAnimation(CoreAnimationPtrC anim);
        virtual void saveAnimations(const std::vector<CoreAnimationPtrC>& anims, const std::string& sFileName);
        virtual void saveAnimations(const std::vector<CoreAnimationPtr>& anims, const std::string& sFileName);
        virtual std::vector<char> saveAnimations(const std::vector<CoreAnimationPtrC>& anims);
        virtual std::vector<char> saveAnimations(const std::vector<CoreAnimationPtr>& anims);
    };

} // namespace bouge

#endif // BOUGE_XMLSAVER_HPP
