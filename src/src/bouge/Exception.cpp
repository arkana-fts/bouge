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
#include <bouge/Exception.hpp>

namespace bouge {

    Exception::Exception(std::string what, const char* file, unsigned long line) throw()
        : m_what(what)
        , m_file(file)
        , m_line(line)
    { }

    Exception::~Exception() throw()
    { }

    const char* Exception::what() const throw()
    {
        return m_what.c_str();
    }

    const char* Exception::file() const
    {
        return m_file;
    }

    unsigned long Exception::line() const
    {
        return m_line;
    }

    NotExistException::NotExistException(std::string whoType, std::string who, std::string whereType, std::string where, const char* file, unsigned long line) throw()
        : Exception(whoType + " " + who + " does not exist in " + whereType + " " + where, file, line)
        , m_who(who)
        , m_where(where)
        , m_whoType(whoType)
        , m_whereType(whereType)
    { }

    NotExistException::~NotExistException() throw()
    { }

    std::string NotExistException::who() const
    {
        return m_who;
    }

    std::string NotExistException::where() const
    {
        return m_where;
    }

    std::string NotExistException::whoType() const
    {
        return m_whoType;
    }

    std::string NotExistException::whereType() const
    {
        return m_whereType;
    }

    NoParentException::NoParentException(std::string whoType, std::string who, const char* file, unsigned long line) throw()
        : Exception(whoType + " " + who + " has no parent", file, line)
        , m_who(who)
        , m_whoType(whoType)
    { }

    NoParentException::~NoParentException() throw()
    { }

    std::string NoParentException::who() const
    {
        return m_who;
    }

    std::string NoParentException::whoType() const
    {
        return m_whoType;
    }

    BadDataException::BadDataException(std::string what, const char* file, unsigned long line) throw()
        : Exception(what, file, line)
    { }

    BadDataException::~BadDataException() throw()
    { }

}
