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

real START()
begin
    
    BLOB l = {0x7C0003E0001F7FE0,0x007F7FE003E07C00,0x7FFF00007FFF0000,0x00007FFF00007FFF};

   
    
    
    DIMGROB_P(G2, 4, 4, l);
    
    RECT_P(G0, 0, 0, 320, 240, RGB(0, 0, 0, 0));
//PIXON_P(G0, 0, 0, RGB(0, 255, 0, 0));

//    BLOB blob = {0,0,0,0};
//    INTEGER n, c;
//FOR (n = 0; n < SIZE(blob); n+=1) DO
//INTEGER r = 0, g = 0, b = 0;
//    b = BITAND(n, 1) * 255;
//    g = BITAND(BITSR(n,1), 1) * 255;
//    r = BITAND(BITSR(n,2), 1) * 255;
//    c = RGB(r, g, b);
//    c = BITOR(BITSL(c,32), c);
//    blob[n] = c;
//END
//    DIMGROB_P(G3, 2, 2, blob);
//
//    DIMGROB_P(G1, 20, 40, RGB(255, 128, 0));
//    BLIT_P(G0, 0, 0, 128, 128, G2, 0, 0, 4, 4);
//
//
//RECT_P(G0, 0, 0, 120, 240, RGB(255, 0, 0, 0));
//PIXON_P(G0, 10, 10, RGB(0, 0, 0, 0));

    LOCAL A = 0;
    IF (A == 0) THEN
    ELSE
    END

    LOCAL B = 0;
    if (B == 0) {
        otherwise
    }

const integer C = A * B;

repeat {
    BLIT_P(G0, 0, 0, 100, 240, G2, 0, 0, 4, 1);
    
    SLEEP;
} until (false);

    return 0;
end
