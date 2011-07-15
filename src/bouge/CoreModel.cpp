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
#include <bouge/CoreModel.hpp>
#include <bouge/CoreMesh.hpp>
#include <bouge/CoreSkeleton.hpp>
#include <bouge/CoreMaterial.hpp>
#include <bouge/CoreMaterialSet.hpp>
#include <bouge/CoreHardwareMesh.hpp>
#include <bouge/CoreAnimation.hpp>

#include <stdexcept>
#include <bouge/CoreTrack.hpp>

namespace bouge {

    CoreModel::CoreModel(std::string name)
        : m_sName(name)
    { }

    CoreModel::~CoreModel()
    { }

    std::string CoreModel::name() const
    {
        return m_sName;
    }

    CoreModel& CoreModel::name(std::string name)
    {
        m_sName = name;
        return *this;
    }

    CoreMeshPtr CoreModel::mesh()
    {
        return m_mesh;
    }

    CoreMeshPtrC CoreModel::mesh() const
    {
        return m_mesh;
    }

    CoreModel& CoreModel::mesh(CoreMeshPtr mesh)
    {
        m_mesh = mesh;
        return *this;
    }

    CoreSkeletonPtr CoreModel::skeleton()
    {
        return m_skel;
    }

    CoreSkeletonPtrC CoreModel::skeleton() const
    {
        return m_skel;
    }

    CoreModel& CoreModel::skeleton(CoreSkeletonPtr skeleton)
    {
        m_skel = skeleton;
        return *this;
    }

    bool CoreModel::hasMaterial(const std::string& name) const
    {
        return m_mats.find(name) != m_mats.end();
    }

    CoreMaterialPtr CoreModel::material(const std::string& name)
    {
        MaterialMap::iterator i = m_mats.find(name);

        if(i == m_mats.end())
            throw std::invalid_argument("There is no material " + name + " in the model " + m_sName);

        return i->second;
    }

    CoreMaterialPtrC CoreModel::material(const std::string& name) const
    {
        MaterialMap::const_iterator i = m_mats.find(name);

        if(i == m_mats.end())
            throw std::invalid_argument("There is no material " + name + " in the model " + m_sName);

        return i->second;
    }

    CoreModel& CoreModel::addMaterial(CoreMaterialPtr mat)
    {
        m_mats[mat->name()] = mat;
        return *this;
    }

    CoreModel& CoreModel::addMaterials(const std::vector<CoreMaterialPtr>& mats)
    {
        for(std::vector<CoreMaterialPtr>::const_iterator i = mats.begin() ; i != mats.end() ; ++i) {
            m_mats[(*i)->name()] = *i;
        }
        return *this;
    }

    CoreModel& CoreModel::removeMaterial(const std::string& name)
    {
        m_mats.erase(name);
        return *this;
    }

    std::size_t CoreModel::materialCount() const
    {
        return m_mats.size();
    }

    CoreMaterialPtr CoreModel::materialForSubmesh(const std::string& submesh, const std::string& matset)
    {
        return this->material(this->materialSet(matset)->materialForMesh(submesh));
    }

    CoreMaterialPtrC CoreModel::materialForSubmesh(const std::string& submesh, const std::string& matset) const
    {
        return this->material(this->materialSet(matset)->materialForMesh(submesh));
    }

    CoreModel::material_iterator::material_iterator(MaterialMap::iterator me)
        : myIter(me)
    { }

    CoreModel::material_iterator::material_iterator()
    { }

    CoreModel::material_iterator::~material_iterator()
    { }

