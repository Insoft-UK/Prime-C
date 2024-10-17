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

#ifndef RECT_P_hpp
#define RECT_P_hpp

#include "graphics.h"
#include "GROB.hpp"

/**
 The Alpha Channel  0 (opaque) to 255 (transparent).
 
 @brief    Draw a rectangle with no fill color
 @param    x1   Top left corner x coordinate
 @param    y1   Top left corner y coordinate
 @param    x2   Width in pixels
 @param    y2   Height in pixels
 @param    edgeColor Specifies what edge color to draw with. It is in ARGB 8888 format.
 @param    fillColor Specifies what fill color to draw with. It is in ARGB 8888 format.
 */
void RECT_P(GROB& Gx, int x1, int y1, int x2, int y2, color_t edgeColor, color_t fillColor);

/**
 The Alpha Channel  0 (opaque) to 255 (transparent).
 
 @brief    Draw a rectangle with no fill color
 @param    x1   Top left corner x coordinate
 @param    y1   Top left corner y coordinate
 @param    x2   Width in pixels
 @param    y2   Height in pixels
 @param    color Specifies what fill color to draw with. It is in ARGB 8888 format.
 */
void RECT_P(GROB& Gx, int x1, int y1, int x2, int y2, color_t color = 0xFFFFFF);
    
/**
 The Alpha Channel  0 (opaque) to 255 (transparent).
 
 @brief    Draw a rectangle with no fill color
 @param    Gx GROB
 @param    color Specifies what fill color to draw with. It is in ARGB 8888 format.
 */
void RECT_P(GROB& Gx, color_t color = 0xFFFFFF);

#endif /* RECT_P_hpp */
