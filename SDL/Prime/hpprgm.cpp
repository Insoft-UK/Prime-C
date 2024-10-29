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


#include "prime.hpp"

SUB START()
BEGIN
    
    BLOB l = {0x7C0003E0001F7FE0,0x007F7FE003E07C00,0x7FFF00007FFF0000,0x00007FFF00007FFF};
  
   
    
    
    DIMGROB_P(G2, 4, 4, l);
    
    RECT_P(G0, 0, 0, 319, 240, RGB(0, 0, 63, 0));
    
    BLOB b = {0,0,0,0};
    LOCAL n;
    FOR (n = 0; n < b.SIZE; n+=1) DO
        BITSHL(RGB,32);
        b.AT(n) = 0;
    END
    DIMGROB_P(G3, 8, 1, b);

    DIMGROB_P(G1, 20, 40, RGB(255, 128, 0));
    BLIT_P(G0, 0, 0, 128, 128, G2, 0, 0, 4, 4);
    BLIT_P(G0, 0, 0, 32, 16, G3, 0, 0, 4, 1);
    
    LOCAL i;
    FOR (i = 0; i < __SCREEN_WIDTH * __SCREEN_HEIGHT; i++) DO
//        PIXON_P(G0, i % __SCREEN_WIDTH, i / __SCREEN_WIDTH, i);
    END
    

    
    
    WHILE (1) DO
        DELAY;
    END

    RETURN;
END
