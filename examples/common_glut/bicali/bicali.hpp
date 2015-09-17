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

// TODO: documentation, tutorial
// TODO: multiple glyph pages
// TODO: text formatting
// TODO: example
// TODO: benchmark

// Setup/Cleanup:
//
// init(0, 0, 800, 600);
// deinit();
//
// Basic Usage:
//
// Font* pFont = load("Arial.png", "Arial.fnt");
// pFont->drawUtf8("Hello,\nWorld!", 10, 10);
// unload(pFont);
//
//
// Optimized usage:
//
// BufferedText* pText = pFont->bufferUtf8("Hello,\nWorld!");
// pText->draw(10, 10);
//
//
// More specific usage:
//
// Format f;
// f.drawbox(10, 10, 200, 200);
// f.align(right);
// f.wrap(true);
// pFont->drawUtf8("Hello,\nWorld!", f);
//
// Oneliner:
//
// pFont->drawUtf8("Hello,\nWorld!", Format(10, 10, 200, 200).align(right).wrap(true));
//
//
// Multiple viewports:
//
// FontManager *pMgr1 = FontManager::create();
// pMgr1->viewport(0, 0, 400, 600);
// Font* pFont = pMgr1->load("Arial.png", "Arial.fnt");
// pFont->...;
// pMgr1->unload(pFont);
// FontManager::destroy(pMgr1);

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
#include <set>
#include <list>

namespace bicali {

typedef unsigned long utf32;

////////////////////////////////////////////////////////////////////////////////
// Detail stuff. Uninteresting for most, but needs to be here.                //
// Except the shader class, which might be of more interest. Maybe take out?  //
////////////////////////////////////////////////////////////////////////////////
void* getEntryPoint(const char* name);

namespace detail {

class TextVBO {
public:
    TextVBO(const std::vector<float>& data, unsigned int type, unsigned int vert_coords_per_vert, unsigned int tex_coords_per_vert, unsigned int aVertex, unsigned int aTexCo);
    ~TextVBO();

    void upload(const std::vector<float>& data);
    void render() const;
private:
    unsigned int m_vaoid;
    unsigned int m_vboid;
    unsigned int m_type;
    unsigned int m_floats_per_vert;
    int m_nVerts;
};

class Shader {
public:
    Shader(const char* sVertCode, const char* sFragCode, const char* sGeomCode = 0);
    ~Shader();

    void use() const;
    unsigned int id() const { return m_progId; };
    bool hasAttrib(const char* name) const;
    bool hasAttrib(const std::string& name) const;
    unsigned int attrib(const char* name) const;
    unsigned int attrib(const std::string& name) const;
    bool hasUniform(const char* name) const;
    bool hasUniform(const std::string& name) const;
    unsigned int uniform(const char* name) const;
    unsigned int uniform(const std::string& name) const;

    bool uniformMatrix3fv(const std::string& name, unsigned int count, bool transpose, const float *value);
    bool uniformMatrix4fv(const std::string& name, unsigned int count, bool transpose, const float *value);
    bool uniformf(const std::string& name, float value);
    bool uniform1fv(const std::string& name, unsigned int count, const float *value);
    bool uniform2fv(const std::string& name, unsigned int count, const float *value);
    bool uniform3fv(const std::string& name, unsigned int count, const float *value);
    bool uniform4fv(const std::string& name, unsigned int count, const float *value);
    bool uniformi(const std::string& name, int value);
    bool uniform1iv(const std::string& name, unsigned int count, const int *value);
    bool uniform2iv(const std::string& name, unsigned int count, const int *value);
    bool uniform3iv(const std::string& name, unsigned int count, const int *value);
    bool uniform4iv(const std::string& name, unsigned int count, const int *value);

    static unsigned int load(const char* sVertCode, const char* sFragCode, const char* sGeomCode = 0, std::map<std::string, unsigned int>* out_attribs = 0, std::map<std::string, unsigned int>* out_uniforms = 0);

private:
    unsigned int m_progId;
    std::map<std::string, unsigned int> m_Attribs;
    std::map<std::string, unsigned int> m_Uniforms;
};

class Matrix4f {
public:
    Matrix4f();
    Matrix4f(const float* entries16f);
    ~Matrix4f();

    static Matrix4f viewproj(int w, int h);
    static Matrix4f transscale(float tx, float ty, float tz, float sx = 1.0f, float sy = 1.0f, float sz = 1.0f);

