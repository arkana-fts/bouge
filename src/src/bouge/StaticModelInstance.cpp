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
#include <bouge/StaticModelInstance.hpp>
#include <bouge/CoreModel.hpp>
#include <bouge/CoreMaterialSet.hpp>

namespace bouge {

    StaticModelInstance::StaticModelInstance(CoreModelPtr coremdl)
        : m_coremdl(coremdl)
    {
        // Select the first material set if there are some.
        if(m_coremdl->begin_materialset() != m_coremdl->end_materialset()) {
            this->selectMatSet(m_coremdl->begin_materialset()->name());
        }
    }

    StaticModelInstance::~StaticModelInstance()
    { }

    std::string StaticModelInstance::currMatSet() const
    {
        return m_sMatSet;
    }

    StaticModelInstance& StaticModelInstance::selectMatSet(std::string matset)
    {
        m_sMatSet = matset;
        return *this;
    }

    CoreMaterialPtr StaticModelInstance::materialForSubmesh(const std::string& submesh)
    {
        return m_coremdl->materialForSubmesh(submesh, m_sMatSet);
    }

    CoreMaterialPtrC StaticModelInstance::materialForSubmesh(const std::string& submesh) const
    {
        return m_coremdl->materialForSubmesh(submesh, m_sMatSet);
    }

}
