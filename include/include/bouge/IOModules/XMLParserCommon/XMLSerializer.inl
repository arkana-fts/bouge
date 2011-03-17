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
template<class OutStream>
XMLSerializer<OutStream>::XMLSerializer(OutStream& out, size_t indentSpace)
    : m_error(false)
    , m_depth(0)
    , m_indentSpace(indentSpace)
    , m_needClose(false)
    , m_lastIsText(false)
    , m_stream(out)
{
    m_stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
    m_error = ! m_stream;
}

template<class OutStream>
XMLSerializer<OutStream>::~XMLSerializer()
{
    if(!m_error || !m_tagStack.empty()) {
        m_stream << std::endl;
    }
}

template<class OutStream>
XMLSerializer<OutStream>& XMLSerializer<OutStream>::openTag(const std::string& name)
{
    if(!m_error) {
        ++m_tagCount;
        if(m_needClose) {
            m_stream << '>';
        }
        if(!m_lastIsText) {
            m_stream << std::endl;
            indentLine();
        }
        m_stream << '<' << name.c_str() /*<< ' '*/;
        m_tagStack.push_back(name);
        ++m_depth;
        m_needClose = true;
        m_lastIsText = false;
        m_error = !m_stream;
    }
    return *this;
}

template<class OutStream>
XMLSerializer<OutStream>& XMLSerializer<OutStream>::closeTag()
{
    std::string back = m_tagStack.back();
    if(!m_error) {
        --m_depth;
        if(m_needClose) {
            m_stream << " />";
        } else if(!m_lastIsText) {
            m_stream << std::endl;
            indentLine();
            m_stream << "</" << back << '>';
        } else {
            m_stream << "</" << back << '>';
        }
        m_lastIsText = false;
        m_needClose = false;
        m_tagStack.pop_back();
        m_error = ! m_stream;
    }
    return *this;
}

template<class OutStream>
XMLSerializer<OutStream>& XMLSerializer<OutStream>::attribute(const std::string& name, const std::string& value)
{
    if(!m_needClose) {
        m_error = true;
    }
    if(!m_error) {
        m_stream << ' ' << name << "=\"" << convertEntityInAttribute(value) << "\"";
        m_lastIsText = false;
        m_error = !m_stream;
    }
    return *this;
}

template<class OutStream>
XMLSerializer<OutStream>& XMLSerializer<OutStream>::text(const std::string& text)
{
    if(!m_error) {
        if(m_needClose) {
            m_stream << '>';
            m_needClose = false;
        }
        m_stream << convertEntityInText(text);
        m_lastIsText = true;
        m_error = !m_stream;
    }
    return *this;
}

template<class OutStream>
unsigned int XMLSerializer<OutStream>::getTagCount() const
{
    return m_tagCount;
}

template<class OutStream>
void XMLSerializer<OutStream>::indentLine()
{
    size_t spaceCount = m_depth * m_indentSpace;

    // There's for sure a best way to do this but it works
    for(size_t i = 0 ; i < spaceCount ; ++i) {
        m_stream << ' ';
    }
}

template<class OutStream>
std::string XMLSerializer<OutStream>::convertEntityInText(const std::string& text)
{
    // Reserve a lot of space
    std::string res;
    res.reserve(text.size()*2);

    const std::string::const_iterator iterEnd = text.end();
    for(std::string::const_iterator iter = text.begin() ; iter != iterEnd ; ++iter) {
        switch(*iter) {
        case '<':
            res += "&lt;";
            break;

        case '>':
            res += "&gt;";
            break;

        case '&':
            res += "&amp;";
            break;

        case '\'':
            res += "&apos;";
            break;

        case '"':
            res += "&quot;";
            break;

        default:
            res += *iter;
        }
    }
    return res;
}

template<class OutStream>
std::string XMLSerializer<OutStream>::convertEntityInAttribute(const std::string& attributeValue)
{
    // Reserve a lot of space
    std::string res;
    res.reserve(attributeValue.size()*2);

    const std::string::const_iterator iterEnd = attributeValue.end();
    for(std::string::const_iterator iter = attributeValue.begin() ; iter != iterEnd ; ++iter) {
        switch(*iter) {
        case '<':
            res += "&lt;";
            break;

        case '>':
            res += "&gt;";
            break;

        case '&':
            res += "&amp;";
            break;

        case '\'':
            res += "&apos;";
            break;

        case '"':
            res += "&quot;";
            break;

        case '\n':
            res += "\\n";
            break;
        default:
            res += *iter;
        }
    }
    return res;
}
