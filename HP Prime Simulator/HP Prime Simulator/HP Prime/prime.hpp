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
#include <chrono>
#include <thread>

using namespace std;

#define __SCREEN G0
#define __SCREEN_WIDTH 320
#define __SCREEN_HEIGHT 240
#define __LIST_LIMIT 10000

#define BEGIN       {
#define END         }

#define AND         &&
#define OR          ||

#define REPEAT      do {
#define until(condition) while (!(condition))
#define UNTIL(condition) while (!(condition))
#define sub         void

#define DO          {
#define THEN        {
#define IF          if
#define ELSE        } else {
#define WHILE       while


#define IP(n)       ((int64_t)n)
#define LOCAL       int64_t
#define CONST       const



#define SLEEP std::this_thread::sleep_for(std::chrono::seconds(1))


#define BITAND(a,b) ((int64_t)a&(int64_t)b)
#define BITNOT(a)   (~(int64_t)a)
#define BITOR(a,b)  ((int64_t)a|(int64_t)b)
#define BITSL(a,n)  ((int64_t)a<<(int64_t)n)
#define BITSR(a,n)  ((int64_t)a>>(int64_t)n)
#define BITXOR(a,b) ((int64_t)a^(int64_t)b)


#define List        std::vector

typedef int Int;
typedef int Int32;
typedef long Int64;

typedef float Float;
typedef float Float32;
typedef double Float64;

#ifndef TYPE
#define TYPE Float64
#endif

extern TYPE A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z;

extern List<TYPE> L1;
extern List<TYPE> L2;
extern List<TYPE> L3;
extern List<TYPE> L4;
extern List<TYPE> L5;
extern List<TYPE> L6;
extern List<TYPE> L7;
extern List<TYPE> L8;
extern List<TYPE> L9;

#include "GROB.hpp"

#include "RGB.hpp"
#include "RECT.hpp"
#include "PIXON.hpp"
#include "DIMGROB.hpp"
#include "BLIT.hpp"
#include "SIZE.hpp"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif
    
    void setMouseState(int state);
    
/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif

#endif /* prime_hpp */
