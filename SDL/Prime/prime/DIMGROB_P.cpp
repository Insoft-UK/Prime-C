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

#include "DIMGROB_P.hpp"
#include <iostream>

void DIMGROB_P(GROB& Gx, int width, int height, color_t color) {
    if (Gx.bytes != nullptr) {
        free(Gx.bytes);
        Gx = {};
    }
    
    if (width <= 0 && height <= 0) return;
    Gx.bytes = malloc(width * height * 4);
    if (Gx.bytes== nullptr) return;
    
    Gx.width = width;
    Gx.height = height;
    setDRAM(Gx.bytes);
    fillArea(0, 0, width, height, color);

}


void DIMGROB_P(GROB& Gx, int width, int height, UList64& list) {
    if (Gx.bytes != nullptr) {
        free(Gx.bytes);
        Gx = {};
    }
    
    if (list.size() == 0) return;
    
    if (width <= 0 && height <= 0) return;
    Gx.bytes = malloc(width * height * 4);
    if (Gx.bytes== nullptr) return;
    
    Gx.width = width;
    Gx.height = height;
    memcpy(Gx.bytes, list.data(), width * height * 4);

}
