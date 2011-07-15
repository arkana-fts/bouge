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
#ifndef BOUGE_BONEINSTANCE_HPP
#define BOUGE_BONEINSTANCE_HPP

#include <bouge/bougefwd.hpp>

#include <bouge/CoreBone.hpp>
#include <bouge/Math/Vector.hpp>
#include <bouge/Math/Quaternion.hpp>

namespace bouge {

    class BOUGE_API BoneInstance
    {
    public:
        BoneInstance(bouge::CoreBonePtrC core);
        virtual ~BoneInstance();

        BOUGE_USER_DATA;

        std::string name() const;

        CoreBonePtrC core() const;

        bool hasParent() const;
        /// \exception NoParentException
        BoneInstancePtr parent(SkeletonInstancePtr skel);
        /// \exception NoParentException
        BoneInstancePtrC parent(SkeletonInstancePtrC skel) const;

        std::size_t childCount() const;
        bool hasChild(const std::string& name, SkeletonInstancePtrC skel) const;
        /// \exception NotExistException
        BoneInstancePtr child(const std::string& name, SkeletonInstancePtr skel);
        /// \exception NotExistException
        BoneInstancePtrC child(const std::string& name, SkeletonInstancePtrC skel) const;

        class BOUGE_API iterator {
        public:
            iterator();
            ~iterator();

            bool operator==(iterator other) const;
            bool operator!=(iterator other) const;
            iterator& operator++();
            iterator operator++(int);
            iterator& operator--();
            iterator operator--(int);
            BoneInstancePtr operator*();
            BoneInstance* operator->();
        private:
            friend class BoneInstance;
            iterator(CoreBone::const_iterator me, SkeletonInstancePtr skel);
            CoreBone::const_iterator myIter;
            SkeletonInstancePtr mySkel;
        };

        iterator begin(SkeletonInstancePtr skel);
        iterator end(SkeletonInstancePtr skel);

        class BOUGE_API const_iterator {
        public:
            const_iterator();
            ~const_iterator();

            bool operator==(const_iterator other) const;
            bool operator!=(const_iterator other) const;
            const_iterator& operator++();
            const_iterator operator++(int);
            const_iterator& operator--();
            const_iterator operator--(int);
            BoneInstancePtrC operator*() const;
            const BoneInstance* operator->() const;
        private:
            friend class BoneInstance;
            const_iterator(CoreBone::const_iterator me, SkeletonInstancePtrC skel);
            CoreBone::const_iterator myIter;
            SkeletonInstancePtrC mySkel;
        };

        const_iterator begin(SkeletonInstancePtrC skel) const;
        const_iterator end(SkeletonInstancePtrC skel) const;

        BoneInstance& recalcRecursive(SkeletonInstancePtr skel, bool bParentWasDirty = false);
        BoneInstance& recalc(BoneInstancePtrC parent);
        BoneInstance& recalc();

        void resetRecursive(SkeletonInstancePtr skel);
        void reset();

        Vector scale() const;
        BoneInstance& scale(Vector v);

        Vector trans() const;
        BoneInstance& trans(Vector v);

        Quaternion rot() const;
        BoneInstance& rot(Quaternion q);

        float length() const;

        AffineMatrix transformMatrix() const;

    protected:
        BoneInstance& recalcMatrixCache();

    public:
        CoreBonePtrC m_core;

        Vector m_scale;
        Vector m_transl;
        Quaternion m_rotation;

        bool m_bDirty;

        // Helper cache variables.
        Vector m_absoluteRootPos;
        Quaternion m_absoluteBoneRot;
        Vector m_absoluteBoneScale;
        AffineMatrix m_transformationMatrix;
    };

} // namespace bouge

#endif // BOUGE_BONEINSTANCE_HPP
