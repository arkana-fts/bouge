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
#ifndef BOUGE_CORESKELETON_HPP
#define BOUGE_CORESKELETON_HPP

#include <bouge/bougefwd.hpp>

#include <map>
#include <vector>
#include <string>

namespace bouge {

    class BOUGE_API CoreSkeleton
    {
        typedef std::map<std::string, BoneId> BoneNamesMap;
        typedef std::vector<CoreBonePtr> BoneMap;
        typedef std::vector<BoneId> RootBoneNames;
    public:
        CoreSkeleton(std::string name);
        virtual ~CoreSkeleton();

        BOUGE_USER_DATA;

        std::string name() const;
        CoreSkeleton& name(std::string name);

        CoreSkeleton& addRootBone(CoreBonePtr bone);

        bool hasBone(const std::string& name) const;
        /// \exception NotExistException
        BoneId boneId(const std::string& name) const;
        /// \exception NotExistException
        CoreBonePtr bone(const std::string& name);
        /// \exception NotExistException
        CoreBonePtrC bone(const std::string& name) const;
        /// \exception NotExistException
        CoreBonePtr bone(const BoneId& id);
        /// \exception NotExistException
        CoreBonePtrC bone(const BoneId& id) const;

        // skeleton.rotateBone("Bla", 90);
        // skeleton.getBone("Bla").rotate(90);
        // skeleton["Bla"].rotate(90);

        BoneMap::size_type boneCount() const;
        RootBoneNames::size_type rootBoneCount() const;

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
            CoreBonePtr operator*();
            CoreBone* operator->();
        private:
            friend class CoreSkeleton;
            iterator(BoneMap::iterator me);
            BoneMap::iterator myIter;
        };

        iterator begin();
        iterator end();

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
            CoreBonePtrC operator*() const;
            const CoreBone* operator->() const;
        private:
            friend class CoreSkeleton;
            const_iterator(BoneMap::const_iterator me);
            BoneMap::const_iterator myIter;
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
            CoreBonePtr operator*();
            CoreBone* operator->();
        private:
            friend class CoreSkeleton;
            root_iterator(CoreSkeleton& me, RootBoneNames::iterator iter);
            CoreSkeleton& me;
            RootBoneNames::iterator myIter;
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
            CoreBonePtrC operator*() const;
            const CoreBone* operator->() const;
        private:
            friend class CoreSkeleton;
            const_root_iterator(const CoreSkeleton& me, RootBoneNames::const_iterator iter);
            const CoreSkeleton& me;
            RootBoneNames::const_iterator myIter;
        };

        const_root_iterator begin_root() const;
        const_root_iterator end_root() const;

    protected:
        BoneId addRecursively(CoreBonePtr bone);

    private:
        BoneNamesMap m_boneNames;
        RootBoneNames m_rootBoneNames;
        BoneMap m_allBones;

        std::string m_sName;
    };

} // namespace bouge

#endif // BOUGE_CORESKELETON_HPP
