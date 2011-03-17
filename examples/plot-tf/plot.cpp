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
#include <bouge/Math/TimeFunction.hpp>

#include "lodepng.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>

using namespace bouge;

#define w 200
#define h 200
#define tmax 4.0f

inline void set(std::vector<unsigned char>& img, unsigned int x, unsigned int y)
{
    img[(y*w+x)*4+0] = 0;
    img[(y*w+x)*4+1] = 0;
    img[(y*w+x)*4+2] = 0;
    img[(y*w+x)*4+3] = 255;
}

inline unsigned int y(float v)
{
    // Trick to start with y=0 at the bottom and go up.
    unsigned int ret = h-1-(unsigned int)(v*((float)(h-1))*0.25f);
    if(ret >= h)
        throw std::runtime_error("too high y");
    return ret;
}

int main(int argc, const char* argv[])
{
    TimeFunction* f = 0;

    // Interpret the arguments as sourcecode.
    std::stringstream ss;
    for(int i = 1 ; i < argc ; ++i) {
        ss << argv[i] << " ";
    }

    // Interpret the sourcecode.
    std::string keyword;
    while(ss >> keyword) {
        if(keyword == "const") {
            float c = 0.0f;
            ss >> c;
            f = new ConstantTF(c);
        } else if(keyword == "linear") {
            float s = 0.0f;
            ss >> s;
            f = new LinearTF(s);
        } else if(keyword == "repeat") {
            if(!f) { std::cerr << "Syntax error: repeat modifier without preceding f" << std::endl; continue; }
            f = new RepeatTF(f);
        } else if(keyword == "cycle") {
            if(!f) { std::cerr << "Syntax error: cycle modifier without preceding f" << std::endl; continue; }
            f = new CycleTF(f);
        } else if(keyword == "hold") {
            if(!f) { std::cerr << "Syntax error: hold modifier without preceding f" << std::endl; continue; }
            f = new HoldTF(f);
        } else if(keyword == "fade-in") {
            float fadeTime = 0.0f;
            ss >> fadeTime;
            if(!f) { std::cerr << "Syntax error: fade-in modifier without preceding f" << std::endl; continue; }
            f = new FadeInTF(fadeTime, f);
        } else if(keyword == "fade-out") {
            float fadeTime = 0.0f;
            ss >> fadeTime;
            if(!f) { std::cerr << "Syntax error: fade-out modifier without preceding f" << std::endl; continue; }
            f = new FadeOutTF(fadeTime, f);
        }
    }

    if(!f) {
        f = new ConstantTF(1.0f);
    }

    std::vector<unsigned char>img(w*h*4, 255);
    unsigned int pixelsOOR = 0;
    for(unsigned int i = 0 ; i < w ; ++i) {
        float t = (float)i/(float)w*tmax;
        try {
            set(img, i, y((*f)(t)));
        } catch(const std::runtime_error&) {++pixelsOOR;};
    }

    if(pixelsOOR > 0)
        std::cerr << pixelsOOR << " pixels out of range" << std::endl;

    std::vector<unsigned char> buffer;
    LodePNG::Encoder encoder;
    encoder.getSettings().zlibsettings.windowSize = 2048;
    encoder.encode(buffer, &img[0], w, h);
    LodePNG::saveFile(buffer, "out.png");

    return 0;
}
