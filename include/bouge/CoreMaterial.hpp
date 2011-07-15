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
#ifndef BOUGE_COREMATERIAL_HPP
#define BOUGE_COREMATERIAL_HPP

#include <bouge/bougefwd.hpp>

#include <string>
#include <map>
#include <vector>

namespace bouge {

    class BOUGE_API CoreMaterial
    {
        class PropValue {
        public:
            PropValue(std::string val = "");

            // Simple value getting/setting
            const std::string& val() const;
            PropValue& val(std::string val);

            // May use cached conversions as an optimization.
            const std::vector<float>& valAsFvec() const;
        private:
            std::string m_val;
            mutable std::vector<float> m_valAsFvecCache;
        };

        typedef std::map<std::string, PropValue> PropretyMap;
    public:
        CoreMaterial(std::string name);
        virtual ~CoreMaterial();

        BOUGE_USER_DATA;

        std::string name() const;
        CoreMaterial& name(std::string name);

        bool hasProprety(const std::string& name) const;
        const std::string& proprety(const std::string& name) const;
        const std::string& propretyOrDefault(const std::string& name, const std::string& def) const;
        /// \exception std::invalid_argument
        const std::vector<float>& propretyAsFvec(const std::string& name) const;
        CoreMaterial& proprety(const std::string& name, std::string value);
        CoreMaterial& proprety(const std::string& name, std::vector<float> value);

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

            const std::string& name() const;
            const std::string& value() const;
            const std::vector<float>& valueAsFvec() const;
            void value(std::string value);
            void value(std::vector<float> value);
        private:
            friend class CoreMaterial;
            iterator(PropretyMap::iterator me);
            PropretyMap::iterator myIter;
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

            const std::string& name() const;
            const std::string& value() const;
            const std::vector<float>& valueAsFvec() const;
        private:
            friend class CoreMaterial;
            const_iterator(PropretyMap::const_iterator me);
            PropretyMap::const_iterator myIter;
        };

        const_iterator begin() const;
        const_iterator end() const;

    private:
        std::string m_sName;

        PropretyMap m_propreties;
    };

} // namespace bouge

#endif // BOUGE_COREMATERIAL_HPP
