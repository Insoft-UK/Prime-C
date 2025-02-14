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

#include "RECT.hpp"

void RECT_P(GROB& Gx, int x1, int y1, int x2, int y2, color_t edgeColor, color_t fillColor) {
    setDRAM(Gx.bytes);
    edgeColor = convertToRGBA(edgeColor);
    fillColor = convertToRGBA(fillColor);
    fillRect(x1, y1, x2 - x1, y2 - y1, invertAlphaChannel(fillColor));
    drawRect(x1, y1, x2 - x1, y2 - y1, invertAlphaChannel(edgeColor));
}

void RECT_P(GROB& Gx, int x1, int y1, int x2, int y2, color_t color) {
    setDRAM(Gx.bytes);
    color = convertToRGBA(color);
    fillRect(x1, y1, x2 - x1, y2 - y1, invertAlphaChannel(color));
}

void RECT_P(GROB& Gx, color_t color) {
    setDRAM(Gx.bytes);
    color = convertToRGBA(color);
    fillRect(0, 0, LCD_WIDTH_PX, LCD_HEIGHT_PX, invertAlphaChannel(color));
}
