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
#ifndef BOUGE_EXCEPTION_HPP
#define BOUGE_EXCEPTION_HPP

#include <bouge/Config.hpp>

#include <exception>
#include <string>

namespace bouge {

    class BOUGE_API Exception : public std::exception
    {
    public:
        Exception(std::string what, const char* file, unsigned long line) throw();
        virtual ~Exception() throw();

        virtual const char* what() const throw();
        virtual const char* file() const;
        virtual unsigned long line() const;

    private:
        std::string m_what;
        const char* m_file;
        unsigned long m_line;
    };

    class BOUGE_API NotExistException : public Exception
    {
    public:
        NotExistException(std::string whoType, std::string whatnot, std::string whereType, std::string where, const char* file, unsigned long line) throw();
        virtual ~NotExistException() throw();

        virtual std::string who() const;
        virtual std::string where() const;
        virtual std::string whoType() const;
        virtual std::string whereType() const;

    private:
        std::string m_who;
        std::string m_where;
        std::string m_whoType;
        std::string m_whereType;
    };

    class BOUGE_API NoParentException : public Exception
    {
    public:
        NoParentException(std::string whoType, std::string who, const char* file, unsigned long line) throw();
        virtual ~NoParentException() throw();

        virtual std::string who() const;
        virtual std::string whoType() const;

    private:
        std::string m_who;
        std::string m_whoType;
    };

    class BOUGE_API BadDataException : public Exception
    {
    public:
        BadDataException(std::string what, const char* file, long unsigned int line) throw();
        virtual ~BadDataException() throw();
    };

} // namespace bouge

#endif // BOUGE_EXCEPTION_HPP
