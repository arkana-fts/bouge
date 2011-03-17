////////////////////////////////////////////////////////////
//
// BiCali - OpenGL 3 bitmap font system
// Copyright (C) 2011 Lucas Beyer (pompei2@gmail.com)
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
#ifndef D_BICALI_HPP
#define D_BICALI_HPP

// Basic Usage:
//
// Font* pFont = load_font("Arial.png", "Arial.fnt");
// pFont->drawTextUtf8("Hello,\nWorld!", 10, 10);
// destroy_font(pFont);
//

#ifdef D_BICALI_NO_PNG
#  define D_BICALI_EMBED_PICOPNG 0
#else
#  define D_BICALI_EMBED_PICOPNG 1
#endif

#ifdef D_BICALI_NO_DEFAULTFONTS
#  define D_BICALI_DEFAULTFONTS 0
#else
#  define D_BICALI_DEFAULTFONTS 1
#endif

#include <string>
#include <vector>
#include <map>

namespace bicali {

typedef unsigned long utf32;

struct Glyph {
    unsigned int x, y;
    unsigned int w, h;
    int xoffs, yoffs;
    int xadv, yadv;
    utf32 codepoint;

    Glyph();
    ~Glyph();
};

class BitmapFont {
public:
    ~BitmapFont();

    void glyph(utf32 codepoint, Glyph glyph);
    Glyph glyph(utf32 codepoint) const;
    Glyph& glyph(utf32 codepoint);
    bool hasGlyph(utf32 codepoint) const;
    unsigned int glyphCount() const;

    void drawTextUtf8(const char* str, int x = 0, int y = 0);

    unsigned int m_lineH;
    unsigned int m_base;

protected:
    BitmapFont(const void* in_pixels, unsigned int w, unsigned int h);

    friend BitmapFont* make_font(const void* in_pixels, unsigned int w, unsigned int h, const char* desc);
    friend BitmapFont* default_small_font();

private:
    unsigned int m_fontTex;
    unsigned int m_fontFBO;

    unsigned int m_w, m_h;

    /// \TODO Optimize by using, for example, a vector of maps, indexed by simple hash values.
    std::map<utf32, Glyph> m_glyphs;
};

#if D_BICALI_EMBED_PICOPNG
/// \throws std::runtime_error If anything failed. (init GL, load texture, ...)
BitmapFont* load_font(const char* imgFilename, const char* descFilename);
#endif

#if D_BICALI_DEFAULTFONTS
BitmapFont* default_small_font();
#endif

BitmapFont* make_font(const std::vector<unsigned char>& in_pixels, unsigned int w, unsigned int h, const char* desc);
BitmapFont* make_font(const void* in_pixels, unsigned int w, unsigned int h, const char* desc);
void destroy_font(BitmapFont* pFont);

} // namespace bicali

#endif // D_BICALI_HPP
