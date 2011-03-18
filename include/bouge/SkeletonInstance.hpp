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
#ifndef BOUGE_SKELETONINSTANCE_HPP
#define BOUGE_SKELETONINSTANCE_HPP

#include <bouge/bougefwd.hpp>
#include <bouge/CoreSkeleton.hpp>

#include <vector>
#include <map>
#include <string>

namespace bouge {

    class BOUGE_API SkeletonInstance : public enable_shared_from_this<SkeletonInstance>::type
    {
        typedef std::vector<BoneInstancePtr> BoneMap;
    public:
        SkeletonInstance(CoreSkeletonPtrC core);
        virtual ~SkeletonInstance();

        BOUGE_USER_DATA;

        std::string name() const;

        CoreSkeletonPtrC core() const;

        bool hasBone(const std::string& name) const;
        /// \exception NotExistException
        BoneInstancePtr bone(const std::string& name);
        /// \exception NotExistException
        BoneInstancePtrC bone(const std::string& name) const;
        /// \exception NotExistException
        BoneInstancePtr bone(const BoneId& id);
        /// \exception NotExistException
        BoneInstancePtrC bone(const BoneId& id) const;

        SkeletonInstance& recalcAllBones();
        SkeletonInstance& resetAllBones();

        BoneMap::size_type boneCount() const;
        std::size_t rootBoneCount() const;

        class BOUGE_API iterator {
        public:
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
            friend class SkeletonInstance;
            iterator(SkeletonInstance* me, CoreSkeleton::const_iterator iter);
            SkeletonInstance* me;
            CoreSkeleton::const_iterator myIter;
        };

        iterator begin();
        iterator end();

        class BOUGE_API const_iterator {
        public:
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
            friend class SkeletonInstance;
            const_iterator(const SkeletonInstance* me, CoreSkeleton::const_iterator iter);
            const SkeletonInstance* me;
            CoreSkeleton::const_iterator myIter;
        };

        const_iterator begin() const;
        const_iterator end() const;

        class BOUGE_API root_iterator {
        public:
            ~root_iterator();

            bool operator==(root_iterator other) const;
            bool operator!=(root_iterator other) const;
            root_iterator& operator++();
            root_iterator operator++(int);
            root_iterator& operator--();
            root_iterator operator--(int);
            BoneInstancePtr operator*();
            BoneInstance* operator->();
        private:
            friend class SkeletonInstance;
            root_iterator(SkeletonInstance* me, CoreSkeleton::const_root_iterator iter);
            SkeletonInstance* me;
            CoreSkeleton::const_root_iterator myIter;
        };

        root_iterator begin_root();
        root_iterator end_root();

        class BOUGE_API const_root_iterator {
        public:
            ~const_root_iterator();

            bool operator==(const_root_iterator other) const;
            bool operator!=(const_root_iterator other) const;
            const_root_iterator& operator++();
            const_root_iterator operator++(int);
            const_root_iterator& operator--();
            const_root_iterator operator--(int);
            BoneInstancePtrC operator*() const;
            const BoneInstance* operator->() const;
        private:
            friend class SkeletonInstance;
            const_root_iterator(const SkeletonInstance* me, CoreSkeleton::const_root_iterator iter);
            const SkeletonInstance* me;
            CoreSkeleton::const_root_iterator myIter;
        };

        const_root_iterator begin_root() const;
        const_root_iterator end_root() const;

    private:
        CoreSkeletonPtrC m_core;

        BoneMap m_bones;
    };

} // namespace bouge

#endif // BOUGE_SKELETONINSTANCE_HPP
