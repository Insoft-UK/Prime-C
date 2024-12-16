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

#include "graphics.h"


static void* _DRAM = NULL;

void setDRAM(void* address) {
    if (address == NULL) {
        _DRAM = displayRAM();
        return;
    }
    _DRAM = address;
}

static void* getDRAM(void) {
    if (_DRAM == NULL) {
        return displayRAM();
    }
    return _DRAM;
}


/**
 Based on Alpha, Red, Green and Blue components values (0 to 31)
 The Alpha Channel  1 (opaque) to 0 (transparent).
 
 @brief    Returns 32-bit TrueColor RGBA.
 @param    color Specifies the color will be converted. It is in RGBA 1555  format.
 @returns  An 32-bit unsigned integer number that can be used as
           the color parameter for a drawing function.
 */
color_t convertHighColorToTrueColor(uint16_t color) {
    uint32_t r, g, b, a;
    
    r = (color & 0b111110000000000) >> 7;
    g = (color & 0b1111100000) >> 2;
    b = (color & 0b11111) << 3;
    a = color & 0x8000 ? 255 : 0;
    
#ifdef __LITTLE_ENDIAN__
    color_t rgba = r | g << 8 | b << 16 | a << 24;
    rgba |= (rgba & 0b000010000000100000001000) >> 3;
    rgba |= (rgba & 0b001000000010000000100000) >> 4;
    rgba |= (rgba & 0b100000001000000010000000) >> 5;
#else
    color_t rgba = r << 24 | g << 16 | b << 8 | a;
    rgba |= (rgba & 0b00001000000010000000100000000000) >> 3;
    rgba |= (rgba & 0b00100000001000000010000000000000) >> 4;
    rgba |= (rgba & 0b10000000100000001000000000000000) >> 5;
#endif
   
    return rgba;
}

/**
 Based on Alpha, Red, Green and Blue components values (0 to 255)
 The Alpha Channel  255 (opaque) to 0 (transparent).
 
 @brief    Returns 32-bit TrueColor RGBA.
 @param    source the TrueColor to alpha blend.
 @param    dest the TrueColor to alpha blend to.
 @returns  An 32-bit unsigned integer number that can be used as
           the color parameter for a drawing function.
 */
static uint32_t alphaBlend(uint32_t source, uint32_t dest) {
    // Alpha blending the source and background RGBA colors : ABGR32 (little endian)
    uint32_t alpha = (source >> 24);
    uint32_t RxBx = (((source & 0x00ff00ff) * alpha) + ((dest & 0x00ff00ff) * (0xff - alpha))) & 0xff00ff00;
    uint32_t xGxA  = ((((source >> 8) & 0x00ff00ff) * alpha) + (((dest >> 8) & 0x00ff00ff) * (0xff - alpha))) & 0xff00ff00;
    
    return (RxBx >> 8) | xGxA;
}

uint32_t convertToRGBA(uint32_t argb) {
    return argb << 24 | (argb >> 8);
}

uint32_t invertAlphaChannel(uint32_t rgba) {
    uint8_t alpha = (rgba >> 24) & 0xFF;
    uint8_t invertedAlpha = 255 - alpha;
    return (invertedAlpha << 24) | (rgba & 0x00FFFFFF);
}

/**
 @brief    Draw a perfectly horizontal line (this is often optimized in a subclass!)
 @param    x   Left-most x coordinate
 @param    y   Left-most y coordinate
 @param    w   Width in pixels
 @param    color Specifies what color the plotted pixel will be. It is in RGBA 8888  format.
 */
static void drawFastHLine(unsigned x, unsigned y, unsigned w, color_t color) {
    if (y > LCD_HEIGHT_PX - 1) return;
    while (w--) {
        plot(x++, y, color);
        if (x > LCD_WIDTH_PX - 1) return;
    }
}

/**
 @brief    Draw a perfectly vertical line (this is often optimized in a subclass!)
 @param    x   Top-most x coordinate
 @param    y   Top-most y coordinate
 @param    h   Height in pixels
 @param    color Specifies what color the plotted pixel will be. It is in RGBA 8888 format.
 */
static void drawFastVLine(unsigned x, unsigned y, unsigned h, color_t color) {
    if (x > LCD_WIDTH_PX - 1) return;
    while (h--) {
        plot(x, y++, color);
        if (y > LCD_HEIGHT_PX - 1) return;
    }
}

