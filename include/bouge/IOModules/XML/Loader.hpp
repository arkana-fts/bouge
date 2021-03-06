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
#ifndef BOUGE_XMLLOADER_HPP
#define BOUGE_XMLLOADER_HPP

#include <bouge/Loader.hpp>
#include <bouge/IOModules/XMLParserCommon/XMLParser.hpp>

namespace bouge {

    /// Override this class and implement all its pure virtuals if you want to
    /// load models from a different file format.
    class BOUGE_API XMLLoader : public Loader
    {
    public:
        XMLLoader(XMLParser* parser);

        /// Default destructor.
        virtual ~XMLLoader();

        virtual CoreMeshPtr loadMesh(const std::string& sFileName);
        virtual CoreMeshPtr loadMesh(const void* pData, std::size_t size = (std::size_t)-1);

        virtual CoreSkeletonPtr loadSkeleton(const std::string& sFileName);
        virtual CoreSkeletonPtr loadSkeleton(const void* pData, std::size_t size = (std::size_t)-1);

        virtual std::vector<CoreMaterialPtr> loadMaterial(const std::string& sFileName);
        virtual std::vector<CoreMaterialPtr> loadMaterial(const void* pData, std::size_t size = (std::size_t)-1);

        virtual std::vector<CoreMaterialSetPtr> loadMaterialSet(const std::string& sFileName);
        virtual std::vector<CoreMaterialSetPtr> loadMaterialSet(const void* pData, std::size_t size = (std::size_t)-1);

        virtual std::vector<CoreAnimationPtr> loadAnimation(const std::string& sFileName);
        virtual std::vector<CoreAnimationPtr> loadAnimation(const void* pData, std::size_t size = (std::size_t)-1);
    private:
        XMLParser* m_parser;
    };

} // namespace bouge

#endif // BOUGE_XMLLOADER_HPP
