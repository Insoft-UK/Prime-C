/*
 The MIT License (MIT)
 
 Copyright (c) 2024 Insoft. All rights reserved.
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include "DIMGROB.hpp"
#include "RECT.hpp"

#include <iostream>

void DIMGROB_P(GROB& Gx, int width, int height, color_t color) {
    if (Gx.bytes != nullptr) {
        free(Gx.bytes);
        Gx = {};
    }
    
    if (width <= 0 && height <= 0) return;
    Gx.bytes = malloc(width * height * sizeof(color_t));
    if (Gx.bytes== nullptr) return;
    
    Gx.width = width;
    Gx.height = height;
    RECT_P(Gx, 0, 0, width, height, color);
}


void DIMGROB_P(GROB& Gx, int width, int height, BLOB& list) {
    if (Gx.bytes != nullptr) {
        free(Gx.bytes);
        Gx = {};
    }
    
    if (list.size() == 0) return;
    
    if (width <= 0 && height <= 0) return;
    Gx.bytes = malloc(width * height * sizeof(color_t));
    if (Gx.bytes == nullptr) return;
    
    unsigned long lengthInBytes = list.size() * 8;
    color_t* dest = (color_t *)Gx.bytes;
    

    if (width * height * 2 == lengthInBytes) {
        // HighColor
        uint16_t* src = (uint16_t *)list.data();
        
        for (int i = 0; i < list.size(); i++) {
            *dest++ = invertAlphaChannel(convertHighColorToTrueColor(*src++));
            *dest++ = invertAlphaChannel(convertHighColorToTrueColor(*src++));
            *dest++ = invertAlphaChannel(convertHighColorToTrueColor(*src++));
            *dest++ = invertAlphaChannel(convertHighColorToTrueColor(*src++));
        }
        
    }
    
    if (width * height * 4 == lengthInBytes) {
        // TrueColor
        uint32_t* src = (uint32_t *)list.data();
        
        for (int i = 0; i < list.size(); i++) {
            color_t argb;
            
            argb = *src++;
            argb = (argb & 0xFF) << 16 | (argb & 0xFF0000) >> 16 | (argb & 0xFF00FF00);
            *dest++ = invertAlphaChannel(argb);
            
            argb = *src++;
            argb = (argb & 0xFF) << 16 | (argb & 0xFF0000) >> 16 | (argb & 0xFF00FF00);
            *dest++ = invertAlphaChannel(argb);
        }
    }
    
    Gx.width = width;
    Gx.height = height;
}