static void swap(int *x, int *y) {
    *x = *x ^ *y;    // x = x ^ y
    *y = *x ^ *y;    // y = (x ^ y) ^ y wich means now y = original x
    *x = *x ^ *y;    // x = (x ^ y) ^ y but since y is now original x then
    // it is really x = (x ^ y) ^ x, so x = original y
}

/**
 @brief    Returns a color in RGBA 8888 format from a given RGB[0-255] value.
 @param    r  Red channel
 @param    g  Green channel
 @param    b  Blue channel
 */
color_t rgb(unsigned char r, unsigned char g, unsigned char b) {
    return (color_t)r | (color_t)g << 8 | (color_t)b << 16;
}

/**
 @brief    Write a line.  Bresenham's algorithm
 @param    x1  Start point x coordinate
 @param    y1  Start point y coordinate
 @param    x2  End point x coordinate
 @param    y2  End point y coordinate
 @param    color Specifies what color the plotted pixel will be. It is in RGBA 8888 format.
 */
void drawLine(int x1, int y1, int x2, int y2, color_t color) {
    if (x1 == x2) {
        if (y1 > y2)
            swap(&y1, &y2);
        drawFastVLine(x1, y1, y2 - y1 + 1, color);
        return;
    }
    
    if (y1 == y2) {
        if (x1 > x2)
            swap(&x1, &x2);
        drawFastHLine(x1, y1, x2 - x1 + 1, color);
        return;
    }
    
    signed char ix;
    signed char iy;
    
    // if x1 == x2 or y1 == y2, then it does not matter what we set here
    int delta_x;
    int delta_y;
    
    if (x2 > x1) {
        ix = 1;
        delta_x = x2 - x1;
    } else {
        ix = -1;
        delta_x = x1 - x2;
    }
    
    if (y2 > y1) {
        iy = 1;
        delta_y = y2 - y1;
    } else {
        iy = -1;
        delta_y = y1 - y2;
    }
    
    plot(x1, y1, color);
    if (delta_x >= delta_y) {
        int error = delta_y - (delta_x >> 1);        // error may go below zero
        while (x1 != x2) {
            if (error >= 0) {
                if (error || (ix > 0)) {
                    y1 += iy;
                    error -= delta_x;
                }                           // else do nothing
            }                              // else do nothing
            x1 += ix;
            error += delta_y;
            plot(x1, y1, color);
        }
    } else {
        int error = delta_x - (delta_y >> 1);      // error may go below zero
        while (y1 != y2) {
            if (error >= 0) {
                if (error || (iy > 0)) {
                    x1 += ix;
                    error -= delta_y;
                }                           // else do nothing
            }                              // else do nothing
            y1 += iy;
            error += delta_x;
            plot(x1, y1, color);
        }
    }
}


void drawRect(int x, int y, short w, short h, color_t color) {
    drawFastHLine(x, y, w, color);
    drawFastHLine(x, y + h - 1, w, color);
    drawFastVLine(x, y, h, color);
    drawFastVLine(x + w - 1, y, h, color);
}


void fillRect(int x, int y, short w, short h, color_t color) {
    while (h-- > 0) {
        if (y >= LCD_HEIGHT_PX) break;
        if (y < 0) continue;
        drawFastHLine(x, y++, w, color);
    }    
}

/**
 @brief    Quarter-circle drawer, used to do circles and roundrects.
 @param    x   Center-point x coordinate.
 @param    y   Center-point y coordinate.
 @param    r   Radius of circle.
 @param    cornername  Mask bit #1 or bit #2 to indicate which quarters of the circle we're doing.
 @param    color Specifies what color to draw with. It is in RGBA 8888 format.
 */
static void drawCircleHelper(int x, int y, short r, unsigned char cornername, color_t color) {
    short f = 1 - r;
    short ddF_x = 1;
    short ddF_y = -2 * r;
    short xx = 0;
    short yy = r;
    
    while (xx < yy) {
        if (f >= 0) {
            yy--;
            ddF_y += 2;
            f += ddF_y;
        }
        xx++;
        ddF_x += 2;
        f += ddF_x;
        if (cornername & 0x4) {
            plot(x + xx, y + yy, color);
            plot(x + yy, y + xx, color);
        }
        if (cornername & 0x2) {
            plot(x + xx, y - yy, color);
            plot(x + yy, y - xx, color);
        }
        if (cornername & 0x8) {
            plot(x - yy, y + xx, color);
            plot(x - xx, y + yy, color);
        }
        if (cornername & 0x1) {
            plot(x - yy, y - xx, color);
            plot(x - xx, y - yy, color);
        }
    }
}

