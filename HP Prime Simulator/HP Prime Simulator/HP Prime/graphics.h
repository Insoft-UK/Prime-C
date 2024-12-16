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

#ifndef graphics_h
#define graphics_h

#include "display.h"


/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif
    
    /**
    @brief    Sets the DRAM address for wear graphics is drawn to.
    @param    address Specifies the address of the DRAM.
    */
    void setDRAM(void* address);
    
    /**
     Based on Alpha, Red, Green and Blue components values (0 to 31)
     The Alpha Channel  1 (opaque) to 0 (transparent).
     
     @brief    Returns 32-bit TrueColor RGBA.
     @param    color Specifies the color will be converted. It is in RGBA 1555  format.
     @returns  An 32-bit unsigned integer number that can be used as
               the color parameter for a drawing function.
     */
    color_t convertHighColorToTrueColor(uint16_t color);
    
    /**
    @brief    Inverts the alpha channel of a given color in RGBA 8888 format.
    @param    rgba Specifies the color in RGBA 8888 format, where the alpha channel ranges from 0 (fully transparent) to 255 (fully opaque).
    @return   A new color in RGBA 8888 format with the alpha channel inverted (i.e., 0 becomes 255 and 255 becomes 0).
    */
    uint32_t invertAlphaChannel(uint32_t rgba);
    
    /**
     @brief    Returns a color in RGBA 8888 format from a given ARGB value.
     @param    argb  ARGB
     */
    uint32_t convertToRGBA(uint32_t argb);
    
    /**
     @brief    Returns a color in RGBA 8888 format from a given RGB[0-255] value.
     @param    r  Red channel
     @param    g  Green channel
     @param    b  Blue channel
     */
    color_t rgb(unsigned char r, unsigned char g, unsigned char b);

    /**
     @brief    Write a line.  Bresenham's algorithm
     @param    x1  Start point x coordinate
     @param    y1  Start point y coordinate
     @param    x2  End point x coordinate
     @param    y2  End point y coordinate
     @param    color Specifies what color the plotted pixel will be. It is in RGBA 8888 format.
     */
    void drawLine(int x1, int y1, int x2, int y2, color_t color);

    /**
     @brief    Draw a rectangle with no fill color
     @param    x   Top left corner x coordinate
     @param    y   Top left corner y coordinate
     @param    w   Width in pixels
     @param    h   Height in pixels
     @param    color Specifies what color to draw with. It is in RGBA 8888 format.
     */
    void drawRect(int x, int y, short w, short h, color_t color);
    
    /**
     @brief    Draw a rectangle with filled color
     @param    x   Top left corner x coordinate
     @param    y   Top left corner y coordinate
     @param    w   Width in pixels
     @param    h   Height in pixels
     @param    color Specifies what color to draw with. It is in RGBA 8888 format.
     */
    void fillRect(int x, int y, short w, short h, color_t color);

    /**
     @brief    Draw a circle outline.
     @param    x   Center-point x coordinate.
     @param    y   Center-point y coordinate.
     @param    r   Radius of circle.
     @param    color Specifies what color to draw with. It is in RGBA 8888 format.
     */
    void drawCircle(int x, int y, short r, color_t color);

    
    /**
     @brief    Draw a circle with filled color
     @param    x   Center-point x coordinate
     @param    y   Center-point y coordinate
     @param    color Specifies what color to draw with. It is in RGBA 8888 format.
     */
    void fillCircle(int x, int y, short r, color_t color);

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
    void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned char color);

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
    void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, color_t color);

    /**
     @brief    Creates a RGBA 8888 format color on a grey value.
     @param    shade Shade which can be between 0 (black) and 255 (white).
     */
    unsigned short makeGray(int shade);

    /**
     @brief    Fills a rectangular area of (width,height) with upper-left corner at (x,y)
     @param    x   X-coordinate of the upper-left corner.
     @param    y   Y-coordinate of the upper-left corner.
     @param    w   Width of the filled rectangle.
     @param    h   Height of the filled rectangle.
     @param    color Color of the filled rectangle.
     */
    void fillArea(unsigned x, unsigned y, unsigned w, unsigned h, color_t color);

    /**
    @brief    Plots a single pixel at the specified (x, y) coordinates.
    @param    x   Specifies the x coordinate of the pixel.
    @param    y   Specifies the y coordinate of the pixel.
    @param    color  Specifies the color of the pixel. It is in RGBA 8888 format.
    */
    void plot(unsigned x, unsigned y, color_t color);
    
    void drawBitmapScaled(int dx, int dy, int width, int height, float scale_x, float scale_y, const void* data);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif /* graphics_h */
