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
#ifndef BOUGE_COREBONE_HPP
#define BOUGE_COREBONE_HPP

#include <bouge/bougefwd.hpp>
#include <bouge/Math/Vector.hpp>
#include <bouge/Math/Quaternion.hpp>
#include <bouge/Math/Matrix.hpp>

#include <string>
#include <map>

namespace bouge {

    /// \TODO: Write description.
    /// \note A bone with no transformation at all is called standard bone and maps to the vector (0,1,0).
    class BOUGE_API CoreBone
    {
    private:
        typedef std::map<std::string, CoreBonePtr> ChildrenMap;
    public:
        CoreBone(std::string name, Vector relativeRootPosition, Quaternion relativeBoneRotation, float length = 1.0f);
        virtual ~CoreBone();

        BOUGE_USER_DATA;

        std::string name() const;
        CoreBone& name(std::string name);

        BoneId id() const;
        CoreBone& id(BoneId id);

        CoreBone& addChild(CoreBonePtr child);
        CoreBone& removeChild(const std::string& name);
        std::size_t childCount() const;
        bool hasChild(const std::string& name) const;
        /// \exception std::logic_error
        CoreBonePtrC child(const std::string& name) const;
        /// \exception std::logic_error
        CoreBonePtr child(const std::string& name);

        bool hasParent() const;
        /// \exception std::logic_error
        CoreBonePtrC parent() const;
        /// \exception std::logic_error
        CoreBonePtr parent();
        CoreBone& parent(CoreBonePtr parent);

        // Getters and setters.

        float length() const;
        CoreBone& length(float len);

        /// \return the position of the bone's root (head), relative to the
        /// parent bone's root, in the "raw" form, that is without any pose applied.
        Vector relativeRootPosition() const;

        /// \return the current rotation of the bone.\n
        /// Imagine the bone has no parent, unit length and its root lies in the
        /// origin ; then, this is the quaternion that rotates the standard bone
        /// into this bone.\n
        /// Technically speaking, this is the bone's rotation relative to its parent.
        Quaternion relativeBoneRotation() const;

        Vector absoluteRootPosition() const;
        Quaternion absoluteBoneRotation() const;

        AffineMatrix boneSpaceToModelSpaceMatrix() const;
        AffineMatrix modelSpaceToBoneSpaceMatrix() const;

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
            friend class CoreBone;
            iterator(ChildrenMap::iterator me);
            ChildrenMap::iterator myIter;
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
            friend class CoreBone;
            const_iterator(ChildrenMap::const_iterator me);
            ChildrenMap::const_iterator myIter;
        };

        const_iterator begin() const;
        const_iterator end() const;

    protected:
        void updateAbsolute();
        void updateAbsoluteRecursive();

    private:
        std::string m_sName;
        CoreBonePtr m_pParent;

        ChildrenMap m_children;

        /// See the getter \a relativeRootPosition for a detailed description.
        Vector m_relativeRootPosition;

        /// See the getter \a relativeBoneRotation for a detailed description.
        Quaternion m_relativeBoneRotation;

        Vector m_absoluteRootPosition;
        Quaternion m_absoluteBoneRotation;

        AffineMatrix m_mBoneSpaceToModelSpace;

        /// This is really just needed for visualization when the bone either has
        /// no children or the children are supposed to be "detached". It is simply
        /// the length of this bone.
        float m_boneLength;

        /// The ID of the bone, in the skeleton. Used for optimized bone access.
        BoneId m_id;
    };

} // namespace bouge

#endif // BOUGE_COREBONE_HPP
