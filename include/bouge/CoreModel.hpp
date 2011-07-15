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
#ifndef BOUGE_COREMODEL_HPP
#define BOUGE_COREMODEL_HPP

#include <bouge/bougefwd.hpp>

#include <map>
#include <set>
#include <vector>
#include <string>
#include <iterator>

namespace bouge {

    class BOUGE_API CoreModel
    {
        typedef std::map<std::string, CoreMaterialPtr> MaterialMap;
        typedef std::map<std::string, CoreMaterialSetPtr> MaterialSetMap;
        typedef std::map<std::string, CoreAnimationPtr> AnimationMap;
    public:
        CoreModel(std::string name);
        virtual ~CoreModel();

        BOUGE_USER_DATA;

        std::string name() const;
        CoreModel& name(std::string name);

        CoreMeshPtr mesh();
        CoreMeshPtrC mesh() const;
        CoreModel& mesh(CoreMeshPtr mesh);

        CoreSkeletonPtr skeleton();
        CoreSkeletonPtrC skeleton() const;
        CoreModel& skeleton(CoreSkeletonPtr skeleton);

        bool hasMaterial(const std::string& name) const;
        /// \exception std::invalid_argument
        CoreMaterialPtr material(const std::string& name);
        /// \exception std::invalid_argument
        CoreMaterialPtrC material(const std::string& name) const;
        CoreModel& addMaterial(CoreMaterialPtr mat);
        CoreModel& addMaterials(const std::vector<CoreMaterialPtr>& mats);
        CoreModel& removeMaterial(const std::string& name);
        std::size_t materialCount() const;

        CoreMaterialPtr materialForSubmesh(const std::string& submesh, const std::string& matset);
        CoreMaterialPtrC materialForSubmesh(const std::string& submesh, const std::string& matset) const;

        class BOUGE_API material_iterator {
        public:
            material_iterator();
            ~material_iterator();

            bool operator==(material_iterator other) const;
            bool operator!=(material_iterator other) const;
            material_iterator& operator++();
            material_iterator operator++(int);
            material_iterator& operator--();
            material_iterator operator--(int);
            CoreMaterialPtr operator*();
            CoreMaterial* operator->();
        private:
            friend class CoreModel;
            material_iterator(MaterialMap::iterator me);
            MaterialMap::iterator myIter;
        };

        material_iterator begin_material();
        material_iterator end_material();

        class BOUGE_API const_material_iterator {
        public:
            const_material_iterator();
            ~const_material_iterator();

            bool operator==(const_material_iterator other) const;
            bool operator!=(const_material_iterator other) const;
            const_material_iterator& operator++();
            const_material_iterator operator++(int);
            const_material_iterator& operator--();
            const_material_iterator operator--(int);
            CoreMaterialPtrC operator*() const;
            const CoreMaterial* operator->() const;
        private:
            friend class CoreModel;
            const_material_iterator(MaterialMap::const_iterator me);
            MaterialMap::const_iterator myIter;
        };

        const_material_iterator begin_material() const;
        const_material_iterator end_material() const;

        bool hasMaterialSet(const std::string& name) const;
        CoreMaterialSetPtr materialSet(const std::string& name);
        CoreMaterialSetPtrC materialSet(const std::string& name) const;
        CoreModel& addMaterialSet(CoreMaterialSetPtr mat);
        CoreModel& addMaterialSets(const std::vector<CoreMaterialSetPtr>& mats);
        CoreModel& removeMaterialSet(const std::string& name);
        std::size_t materialSetCount() const;

        class BOUGE_API materialset_iterator {
        public:
            materialset_iterator();
            ~materialset_iterator();

            bool operator==(materialset_iterator other) const;
            bool operator!=(materialset_iterator other) const;
            materialset_iterator& operator++();
            materialset_iterator operator++(int);
            materialset_iterator& operator--();
            materialset_iterator operator--(int);
            CoreMaterialSetPtr operator*();
            CoreMaterialSet* operator->();
        private:
            friend class CoreModel;
            materialset_iterator(MaterialSetMap::iterator me);
            MaterialSetMap::iterator myIter;
        };

        materialset_iterator begin_materialset();
        materialset_iterator end_materialset();

        class BOUGE_API const_materialset_iterator {
        public:
            const_materialset_iterator();
            ~const_materialset_iterator();

            bool operator==(const_materialset_iterator other) const;
            bool operator!=(const_materialset_iterator other) const;
            const_materialset_iterator& operator++();
            const_materialset_iterator operator++(int);
            const_materialset_iterator& operator--();
            const_materialset_iterator operator--(int);
            CoreMaterialSetPtrC operator*() const;
            const CoreMaterialSet* operator->() const;
        private:
            friend class CoreModel;
            const_materialset_iterator(MaterialSetMap::const_iterator me);
            MaterialSetMap::const_iterator myIter;
        };

        const_materialset_iterator begin_materialset() const;
        const_materialset_iterator end_materialset() const;

        bool hasAnimation(const std::string& name) const;
        CoreAnimationPtr animation(const std::string& name);
        CoreAnimationPtrC animation(const std::string& name) const;
        CoreModel& addAnimation(CoreAnimationPtr mat);
        CoreModel& addAnimations(const std::vector<CoreAnimationPtr>& mats);
        CoreModel& removeAnimation(const std::string& name);
        std::size_t animationCount() const;

        class BOUGE_API animation_iterator {
        public:
            animation_iterator();
            ~animation_iterator();

            bool operator==(animation_iterator other) const;
            bool operator!=(animation_iterator other) const;
            animation_iterator& operator++();
            animation_iterator operator++(int);
            animation_iterator& operator--();
            animation_iterator operator--(int);
            CoreAnimationPtr operator*();
            CoreAnimationPtrC operator*() const;
            CoreAnimation* operator->();
            const CoreAnimation* operator->() const;
        private:
            friend class CoreModel;
            animation_iterator(AnimationMap::iterator me);
            AnimationMap::iterator myIter;
        };

        animation_iterator begin_animation();
        animation_iterator end_animation();

        class BOUGE_API const_animation_iterator {
        public:
            const_animation_iterator();
            ~const_animation_iterator();

            bool operator==(const_animation_iterator other) const;
            bool operator!=(const_animation_iterator other) const;
            const_animation_iterator& operator++();
            const_animation_iterator operator++(int);
            const_animation_iterator& operator--();
            const_animation_iterator operator--(int);
            CoreAnimationPtrC operator*() const;
            const CoreAnimation* operator->() const;
        private:
            friend class CoreModel;
            const_animation_iterator(AnimationMap::const_iterator me);
            AnimationMap::const_iterator myIter;
        };

        const_animation_iterator begin_animation() const;
        const_animation_iterator end_animation() const;

        std::set<std::string> missingBones() const;
        std::set<std::string> missingMaterials(const std::string& in_restrictToMatset = "") const;
        std::set<std::string> missingMatsetSpecs(const std::string& in_restrictToMatset = "") const;
        bool isComplete() const;
        CoreHardwareMeshPtr buildHardwareMesh(unsigned int bonesPerMesh, unsigned char bonesPerVertex = 4, unsigned char verticesPerFace = 3) const;

    private:
        std::string m_sName;
        CoreMeshPtr m_mesh;
        CoreSkeletonPtr m_skel;
        MaterialMap m_mats;
        MaterialSetMap m_matsets;
        AnimationMap m_anims;
    };

} // namespace bouge

#endif // BOUGE_COREMODEL_HPP