    bool CoreModel::material_iterator::operator==(material_iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreModel::material_iterator::operator!=(material_iterator other) const
    {
        return myIter != other.myIter;
    }

    CoreModel::material_iterator& CoreModel::material_iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreModel::material_iterator CoreModel::material_iterator::operator++(int)
    {
        return material_iterator(myIter++);
    }

    CoreModel::material_iterator& CoreModel::material_iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreModel::material_iterator CoreModel::material_iterator::operator--(int)
    {
        return material_iterator(myIter--);
    }

    CoreMaterialPtr CoreModel::material_iterator::operator*()
    {
        return myIter->second;
    }

    CoreMaterial* CoreModel::material_iterator::operator->()
    {
        return myIter->second.operator->();
    }

    CoreModel::material_iterator CoreModel::begin_material()
    {
        return material_iterator(m_mats.begin());
    }

    CoreModel::material_iterator CoreModel::end_material()
    {
        return material_iterator(m_mats.end());
    }

    CoreModel::const_material_iterator::const_material_iterator(MaterialMap::const_iterator me)
        : myIter(me)
    { }

    CoreModel::const_material_iterator::const_material_iterator()
    { }

    CoreModel::const_material_iterator::~const_material_iterator()
    { }

    bool CoreModel::const_material_iterator::operator==(const_material_iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreModel::const_material_iterator::operator!=(const_material_iterator other) const
    {
        return myIter != other.myIter;
    }

    CoreModel::const_material_iterator& CoreModel::const_material_iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreModel::const_material_iterator CoreModel::const_material_iterator::operator++(int)
    {
        return const_material_iterator(myIter++);
    }

    CoreModel::const_material_iterator& CoreModel::const_material_iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreModel::const_material_iterator CoreModel::const_material_iterator::operator--(int)
    {
        return const_material_iterator(myIter--);
    }

    CoreMaterialPtrC CoreModel::const_material_iterator::operator*() const
    {
        return myIter->second;
    }

    const CoreMaterial* CoreModel::const_material_iterator::operator->() const
    {
        return myIter->second.operator->();
    }

    CoreModel::const_material_iterator CoreModel::begin_material() const
    {
        return const_material_iterator(m_mats.begin());
    }

    CoreModel::const_material_iterator CoreModel::end_material() const
    {
        return const_material_iterator(m_mats.end());
    }

    bool CoreModel::hasMaterialSet(const std::string& name) const
    {
        return m_matsets.find(name) != m_matsets.end();
    }

    CoreMaterialSetPtr CoreModel::materialSet(const std::string& name)
    {
        MaterialSetMap::iterator i = m_matsets.find(name);

        if(i == m_matsets.end())
            throw std::invalid_argument("There is no material-set " + name + " in the model " + m_sName);

        return i->second;
    }

    CoreMaterialSetPtrC CoreModel::materialSet(const std::string& name) const
    {
        MaterialSetMap::const_iterator i = m_matsets.find(name);

        if(i == m_matsets.end())
            throw std::invalid_argument("There is no material-set " + name + " in the model " + m_sName);

        return i->second;
    }

    CoreModel& CoreModel::addMaterialSet(CoreMaterialSetPtr matset)
    {
        m_matsets[matset->name()] = matset;
        return *this;
    }

    CoreModel& CoreModel::addMaterialSets(const std::vector<CoreMaterialSetPtr>& matsets)
    {
        for(std::vector<CoreMaterialSetPtr>::const_iterator i = matsets.begin() ; i != matsets.end() ; ++i) {
            this->addMaterialSet(*i);
        }
        return *this;
    }

    CoreModel& CoreModel::removeMaterialSet(const std::string& name)
    {
        m_matsets.erase(name);
        return *this;
    }

    std::size_t CoreModel::materialSetCount() const
    {
        return m_matsets.size();
    }

    CoreModel::materialset_iterator::materialset_iterator(MaterialSetMap::iterator me)
        : myIter(me)
    { }

    CoreModel::materialset_iterator::materialset_iterator()
    { }

    CoreModel::materialset_iterator::~materialset_iterator()
    { }

    bool CoreModel::materialset_iterator::operator==(materialset_iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreModel::materialset_iterator::operator!=(materialset_iterator other) const
    {
        return myIter != other.myIter;
    }

    CoreModel::materialset_iterator& CoreModel::materialset_iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreModel::materialset_iterator CoreModel::materialset_iterator::operator++(int)
    {
        return materialset_iterator(myIter++);
    }

    CoreModel::materialset_iterator& CoreModel::materialset_iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreModel::materialset_iterator CoreModel::materialset_iterator::operator--(int)
    {
        return materialset_iterator(myIter--);
    }

    CoreMaterialSetPtr CoreModel::materialset_iterator::operator*()
    {
        return myIter->second;
    }

    CoreMaterialSet* CoreModel::materialset_iterator::operator->()
    {
        return myIter->second.operator->();
    }

    CoreModel::materialset_iterator CoreModel::begin_materialset()
    {
        return materialset_iterator(m_matsets.begin());
    }

    CoreModel::materialset_iterator CoreModel::end_materialset()
    {
        return materialset_iterator(m_matsets.end());
    }

    CoreModel::const_materialset_iterator::const_materialset_iterator(MaterialSetMap::const_iterator me)
         : myIter(me)
    { }

    CoreModel::const_materialset_iterator::const_materialset_iterator()
    { }

    CoreModel::const_materialset_iterator::~const_materialset_iterator()
    { }

    bool CoreModel::const_materialset_iterator::operator==(const_materialset_iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreModel::const_materialset_iterator::operator!=(const_materialset_iterator other) const
    {
        return myIter != other.myIter;
    }

    CoreModel::const_materialset_iterator& CoreModel::const_materialset_iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreModel::const_materialset_iterator CoreModel::const_materialset_iterator::operator++(int)
    {
        return const_materialset_iterator(myIter++);
    }

    CoreModel::const_materialset_iterator& CoreModel::const_materialset_iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreModel::const_materialset_iterator CoreModel::const_materialset_iterator::operator--(int)
    {
        return const_materialset_iterator(myIter--);
    }

    CoreMaterialSetPtrC CoreModel::const_materialset_iterator::operator*() const
    {
        return myIter->second;
    }

    const CoreMaterialSet* CoreModel::const_materialset_iterator::operator->() const
    {
        return myIter->second.operator->();
    }

    CoreModel::const_materialset_iterator CoreModel::begin_materialset() const
    {
        return const_materialset_iterator(m_matsets.begin());
    }

    CoreModel::const_materialset_iterator CoreModel::end_materialset() const
    {
        return const_materialset_iterator(m_matsets.end());
    }

    bool CoreModel::hasAnimation(const std::string& name) const
    {
        return m_anims.find(name) != m_anims.end();
    }

    CoreAnimationPtr CoreModel::animation(const std::string& name)
    {
        AnimationMap::iterator i = m_anims.find(name);

        if(i == m_anims.end())
            throw std::invalid_argument("There is no animation " + name + " in the model " + m_sName);

        return i->second;
    }

    CoreAnimationPtrC CoreModel::animation(const std::string& name) const
    {
        AnimationMap::const_iterator i = m_anims.find(name);

        if(i == m_anims.end())
            throw std::invalid_argument("There is no animation " + name + " in the model " + m_sName);

        return i->second;
    }

    CoreModel& CoreModel::addAnimation(CoreAnimationPtr anim)
    {
        m_anims[anim->name()] = anim;
        return *this;
    }

    CoreModel& CoreModel::addAnimations(const std::vector<bouge::CoreAnimationPtr>& mats)
    {
        for(std::vector<CoreAnimationPtr>::const_iterator i = mats.begin() ; i != mats.end() ; ++i) {
            m_anims[(*i)->name()] = *i;
        }
        return *this;
    }

    CoreModel& CoreModel::removeAnimation(const std::string& name)
    {
        m_anims.erase(name);
        return *this;
    }

    std::size_t CoreModel::animationCount() const
    {
        return m_anims.size();
    }

    CoreModel::animation_iterator::animation_iterator(CoreModel::AnimationMap::iterator me)
        : myIter(me)
    { }

    CoreModel::animation_iterator::animation_iterator()
    { }

    CoreModel::animation_iterator::~animation_iterator()
    { }

    bool CoreModel::animation_iterator::operator==(CoreModel::animation_iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreModel::animation_iterator::operator!=(CoreModel::animation_iterator other) const
    {
        return myIter != other.myIter;
    }

    CoreModel::animation_iterator& CoreModel::animation_iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreModel::animation_iterator CoreModel::animation_iterator::operator++(int)
    {
        return animation_iterator(myIter++);
    }

    CoreModel::animation_iterator& CoreModel::animation_iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreModel::animation_iterator CoreModel::animation_iterator::operator--(int)
    {
        return animation_iterator(myIter--);
    }

    CoreAnimationPtr CoreModel::animation_iterator::operator*()
    {
        return myIter->second;
    }

    CoreAnimationPtrC CoreModel::animation_iterator::operator*() const
    {
        return myIter->second;
    }

    CoreAnimation* CoreModel::animation_iterator::operator->()
    {
        return myIter->second.operator->();
    }

    const CoreAnimation* CoreModel::animation_iterator::operator->() const
    {
        return myIter->second.operator->();
    }

    CoreModel::animation_iterator CoreModel::begin_animation()
    {
        return animation_iterator(m_anims.begin());
    }

    CoreModel::animation_iterator CoreModel::end_animation()
    {
        return animation_iterator(m_anims.end());
    }

    CoreModel::const_animation_iterator::const_animation_iterator(CoreModel::AnimationMap::const_iterator me)
        : myIter(me)
    { }

    CoreModel::const_animation_iterator::const_animation_iterator()
    { }

    CoreModel::const_animation_iterator::~const_animation_iterator()
    { }

    bool CoreModel::const_animation_iterator::operator==(CoreModel::const_animation_iterator other) const
    {
        return myIter == other.myIter;
    }

    bool CoreModel::const_animation_iterator::operator!=(CoreModel::const_animation_iterator other) const
    {
        return myIter != other.myIter;
    }

    CoreModel::const_animation_iterator& CoreModel::const_animation_iterator::operator++()
    {
        return ++myIter, *this;
    }

    CoreModel::const_animation_iterator CoreModel::const_animation_iterator::operator++(int)
    {
        return const_animation_iterator(myIter++);
    }

    CoreModel::const_animation_iterator& CoreModel::const_animation_iterator::operator--()
    {
        return --myIter, *this;
    }

    CoreModel::const_animation_iterator CoreModel::const_animation_iterator::operator--(int)
    {
        return const_animation_iterator(myIter--);
    }

    CoreAnimationPtrC CoreModel::const_animation_iterator::operator*() const
    {
        return myIter->second;
    }

    const CoreAnimation* CoreModel::const_animation_iterator::operator->() const
    {
        return myIter->second.operator->();
    }

    CoreModel::const_animation_iterator CoreModel::begin_animation() const
    {
        return const_animation_iterator(m_anims.begin());
    }

    CoreModel::const_animation_iterator CoreModel::end_animation() const
    {
        return const_animation_iterator(m_anims.end());
    }

    std::set<std::string> CoreModel::missingBones() const
    {
        std::set<std::string> ret;

        // Check for all bones appearing in the vertex influences.
        for(CoreMesh::const_iterator iSubMesh = this->mesh()->begin() ; iSubMesh != this->mesh()->end() ; ++iSubMesh) {
            for(std::size_t iVtx = 0 ; iVtx < iSubMesh->vertexCount() ; ++iVtx) {
                Vertex vtx = iSubMesh->vertex(iVtx);
                for(std::size_t iInfluence = 0 ; iInfluence < vtx.influenceCount() ; ++iInfluence) {
                    Influence influence = vtx.influence(iInfluence);
                    if(!m_skel || !m_skel->hasBone(influence.sBoneName)) {
                        ret.insert(influence.sBoneName);
                    }
                }
            }
        }

        // Check for all bones appearing in the animations.
        for(const_animation_iterator iAnim = this->begin_animation() ; iAnim != this->end_animation() ; ++iAnim) {
            for(CoreAnimation::const_iterator iTrack = iAnim->begin() ; iTrack != iAnim->end() ; ++iTrack) {
                if(!m_skel || !m_skel->hasBone(iTrack.bone())) {
                    ret.insert(iTrack.bone());
                }
            }
        }

        return ret;
    }

    std::set<std::string> CoreModel::missingMaterials(const std::string& in_restrictToMatset) const
    {
        std::set<std::string> ret;

        for(const_materialset_iterator iMatSet = this->begin_materialset() ; iMatSet != this->end_materialset() ; ++iMatSet) {
            // If restrictToMatset is specified, skip all matsets with a different name.
            if(!in_restrictToMatset.empty() && iMatSet->name() != in_restrictToMatset)
                continue;

            for(CoreMaterialSet::const_iterator iAssoss = iMatSet->begin() ; iAssoss != iMatSet->end() ; ++iAssoss) {
                // Only count for actually existing submeshes.
                if(!this->hasMaterial(iAssoss.matname()) && !this->mesh()->hasSubmesh(iAssoss.meshname())) {
                    ret.insert(iAssoss.matname());
                }
            }
        }

        return ret;
    }

    std::set<std::string> CoreModel::missingMatsetSpecs(const std::string& in_restrictToMatset) const
    {
        std::set<std::string> ret;

        for(CoreMesh::iterator iSubMesh = m_mesh->begin() ; iSubMesh != m_mesh->end() ; ++iSubMesh) {
            for(const_materialset_iterator iMatSet = this->begin_materialset() ; iMatSet != this->end_materialset() ; ++iMatSet) {
                // If restrictToMatset is specified, skip all matsets with a different name.
                if(!in_restrictToMatset.empty() && iMatSet->name() != in_restrictToMatset)
                    continue;

                try {
                    iMatSet->materialForMesh(iSubMesh->name());
                } catch(const std::exception&) {
                    // No entry for this submesh present? That's bad.
                    ret.insert(iSubMesh->name());
                }
            }
        }

        return ret;
    }

    bool CoreModel::isComplete() const
    {
        return !missingBones().empty() && !missingMaterials().empty() && !missingMatsetSpecs().empty();
    }

    CoreHardwareMeshPtr CoreModel::buildHardwareMesh(unsigned int bonesPerMesh, unsigned char bonesPerVertex, unsigned char verticesPerFace) const
    {
        return CoreHardwareMeshPtr(new CoreHardwareMesh(this->mesh(), bonesPerMesh, bonesPerVertex, verticesPerFace));
    }
}
