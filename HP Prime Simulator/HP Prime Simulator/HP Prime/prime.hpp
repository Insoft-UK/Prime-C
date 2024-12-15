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

#define __SCREEN G0
#define __SCREEN_WIDTH 320
#define __SCREEN_HEIGHT 240

#define begin       {
#define end         }

#define BEGIN       {
#define END         }

#ifndef __cplusplus
#define and         &&
#define or          ||
#define AND         &&
#define OR          ||
#endif

#define repeat      do
#define REPEAT      do
#define until(condition) while (!(condition))
#define UNTIL(condition) while (!(condition))
#define sub         void

#define DO          {
#define THEN        {
#define IF          if
#define ELSE        } else {
#define otherwise   } else {
#define WHILE       while

#define integer     int64_t
#define real        double
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

typedef std::vector<uint64_t> BLOB;
typedef std::vector<int64_t> DATA;
typedef std::vector<double> LIST;

#define SIZE(obj)   obj.size()

#include "GROB.hpp"

#include "RGB.hpp"
#include "RECT.hpp"
#include "PIXON.hpp"
#include "DIMGROB.hpp"
#include "BLIT.hpp"

#endif

#endif /* prime_hpp */
