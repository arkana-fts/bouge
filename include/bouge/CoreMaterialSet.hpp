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
#ifndef BOUGE_COREMATERIALSET_HPP
#define BOUGE_COREMATERIALSET_HPP

#include <bouge/bougefwd.hpp>

#include <map>
#include <string>

namespace bouge {

    class BOUGE_API CoreMaterialSet
    {
        typedef std::map<std::string, std::string> AssossiationsMap;
    public:
        CoreMaterialSet(std::string name);
        virtual ~CoreMaterialSet();

        BOUGE_USER_DATA;

        std::string name() const;
        CoreMaterialSet& name(std::string name);

        /// \exception std::invalid_argument
        std::string materialForMesh(const std::string& meshname) const;
        CoreMaterialSet& materialForMesh(std::string meshname, std::string matname);

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

            std::string meshname() const;
            std::string matname() const;
            void matname(std::string value);
        private:
            friend class CoreMaterialSet;
            iterator(AssossiationsMap::iterator me);
            AssossiationsMap::iterator myIter;
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

            std::string meshname() const;
            std::string matname() const;
        private:
            friend class CoreMaterialSet;
            const_iterator(AssossiationsMap::const_iterator me);
            AssossiationsMap::const_iterator myIter;
        };

        const_iterator begin() const;
        const_iterator end() const;

    private:
        std::string m_sName;

        AssossiationsMap m_sAssossiations;
    };

} // namespace bouge

#endif // BOUGE_COREMATERIALSET_HPP
