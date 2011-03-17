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

#ifndef BOUGE_XML_SERIALIZER_HPP
#define BOUGE_XML_SERIALIZER_HPP

// Note: This file is heavily inspired/copied from the CEGUI project.

#include <bouge/Config.hpp>

#include <string>
#include <vector>
#include <iterator>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace bouge
{
    /// Class used to create XML Document.
    ///
    /// This class hides the complexity of formatting valid XML files. The
    /// class provides automatic substitution of entities, XML indenting
    /// in respect of the spaces. The following
    /// example show the code needed to exports parts of an XML document
    /// similar to what can be found in a layout.
    ///
    /// @code
    /// #include <iostream>
    /// #include <bouge/XMLSerializer.h>
    ///
    /// int main()
    /// {
    ///    // Create an encoder that outputs its result on standard output
    ///    XMLSerializer xml(std::cout, 4);
    ///    xml.openTag("Window")
    ///       .attribute("Type", "TaharezLook/StaticText")
    ///       .attribute("Name", "Test")
    ///       .openTag("Property")
    ///       .attribute("Name", "Text")
    ///       .text("This is the static text to be displayed")
    ///       .closeTag()
    ///       .openTag("Window")
    ///       .attribute("Name", "Button")
    ///       .attribute("Type", "Vanilla/Button")
    ///       .openTag("Property")
    ///       .attribute("Name", "Text")
    ///       .attribute("Value", "Push me")
    ///       .closeTag()
    ///       .closeTag()
    ///       .closeTag();
    ///
    ///    if (xml)
    ///    {
    ///        std::cout << "XML Exported successfully" << std::endl;
    ///    }
    ///    return 0;
    /// }
    /// @endcode
    template<class OutStream>
    class BOUGE_API XMLSerializer
    {
    public:
        /// XMLSerializer constructor
        /// \param out The stream to use to export the result
        /// \param indentSpace The indentation level (0 to disable indentation)
        XMLSerializer(OutStream& out, size_t indentSpace = 4);

        /// XMLSerializer destructor
        virtual ~XMLSerializer();

        /// Start a new tag in the xml document.
        /// \param name The name of the tag
        /// \return A reference to the current object for chaining operation
        XMLSerializer& openTag(const std::string& name);

        /// brief Close the current tag.
        /// \return A reference to the current object for chaining operation
        XMLSerializer& closeTag(void);

        /// After an opening tag you can populate attribute list with this function
        /// \param name The name of the attribute
        /// \param value The value of the attribute
        /// \return A reference to the current object for chaining operation
        XMLSerializer& attribute(const std::string& name, const std::string& value);

        /// Create a text node
        /// \param text the content of the node
        /// \return A reference to the current object for chaining operation
        XMLSerializer& text(const std::string& text);

        /// report the nimber of tags created in the document
        /// \return return the number of tag created in the document
        unsigned int getTagCount() const;

        /// Check wether the XML Serializer status is valid
        /// \return True if all previous operations where successfull
        operator bool () const
        {
            return false == m_error;
        }

        /// Check wether the XML Serializer status is invalid
        /// \return True if one operations failed
        bool operator!() const
        {
            return false != m_error;
        }

    protected:
    private:
        bool m_error; //!< Store the status of the serializer
        unsigned int m_tagCount; //!<Return the number of tag in the document
        size_t m_depth; //!< Store the current depth for indentation purpose
        size_t m_indentSpace; //!< Store the number of space use for indenting
        bool m_needClose; //!< Store whether the next operation need to close the tag or not
        bool m_lastIsText; //!< Store whether the last operation was a text node or not
        OutStream& m_stream; //!< A reference to the stream object use
        std::vector<std::string> m_tagStack; //!< Store the tag stack for correct closing of the tags.

        /// put padding in the stream before line data
        void indentLine();

        /// convert special char to there corresponding entity in text data.
        static std::string convertEntityInText(const std::string& text);

        /// convert special char into entities including line ending for use in attributes.
        static std::string convertEntityInAttribute(const std::string& attributeValue);

        // Disabled operation
        XMLSerializer(const XMLSerializer& obj);
        // Disabled operation
        XMLSerializer& operator=(const XMLSerializer& obj);
    };

#include "XMLSerializer.inl"

} // namespace bouge

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif // BOUGE_XML_SERIALIZER_HPP
