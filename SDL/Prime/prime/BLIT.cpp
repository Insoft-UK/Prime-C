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

#include "BLIT.hpp"

void BLIT_P(GROB& trgtGx, int dx, int dy, int dw, int dh, GROB& srcGx, int sx, int sy, int sw, int sh, uint8_t alpha) {
    float scale_x = (float)(dw) / (float)sw;
    float scale_y = (float)(dh) / (float)sh;
    
    setDRAM(trgtGx.bytes);
    drawBitmapScaled(dx, dy, sw, sh, scale_x, scale_y, srcGx.bytes);
}

void BLIT_P(GROB& trgtGx, int dx, int dy, int dw, int dh, GROB& srcGx, uint8_t alpha) {
    float scale_x = (float)(dw) / (float)srcGx.width;
    float scale_y = (float)(dh) / (float)srcGx.height;
    
    setDRAM(trgtGx.bytes);
    drawBitmapScaled(dx, dy, srcGx.width, srcGx.height, scale_x, scale_y, srcGx.bytes);
}
