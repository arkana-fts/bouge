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
#include <bouge/IOModules/Cal3dX/Loader.hpp>
#include <bouge/IOModules/Cal3dX/CoreMesh_Handler.hpp>
#include <bouge/IOModules/Cal3dX/CoreSkeleton_Handler.hpp>
#include <bouge/IOModules/Cal3dX/CoreMaterial_Handler.hpp>
#include <bouge/IOModules/Cal3dX/CoreAnimation_Handler.hpp>
#include <bouge/CoreMesh.hpp>
#include <bouge/CoreSkeleton.hpp>

#include <sstream>
#include <fstream>

namespace bouge
{

    Cal3DXLoader::Cal3DXLoader(XMLParser* parser)
        : m_parser(parser)
    { }

    Cal3DXLoader::~Cal3DXLoader()
    {
        delete m_parser;
    }

    CoreMeshPtr Cal3DXLoader::loadMesh(const std::string& sFileName)
    {
        return this->loadMesh(sFileName, m_pLastSkel);
    }

    CoreMeshPtr Cal3DXLoader::loadMesh(const void* pData, std::size_t size)
    {
        return this->loadMesh(pData, size, m_pLastSkel);
    }

    CoreMeshPtr Cal3DXLoader::loadMesh(const std::string& sFileName, CoreSkeletonPtr skeleton)
    {
        CoreMeshPtr ret(new CoreMesh(sFileName));
        CoreMesh_Cal3dXHandler handler(*ret, skeleton);

        // We haven't written a schema yet.
        m_parser->parseXMLFile(handler, sFileName, "");

        return ret;
    }

    CoreMeshPtr Cal3DXLoader::loadMesh(const void* pData, std::size_t size, CoreSkeletonPtr skeleton)
    {
        CoreMeshPtr ret(new CoreMesh(""));
        CoreMesh_Cal3dXHandler handler(*ret, skeleton);

        // We haven't written a schema yet.
        m_parser->parseXMLFileFromMem(handler, std::string(reinterpret_cast<const char*>(pData), size), "");

        return ret;
    }

    CoreSkeletonPtr Cal3DXLoader::loadSkeleton(const std::string& sFileName)
    {
        m_pLastSkel = CoreSkeletonPtr(new CoreSkeleton(sFileName));
        CoreSkeleton_Cal3dXHandler handler(*m_pLastSkel);

        // We haven't written a schema yet.
        m_parser->parseXMLFile(handler, sFileName, "");

        return m_pLastSkel;
    }

    CoreSkeletonPtr Cal3DXLoader::loadSkeleton(const void* pData, std::size_t size)
    {
        m_pLastSkel = CoreSkeletonPtr(new CoreSkeleton(""));
        CoreSkeleton_Cal3dXHandler handler(*m_pLastSkel);

        // We haven't written a schema yet.
        m_parser->parseXMLFileFromMem(handler, std::string(reinterpret_cast<const char*>(pData), size), "");

        return m_pLastSkel;
    }

    std::vector<CoreMaterialPtr> Cal3DXLoader::loadMaterial(const std::string& sFileName)
    {
        std::vector<CoreMaterialPtr> ret;
        CoreMaterial_Cal3dXHandler handler(ret, sFileName);

        // We haven't written a schema yet.
        m_parser->parseXMLFile(handler, sFileName, "");

        return ret;
    }

    std::vector<CoreMaterialPtr> Cal3DXLoader::loadMaterial(const void* pData, std::size_t size)
    {
        std::vector<CoreMaterialPtr> ret;
        CoreMaterial_Cal3dXHandler handler(ret, "unnamed");

        // We haven't written a schema yet.
        m_parser->parseXMLFileFromMem(handler, std::string(reinterpret_cast<const char*>(pData), size), "");

        return ret;
    }

    std::vector<CoreMaterialSetPtr> Cal3DXLoader::loadMaterialSet(const std::string& sFileName)
    {
        // TODO: How?
        std::vector<CoreMaterialSetPtr> ret;
        return ret;
    }

    std::vector<CoreMaterialSetPtr> Cal3DXLoader::loadMaterialSet(const void* pData, std::size_t size)
    {
        // TODO: How?
        std::vector<CoreMaterialSetPtr> ret;
        return ret;
    }

    std::vector<CoreAnimationPtr> Cal3DXLoader::loadAnimation(const std::string& sFileName)
    {
        return this->loadAnimation(sFileName, m_pLastSkel, sFileName);
    }

    std::vector<CoreAnimationPtr> Cal3DXLoader::loadAnimation(const void* pData, std::size_t size)
    {
        return this->loadAnimation(pData, size, m_pLastSkel, "unnamed");
    }

    std::vector<CoreAnimationPtr> Cal3DXLoader::loadAnimation(const std::string& sFileName, CoreSkeletonPtr skeleton, std::string animName)
    {
        std::vector<CoreAnimationPtr> ret;
        CoreAnimation_Cal3dXHandler handler(ret, skeleton, animName);

        // We haven't written a schema yet.
        m_parser->parseXMLFile(handler, sFileName, "");

        return ret;
    }

    std::vector<CoreAnimationPtr> Cal3DXLoader::loadAnimation(const void* pData, std::size_t size, CoreSkeletonPtr skeleton, std::string animName)
    {
        std::vector<CoreAnimationPtr> ret;
        CoreAnimation_Cal3dXHandler handler(ret, skeleton, animName);

        // We haven't written a schema yet.
        m_parser->parseXMLFileFromMem(handler, std::string(reinterpret_cast<const char*>(pData), size), "");

        return ret;
    }

} // namespace bouge
