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
#include <bouge/IOModules/XML/Loader.hpp>
#include <bouge/IOModules/XML/CoreMesh_Handler.hpp>
#include <bouge/IOModules/XML/CoreSkeleton_Handler.hpp>
#include <bouge/IOModules/XML/CoreMaterial_Handler.hpp>
#include <bouge/IOModules/XML/CoreMaterialSet_Handler.hpp>
#include <bouge/IOModules/XML/CoreAnimation_Handler.hpp>

#include <bouge/CoreMesh.hpp>
#include <bouge/CoreSkeleton.hpp>

#include <sstream>
#include <fstream>

namespace bouge
{

    XMLLoader::XMLLoader(XMLParser* parser)
        : m_parser(parser)
    { }

    XMLLoader::~XMLLoader()
    {
        delete m_parser;
    }

    CoreMeshPtr XMLLoader::loadMesh(const std::string& sFileName)
    {

        CoreMeshPtr ret(new CoreMesh(sFileName));
        CoreMesh_XMLHandler handler(*ret);

        // We haven't written a schema yet.
        m_parser->parseXMLFile(handler, sFileName, "");

        return ret;
    }

    CoreMeshPtr XMLLoader::loadMesh(const void* pData, std::size_t size)
    {
        CoreMeshPtr ret(new CoreMesh(""));
        CoreMesh_XMLHandler handler(*ret);

        // We haven't written a schema yet.
        if(size == (std::size_t)-1) {
            m_parser->parseXMLFileFromMem(handler, std::string(reinterpret_cast<const char*>(pData)), "");
        } else {
            m_parser->parseXMLFileFromMem(handler, std::string(reinterpret_cast<const char*>(pData), size), "");
        }

        return ret;
    }

    CoreSkeletonPtr XMLLoader::loadSkeleton(const std::string& sFileName)
    {
        CoreSkeletonPtr ret(new CoreSkeleton(sFileName));
        CoreSkeleton_XMLHandler handler(*ret);

        // We haven't written a schema yet.
        m_parser->parseXMLFile(handler, sFileName, "");

        return ret;
    }

    CoreSkeletonPtr XMLLoader::loadSkeleton(const void* pData, std::size_t size)
    {
        CoreSkeletonPtr ret(new CoreSkeleton(""));
        CoreSkeleton_XMLHandler handler(*ret);

        // We haven't written a schema yet.
        if(size == (std::size_t)-1) {
            m_parser->parseXMLFileFromMem(handler, std::string(reinterpret_cast<const char*>(pData)), "");
        } else {
            m_parser->parseXMLFileFromMem(handler, std::string(reinterpret_cast<const char*>(pData), size), "");
        }

        return ret;
    }

    std::vector<CoreMaterialPtr> XMLLoader::loadMaterial(const std::string& sFileName)
    {
        std::vector<CoreMaterialPtr> ret;
        CoreMaterial_XMLHandler handler(ret);

        // We haven't written a schema yet.
        m_parser->parseXMLFile(handler, sFileName, "");

        return ret;
    }

    std::vector<CoreMaterialPtr> XMLLoader::loadMaterial(const void* pData, std::size_t size)
    {
        std::vector<CoreMaterialPtr> ret;
        CoreMaterial_XMLHandler handler(ret);

        // We haven't written a schema yet.
        if(size == (std::size_t)-1) {
            m_parser->parseXMLFileFromMem(handler, std::string(reinterpret_cast<const char*>(pData)), "");
        } else {
            m_parser->parseXMLFileFromMem(handler, std::string(reinterpret_cast<const char*>(pData), size), "");
        }

        return ret;
    }

    std::vector<CoreMaterialSetPtr> XMLLoader::loadMaterialSet(const std::string& sFileName)
    {
        std::vector<CoreMaterialSetPtr> ret;
        CoreMaterialSet_XMLHandler handler(ret);

        // We haven't written a schema yet.
        m_parser->parseXMLFile(handler, sFileName, "");

        return ret;
    }

    std::vector<CoreMaterialSetPtr> XMLLoader::loadMaterialSet(const void* pData, std::size_t size)
    {
        std::vector<CoreMaterialSetPtr> ret;
        CoreMaterialSet_XMLHandler handler(ret);

        // We haven't written a schema yet.
        if(size == (std::size_t)-1) {
            m_parser->parseXMLFileFromMem(handler, std::string(reinterpret_cast<const char*>(pData)), "");
        } else {
            m_parser->parseXMLFileFromMem(handler, std::string(reinterpret_cast<const char*>(pData), size), "");
        }

        return ret;
    }

    std::vector<CoreAnimationPtr> XMLLoader::loadAnimation(const std::string& sFileName)
    {
        std::vector<CoreAnimationPtr> ret;
        CoreAnimation_XMLHandler handler(ret);

        // We haven't written a schema yet.
        m_parser->parseXMLFile(handler, sFileName, "");

        return ret;
    }

    std::vector<CoreAnimationPtr> XMLLoader::loadAnimation(const void* pData, std::size_t size)
    {
        std::vector<CoreAnimationPtr> ret;
        CoreAnimation_XMLHandler handler(ret);

        // We haven't written a schema yet.
        if(size == (std::size_t)-1) {
            m_parser->parseXMLFileFromMem(handler, std::string(reinterpret_cast<const char*>(pData)), "");
        } else {
            m_parser->parseXMLFileFromMem(handler, std::string(reinterpret_cast<const char*>(pData), size), "");
        }

        return ret;
    }

} // namespace bouge
