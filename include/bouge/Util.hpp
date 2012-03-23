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
#ifndef BOUGE_UTIL_HPP
#define BOUGE_UTIL_HPP

#include <bouge/Config.hpp>

#include <string>
#include <sstream>
#include <vector>

namespace bouge {

    bool BOUGE_API ansiieq(const std::string& lhs, const std::string& rhs);
    std::string BOUGE_API ansitolower(std::string orig);

    template <class T>
    std::string BOUGE_API to_s(const T& t)
    {
        std::stringstream ss;
        ss << t;
        return ss.str();
    }

    std::string BOUGE_API fast_to_s(std::size_t i);

    template <class T>
    T BOUGE_API to(std::string s)
    {
        T t;
        std::stringstream ss(s);
        ss >> t;
        return t;
    }

    template <>
    std::string BOUGE_API to_s< std::vector<float> >(const std::vector<float>& t);

    template<>
    std::vector<float> BOUGE_API to< std::vector<float> >(std::string s);

    template <class InputIterator>
    std::string BOUGE_API to_s(InputIterator begin, const InputIterator& end, const std::string& glue = " ")
    {
        InputIterator i = begin;

        std::stringstream ss;
        while(i != end) {
            ss << *i << glue;
            ++i;
        }

        // remove the trailing space.
        std::string ret = ss.str();
        if(ret.size() >= glue.size()) {
            ret.resize(ret.size()-glue.size());
        }

        return ret;
    }

} // namespace bouge

#endif // BOUGE_UTIL_HPP
