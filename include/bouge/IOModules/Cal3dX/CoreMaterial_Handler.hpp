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
#ifndef BOUGE_COREMATERIAL_CAL3DXHANDLER_HPP
#define BOUGE_COREMATERIAL_CAL3DXHANDLER_HPP

#include <bouge/IOModules/XMLParserCommon/XMLHandler.hpp>
#include <bouge/bougefwd.hpp>

#include <string>
#include <vector>

namespace bouge {

    class CoreMaterial_Cal3dXHandler : public XMLHandler
    {
    public:
        CoreMaterial_Cal3dXHandler(std::vector<CoreMaterialPtr>& materialsToLoad, std::string name);
        virtual ~CoreMaterial_Cal3dXHandler();

        virtual void elementStart(const std::string& element, const XMLAttributes& attributes);
        virtual void elementEnd(const std::string& element);
        virtual void textElement(const std::string& element, const XMLAttributes& attributes, const std::string& text);
        virtual bool wantsToEnter(const std::string& element);

    protected:
        void materialStart(const XMLAttributes& attributes);
        void materialEnd();

    private:
        std::vector<CoreMaterialPtr>& m_materialsToLoad;
        std::string m_name;
        unsigned int m_iCurrMapId;
    };

} // namespace bouge

#endif // BOUGE_COREMATERIAL_CAL3DXHANDLER_HPP
