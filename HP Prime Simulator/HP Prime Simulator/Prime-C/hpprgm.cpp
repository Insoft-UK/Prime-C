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

Int START()
begin
    BLOB blobA = {0x03E07C00007F7FE0,0x007F7FE003E07C00,0x7FFF00007FFF0000,0x00007FFF00007FFF};
    BLOB blobB = {0x0000FF0000FF0000,0x00FFFFFF000000FF};

//    RECT_P(G0, 0, 0, 320, 240, RGB(0, 0, 0));

    DIMGROB_P(G2, 4, 4, blobA);
//    BLIT_P(G0, 0, 0, 200, 200, G2, 0, 0, 4, 4);

    DIMGROB_P(G3, 4, 1, blobB);
//    BLIT_P(G0, 0, 0, 200, 200, G3);

for (Int i = 0; i < 20; i += 1) {
    Int l = IP(2.5 * (Float)i) + 31;
    drawLine(0, 220 + i, 320, 220 + i, rgb(l, l, l));
}
drawRoundRect(0, 220, 52, 20, 4, rgb(0,0,0));
drawRect(53, 220, 52, 20, rgb(0,0,0));
drawRect(106, 220, 52, 20, rgb(0,0,0));
drawRect(159, 220, 52, 20, rgb(0,0,0));
drawRect(212, 220, 52, 20, rgb(0,0,0));
drawRect(265, 220, 55, 20, rgb(0,0,0));

drawLine(52, 220, 52, 239, rgb(255,255,255));


    while(true) {
        SLEEP;
    }

    return 0;
end
