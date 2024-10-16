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

#ifndef PIXON_P_hpp
#define PIXON_P_hpp

#include "graphics.h"

/**
 @brief    Plots a single pixel at the specified (x, y) coordinates.
 @param    Gx Specifies the GROB.
 @param    x   Specifies the x coordinate of the pixel.
 @param    y   Specifies the y coordinate of the pixel.
 @param    color  Specifies the color of the pixel. It is in ARGB 8888 format, alpha channel runs from 0(opaque) to 255(transparent).
 */
void PIXON_P(void* Gx, int x, int y, color_t color = 0);

/**
 @brief    Plots a single pixel at the specified (x, y) coordinates.
 @param    x   Specifies the x coordinate of the pixel.
 @param    y   Specifies the y coordinate of the pixel.
 @param    color  Specifies the color of the pixel. It is in ARGB 8888 format, alpha channel runs from 0(opaque) to 255(transparent).
 */
void PIXON_P(int x, int y, color_t color = 0);
    

#endif /* PIXON_P_hpp */