/**
 @brief    Draw a circle outline.
 @param    x   Center-point x coordinate.
 @param    y   Center-point y coordinate.
 @param    r   Radius of circle.
 @param    color Specifies what color to draw with. It is in RGBA 8888 format.
 */
void drawCircle(int x, int y, short r, color_t color) {
    short f = 1 - r;
    short ddF_x = 1;
    short ddF_y = -2 * r;
    short xx = 0;
    short yy = r;
    
    plot(x, y + r, color);
    plot(x, y - r, color);
    plot(x + r, y, color);
    plot(x - r, y, color);
    
    while (xx < yy) {
        if (f >= 0) {
            yy--;
            ddF_y += 2;
            f += ddF_y;
        }
        xx++;
        ddF_x += 2;
        f += ddF_x;
        
        plot(x + xx, y + yy, color);
        plot(x - xx, y + yy, color);
        plot(x + xx, y - yy, color);
        plot(x - xx, y - yy, color);
        plot(x + yy, y + xx, color);
        plot(x - yy, y + xx, color);
        plot(x + yy, y - xx, color);
        plot(x - yy, y - xx, color);
    }
}

/**
 @brief    Quarter-circle drawer with fill, used for circles and roundrects.
 @param    x Center-point x coordinate.
 @param    y Center-point y coordinate.
 @param    r  Radius of circle.
 @param    corners  Mask bits indicating which quarters we're doing.
 @param    delta    Offset from center-point, used for round-rects.
 @param    color Specifies what color to draw with. It is in RGBA 8888 format.
 */
static void fillCircleHelper(int x, int y, short r, unsigned char corners, short delta, color_t color) {
    short f = 1 - r;
    short ddF_x = 1;
    short ddF_y = -2 * r;
    short xx = 0;
    short yy = r;
    short px = xx;
    short py = yy;
    
    delta++; // Avoid some +1's in the loop
    
    while (xx < yy) {
        if (f >= 0) {
            yy--;
            ddF_y += 2;
            f += ddF_y;
        }
        xx++;
        ddF_x += 2;
        f += ddF_x;
        // These checks avoid double-drawing certain lines, important
        // for the SSD1306 library which has an INVERT drawing mode.
        if (xx < (yy + 1)) {
            if (corners & 1)
                drawFastVLine(x + xx, y - yy, 2 * yy + delta, color);
            if (corners & 2)
                drawFastVLine(x - xx, y - yy, 2 * yy + delta, color);
        }
        if (yy != py) {
            if (corners & 1)
                drawFastVLine(x + py, y - px, 2 * px + delta, color);
            if (corners & 2)
                drawFastVLine(x - py, y - px, 2 * px + delta, color);
            py = yy;
        }
        px = xx;
    }
}

/**
 @brief    Draw a circle with filled color
 @param    x   Center-point x coordinate
 @param    y   Center-point y coordinate
 @param    color Specifies what color to draw with. It is in RGBA 8888 format.
 */
void fillCircle(int x, int y, short r, color_t color) {
    drawFastVLine(x, y - r, 2 * r + 1, color);
    fillCircleHelper(x, y, r, 3, 0, color);
}

/**
 @brief    Draw a triangle with no fill color
 @param    x1  Vertex #1 x coordinate
 @param    y1  Vertex #1 y coordinate
 @param    x2  Vertex #2 x coordinate
 @param    y2  Vertex #2 y coordinate
 @param    x3  Vertex #3 x coordinate
 @param    y3  Vertex #3 y coordinate
 @param    color Specifies what color to draw with. It is in RGBA 8888 format.
 */
void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned char color) {
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x3, y3, color);
    drawLine(x3, y3, x1, y1, color);
}

/**
 @brief    Draw a triangle with color-fill
 @param    x1  Vertex #1 x coordinate
 @param    y1  Vertex #1 y coordinate
 @param    x2  Vertex #2 x coordinate
 @param    y2  Vertex #2 y coordinate
 @param    x3  Vertex #3 x coordinate
 @param    y3  Vertex #3 y coordinate
 @param    color Specifies what color to draw with. It is in RGBA 8888 format.
 */