    inline operator const float*() const {return m;};
    Matrix4f operator*(const Matrix4f& other) const;

private:
    float m[16];
};

} // namespace bicali::detail

////////////////////////////////////////////////////////////////////////////////
// Glyph class. Describes one glyph, where it is and how it behaves.          //
////////////////////////////////////////////////////////////////////////////////
struct Glyph {
    unsigned int x, y, w, h;
    int xoffs, yoffs;
    int xadv, yadv;
    utf32 codepoint;

    Glyph();
    ~Glyph();
};

    /// \TODO Optimize by using, for example, a vector of maps, indexed by simple hash values.
typedef std::map<utf32, Glyph> GlyphHashMap;
// class GlyphHashMap {
// public:
//     Glyph& operator[](utf32 codepoint);
// private:
//     std::vector< std::list<Glyph> > map;
// };

////////////////////////////////////////////////////////////////////////////////
// Format class. For describing the formatting of the text.                   //
////////////////////////////////////////////////////////////////////////////////
enum Align {
    left,
    right,
    center,
};

class Format {
public:
    Format(int w = 0, int h = 0);

    Format& align(Align align);
    Format& wrap(bool wrap = true);
protected:
    int m_w, m_h;
    Align m_align;
    bool m_bWrap;
};

////////////////////////////////////////////////////////////////////////////////
// Static text class. Everything is fixed, set in stone. Forever. Ever.       //
////////////////////////////////////////////////////////////////////////////////
class StaticText {
public:
    StaticText& draw();

protected:
    StaticText(const char* str, int x, int y, const class BitmapFont* pFont, Format fmt, const float* tint4f = 0);
    ~StaticText();
    friend class BitmapFont;

    const class BitmapFont* m_pFont;
    detail::TextVBO m_vbo;
    float m_tint[4];
};

////////////////////////////////////////////////////////////////////////////////
// Buffered text class. Fixed text string and mode, but free pos and tint.    //
////////////////////////////////////////////////////////////////////////////////
class BufferedText {
public:
    BufferedText& draw(int x, int y, const float *in_vTint4f = 0);

protected:
    BufferedText(const char* str, Format fmt, const class BitmapFont* pFont);
    ~BufferedText();
    friend class BitmapFont;

    const class BitmapFont* m_pFont;
    detail::TextVBO m_vbo;
};

////////////////////////////////////////////////////////////////////////////////
// Dynamic text class. Text potentially changing every frame.                 //
////////////////////////////////////////////////////////////////////////////////
class DynamicText {
public:
    DynamicText& draw(const char* text, int x, int y, Format fmt = Format(), const float* in_vTint4f = 0);

protected:
    DynamicText(const class BitmapFont* pFont);
    ~DynamicText();
    friend class BitmapFont;

    const class BitmapFont* m_pFont;
    detail::TextVBO m_vbo;
};

////////////////////////////////////////////////////////////////////////////////
// Bitmap class. This is a small present for you, to draw a simple 2D bitmap  //
// on the screen without further ado.                                         //
////////////////////////////////////////////////////////////////////////////////
class Bitmap {
public:
    const Bitmap& draw(int x, int y, const float* in_vTint4f = 0) const;

    const detail::Matrix4f& viewproj() const;
    int selectTexture(unsigned int slot = 0) const;
    int deselectTexture(unsigned int slot = 0) const;

protected:
    Bitmap(const void* in_pixels, unsigned int w, unsigned int h, float z, const detail::Matrix4f& viewproj);
    ~Bitmap();
    friend class BitmapFontManager;

private:
    unsigned int m_TexId;
    unsigned int m_w, m_h;

    const detail::Matrix4f& m_viewproj;
    detail::TextVBO m_myTexVBO;
};

////////////////////////////////////////////////////////////////////////////////
// BitmapFont class. Represents one bitmap font and its corresponding graphic //
// resources, as well as all glyphs in it.                                    //
// Use to create [Static|Buffered|Dynamic]Text objects to draw text.          //
////////////////////////////////////////////////////////////////////////////////
class BitmapFont : public Bitmap {
public:
    void glyph(utf32 codepoint, Glyph glyph);
    Glyph glyph(utf32 codepoint) const;
    Glyph& glyph(utf32 codepoint);
    bool hasGlyph(utf32 codepoint) const;
    unsigned int glyphCount() const;

