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

#ifndef BOUGE_XML_HANDLER_HPP
#define BOUGE_XML_HANDLER_HPP

// Note: This file is heavily inspired/copied from the CEGUI project.

#include <bouge/Config.hpp>

#include <string>

namespace bouge
{
    class XMLAttributes;

    class BOUGE_API XMLHandler
    {
    public:
        /// XMLHandler base class constructor.
        XMLHandler();

        /// XMLHandler base class destructor.
        virtual ~XMLHandler();

        /// Method called to notify the handler at the start of each XML element encountered.
        /// \param element String object holding the name of the element that is starting.
        /// \param attributes An XMLAttributes object holding the collection of attributes specified for the element.
        virtual void elementStart(const std::string& element, const XMLAttributes& attributes);

        /// Method called to notify the handler at the end of each XML element encountered.
        /// \param element String object holding the name of the element that is ending.
        virtual void elementEnd(const std::string& element);

        /// Called for an element containing only text, no children.
        /// \param text String object holding the content of the text node.
        virtual void textElement(const std::string& element, const XMLAttributes& attributes, const std::string& text);

        /// Called for any element encountered, decides whether to enter an element or not.
        /// \return True if the element should be entered, false if not.
        virtual bool wantsToEnter(const std::string& element);
    };

} // namespace bouge


#endif  // BOUGE_XML_HANDLER_HPP
