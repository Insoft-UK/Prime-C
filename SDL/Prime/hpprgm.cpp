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

#ifdef __clang__
#include "prime.hpp"
#endif

void START(void) {
    
 
    
    UList64 l = {0x00FFFFFF00FFFFFF,0x00FFFFFF00FFFFFF,0x00FFFFFF00FFFFFF,0x00FFFFFF00FFFFFF};
    int x = 10;
    
    DIMGROB_P(G2, 2, 2, l);
    
    RECT_P(G0, 0, 0, 319, 240, RGB(0, 0, 63, 0));
    PIXON_P(G0, x, 10, RGB(255,0,0));
    PIXON_P(G0, x, 11, RGB(0,255,0));
    PIXON_P(G0, 20, 100, RGB(0,0,255));
    
    DIMGROB_P(G1, 20, 40, RGB(255, 128, 0));
    
    
    int i;
    for (i = 0; i < __SCREEN_WIDTH * __SCREEN_HEIGHT; i++) {
//        PIXON_P(G0, i % __SCREEN_WIDTH, i / __SCREEN_WIDTH, i);
    }
    
    while (1) {
        
    }

    return;
}