    BitmapFont& drawUtf8(const char* str, int x, int y, Format fmt = Format(), const float* in_vTint4f = 0);
    StaticText* bufferUtf8(const char* str, int x = 0, int y = 0, Format fmt = Format(), const float* tint4f = 0);
    BufferedText* bufferUtf8(const char* str, const Format& fmt);
    std::pair<int, int> sizeUtf8(const char* str, Format fmt = Format());
    BitmapFont& free(BufferedText* bt);
    BitmapFont& free(StaticText* st);

    unsigned int m_lineH;
    unsigned int m_base;

protected:
    BitmapFont(const void* in_pixels, unsigned int w, unsigned int h, const detail::Matrix4f& viewproj);
    ~BitmapFont();
    friend class BitmapFontManager;

private:
    GlyphHashMap m_glyphs;

    /// Used only for automatic cleanup.
    std::set<StaticText*> m_staticTexts;

    /// Used only for automatic cleanup.
    std::set<BufferedText*> m_bufferedTexts;

    /// Each font has one dynamic text object, for use with their unbuffered draw methods.
    DynamicText m_dynText;
};

////////////////////////////////////////////////////////////////////////////////
// BitmapFont manager class. One per viewport is enough. Holds per-vp data.   //
////////////////////////////////////////////////////////////////////////////////
class BitmapFontManager {
public:
    static BitmapFontManager* create();
    static void destroy(BitmapFontManager* mgr);

    BitmapFontManager& viewport(/*int x, int y, */int w, int h);

#if D_BICALI_EMBED_PICOPNG
    /// \throws std::runtime_error If anything failed. (init GL, load texture, ...)
    BitmapFont* load(const char* imgFilename, const char*descFilename);
    /// \throws std::runtime_error If anything failed. (init GL, load texture, ...)
    Bitmap* loadImage(const char* imgFilename);
#endif

#if D_BICALI_DEFAULTFONTS
    BitmapFont* default_small();
#endif

    BitmapFont* make(const std::vector<unsigned char>& in_pixels, unsigned int w, unsigned int h, const char* desc);
    BitmapFont* make(const void* in_pixels, unsigned int w, unsigned int h, const char* desc);
    Bitmap* makeImage(const std::vector<unsigned char>& in_pixels, unsigned int w, unsigned int h, float z = 0.5f);
    Bitmap* makeImage(const void* in_pixels, unsigned int w, unsigned int h, float z = 0.5f);
    BitmapFontManager& unload(BitmapFont* pFont);
    BitmapFontManager& unload(Bitmap* pImage);

protected:
    BitmapFontManager();
    ~BitmapFontManager();

    friend void init(/*int x, int y,*/ int w, int h);
    friend void deinit();

    std::set<BitmapFont*> m_fonts;
    std::set<Bitmap*> m_images;
    detail::Matrix4f m_viewproj;

#if D_BICALI_DEFAULTFONTS
    BitmapFont m_defaultSmall;
#endif

#if D_BICALI_EMBED_PICOPNG
    void loadCommon(const char* imgFilename, std::vector<unsigned char>& imgData, unsigned long& w, unsigned long& h);
#endif
};

////////////////////////////////////////////////////////////////////////////////
// Global functions easing the basic use. Actually just hiding a default      //
// manager behind the scenes, as only one is needed in most cases anyway.     //
////////////////////////////////////////////////////////////////////////////////
void init(/*int x = 0, int y = 0,*/ int w = 0, int h = 0);
void deinit();
void viewport(/*int x, int y, */int w, int h);

#if D_BICALI_EMBED_PICOPNG
/// \throws std::runtime_error If anything failed. (init GL, load texture, ...)
BitmapFont* load_font(const char* imgFilename, const char* descFilename);
/// \throws std::runtime_error If anything failed. (init GL, load texture, ...)
Bitmap* load_image(const char* imgFilename);
#endif

#if D_BICALI_DEFAULTFONTS
BitmapFont* default_small_font();
#endif

BitmapFont* make_font(const std::vector<unsigned char>& in_pixels, unsigned int w, unsigned int h, const char* desc);
BitmapFont* make_font(const void* in_pixels, unsigned int w, unsigned int h, const char* desc);
Bitmap* make_image(const std::vector<unsigned char>& in_pixels, unsigned int w, unsigned int h);
Bitmap* make_image(const void* in_pixels, unsigned int w, unsigned int h);
void unload_font(BitmapFont* pFont);
void unload_image(Bitmap* pImage);

} // namespace bicali

#endif // D_BICALI_HPP
