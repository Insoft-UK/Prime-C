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

#ifndef prime_hpp
#define prime_hpp

#ifdef __clang__

#include <vector>
#include <SDL2/SDL.h>

#define __SCREEN G0
#define __SCREEN_WIDTH 320
#define __SCREEN_HEIGHT 240

#define BEGIN    {
#define END      }

#define DO       {
#define THEN     {
#define REPEAT   do {
#define IF       if
#define FOR      for
#define WHILE(condition) while (condition)
#define UNTIL(condition) } while (!(condition))
#define LOCAL    float
#define CONST    const float
#define ELSE     } else {
#define FUNCTION float
#define SUB      void
#define RETURN   return
#define CONTINUE continue
#define BREAK    break
#define DEFAULT
#define CASE     for (;;) {
#define ENDCASE  break; }

#define INTEGER  int64_t
#define IP(n)    ((int64_t)n)

#define DELAY SDL_Delay(16)

#define NOT         !
#define AND         &&
#define OR          ||

#define BITAND(a,b) ((int64_t)a&(int64_t)b)
#define BITNOT(a)   (~(int64_t)a)
#define BITOR(a,b)  ((int64_t)a|(int64_t)b)
#define BITSL(a,n)  ((int64_t)a<<(int64_t)n)
#define BITSR(a,n)  ((int64_t)a>>(int64_t)n)
#define BITXOR(a,b) ((int64_t)a^(int64_t)b)

typedef std::vector<uint64_t> BLOB;
typedef std::vector<int64_t> DATA;
typedef std::vector<float> LIST;

#define SIZE(obj)   obj.size()

#include "GROB.hpp"

#include "RGB.hpp"
#include "RECT.hpp"
#include "PIXON.hpp"
#include "DIMGROB.hpp"
#include "BLIT.hpp"

#endif

#endif /* prime_hpp */