void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, color_t color) {
    
    int a, b, y, last;
    
    // Sort coordinates by Y order (y3 >= y2 >= y1)
    if (y1 > y2) {
        swap(&y1, &y2);
        swap(&x1, &x2);
    }
    if (y2 > y3) {
        swap(&y3, &y2);
        swap(&x3, &x2);
    }
    if (y1 > y2) {
        swap(&y1, &y2);
        swap(&x1, &x2);
    }
    
    if (y1 == y3) { // Handle awkward all-on-same-line case as its own thing
        a = b = x1;
        if (x2 < a)
            a = x2;
        else if (x2 > b)
            b = x2;
        if (x3 < a)
            a = x3;
        else if (x3 > b)
            b = x3;
        drawFastHLine(a, y1, b - a + 1, color);
        return;
    }
    
    int dx01 = x2 - x1, dy01 = y2 - y1, dx02 = x3 - x1, dy02 = y3 - y1, dx12 = x3 - x2, dy12 = y3 - y2;
    int sa = 0, sb = 0;
    
    /*
     For upper part of triangle, find scanline crossings for segments
     0-1 and 0-2.  If y2=y3 (flat-bottomed triangle), the scanline y2
     is included here (and second loop will be skipped, avoiding a /0
     error there), otherwise scanline y2 is skipped here and handled
     in the second loop...which also avoids a /0 error here if y1=y2
     (flat-topped triangle).
     */
    
    if (y2 == y3)
        last = y2; // Include y2 scanline
    else
        last = y2 - 1; // Skip it
    
    for (y = y1; y <= last; y++) {
        a = x1 + sa / dy01;
        b = x1 + sb / dy02;
        sa += dx01;
        sb += dx02;
        
        if (a > b)
            swap(&a, &b);
        drawFastHLine(a, y, b - a + 1, color);
    }
    
    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y2=y3.
    sa = dx12 * (y - y2);
    sb = dx02 * (y - y1);
    for (; y <= y3; y++) {
        a = x2 + sa / dy12;
        b = x1 + sb / dy02;
        sa += dx12;
        sb += dx02;
        
        if (a > b)
            swap(&a, &b);
        drawFastHLine(a, y, b - a + 1, color);
    }
}


/**
 @brief    Creates a RGBA 8888 format color on a grey value.
 @param    shade Shade which can be between 0 (black) and 255 (white).
 */
unsigned short makeGray(int shade) {
    return shade * 0x10101;
}

/**
 @brief    Fills a rectangular area of (width,height) with upper-left corner at (x,y)
 @param    x   X-coordinate of the upper-left corner.
 @param    y   Y-coordinate of the upper-left corner.
 @param    w   Width of the filled rectangle.
 @param    h   Height of the filled rectangle.
 @param    color Color of the filled rectangle.
 */
void fillArea(unsigned x, unsigned y, unsigned w, unsigned h, color_t color) {
    if (x > LCD_WIDTH_PX - 1 || y > LCD_HEIGHT_PX - 1) return;
    color_t* DRAM = (color_t *)getDRAM();
    DRAM += y * LCD_WIDTH_PX + x;
    while(h--){
        unsigned w2 = w;
        while(w2--) {
            *DRAM++ = color;
        }
        DRAM += LCD_WIDTH_PX - w;
    }
}

/**
 @brief    Fills a rectangular area of (width,height) with upper-left corner at (x,y)
 @param    x   Specifies the x coordinate of the pixel in range of [0,319]
 @param    y   Specifies the y coordinate of the pixel in range of [0,239]
 @param    color Specifies what color the plotted pixel will be. It is in RGBA 8888 format.
 */
void plot(unsigned x, unsigned y, color_t color) {
    color_t* DRAM = NULL;
    
    if (x > LCD_WIDTH_PX - 1 || y > LCD_HEIGHT_PX - 1) return;
    DRAM = (color_t *)getDRAM();
    DRAM += x + y * LCD_WIDTH_PX;
    *DRAM = alphaBlend(color, *DRAM);
}


void drawBitmapScaled(int dx, int dy, int width, int height, float scale_x, float scale_y, const void *data) {
    uint32_t *bitmap = (uint32_t*)data;
    
    // Iterate through the bitmap and draw it scaled
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Access pixel color data from the bitmap array
            color_t color = bitmap[y * width + x];
            
            // Draw the pixel at the scaled position
            for (float sy = 0; sy < scale_y; sy += 1.0f) {
                for (float sx = 0; sx < scale_x; sx += 1.0f) {
                    // Calculate the scaled position
                    int posX = (int)(x * scale_x + sx);
                    int posY = (int)(y * scale_y + sy);
                    plot(posX + dx, posY + dy, color);
                }
            }
        }
    }
}



