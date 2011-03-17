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
#include <bouge/Util.hpp>

#include <locale>
#include <algorithm>

namespace bouge
{

    bool ansiieq(const std::string& lhs, const std::string& rhs)
    {
        unsigned int sz = lhs.size();
        if(rhs.size() != sz)
            return false;
        for(unsigned int i = 0; i < sz; ++i)
            if(tolower(lhs[i]) != tolower(rhs[i]))
                return false;
        return true;
    }

    struct ToLower {
    public:
        std::string::value_type operator()(std::string::value_type ch) const {
            return std::tolower(ch, std::locale());
        }
    };

    std::string ansitolower(std::string orig)
    {
        std::transform(orig.begin(), orig.end(), orig.begin(), ToLower());
        return orig;
    }

    std::string fast_to_s(std::size_t i)
    {
        if(i < 10)
            return std::string(1, '0' + i);

        return to_s(i);
    }

    template <>
    std::string to_s< std::vector<float> >(const std::vector<float>& t)
    {
        return to_s(t.begin(), t.end());
    }

    template<>
    std::vector<float> to< std::vector<float> >(std::string s)
    {
        std::stringstream ssText(s);
        std::vector<float> ret;

        float f;
        for(ssText >> f ; ssText ; ssText >> f) {
            ret.push_back(f);
        }
        return ret;
    }

}