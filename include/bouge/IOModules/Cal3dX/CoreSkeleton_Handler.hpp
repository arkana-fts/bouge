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
#ifndef BOUGE_CORESKELETON_CAL3DXHANDLER_HPP
#define BOUGE_CORESKELETON_CAL3DXHANDLER_HPP

#include <bouge/IOModules/XMLParserCommon/XMLHandler.hpp>
#include <bouge/bougefwd.hpp>
#include <bouge/Math/Quaternion.hpp>
#include <bouge/Math/Vector.hpp>

#include <map>

namespace bouge {

    class CoreSkeleton_Cal3dXHandler : public XMLHandler
    {
    public:
        CoreSkeleton_Cal3dXHandler(CoreSkeleton& skeletonToBuild);
        virtual ~CoreSkeleton_Cal3dXHandler();

        virtual void elementStart(const std::string& element, const bouge::XMLAttributes& attributes);
        virtual void elementEnd(const std::string& element);
        virtual void textElement(const std::string& element, const bouge::XMLAttributes& attributes, const std::string& text);
        virtual bool wantsToEnter(const std::string& element);

    protected:
        void skeletonStart(const XMLAttributes& attributes);
        void skeletonEnd();

        void boneStart(const XMLAttributes& attributes);
        void boneEnd();

    private:
        CoreSkeleton& m_skeletonToBuild;

        std::map<int, std::pair<CoreBonePtr, int> > m_bonesAndParents;

        int m_nUnnamedBoneCount;

        std::string m_sCurrBoneName;
        int m_iCurrBoneId;
        int m_iCurrBoneParentId;
        Vector m_vCurrBoneRelativePosition;
        Quaternion m_qCurrBoneRelativeRotation;
    };

    struct Cal3dXBoneIdUD : public UserData
    {
        int id;
        Cal3dXBoneIdUD(int id) : id(id) {};
        virtual ~Cal3dXBoneIdUD() {};
    };

} // namespace bouge

#endif // BOUGE_CORESKELETON_CAL3DXHANDLER_HPP
