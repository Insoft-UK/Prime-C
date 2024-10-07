/*
 The MIT License (MIT)
 
 Copyright (c) 2023 Insoft. All rights reserved.
 
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



#ifndef common_hpp
#define common_hpp

#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <ostream>
#include <fstream>

#include "singleton.hpp"

#define INDENT_WIDTH 2

#define basename(path)  path.substr(path.find_last_of("/") + 1)

enum class MessageType {
    Warning,
    Error,
    CriticalError,
    Verbose,
    Deprecated,
    Tip
};


namespace ANSI {
#ifdef DEBUG
    const std::string Default = "";
    const std::string Bold = "";
    const std::string Green = "";
    const std::string Red = "";
    const std::string Orange = "";
    const std::string Yellow = "";
    const std::string Purpel = "";
    const std::string Blue = "";
#else
    const std::string Default = R"(\e[0;m)";
    const std::string Bold = R"(\e[1m)";
    const std::string Green = R"(\e[38;5;34m)";
    const std::string Red = R"(\e[38;5;160m)";
    const std::string Orange = R"(\e[38;5;208m)";
    const std::string Yellow = R"(\e[38;5;220)";
    const std::string Purpel = R"(\e[38;5;125m)";
    const std::string Blue = R"(\e[48;5;38m)";
#endif
};


bool hasErrors(void);
std::ostream& operator<<(std::ostream& os, MessageType type);

std::string& ltrim(std::string& str);
std::string& rtrim(std::string& str);
std::string& trim(std::string& str);

std::string trim_copy(const std::string& str);

std::string& strip(std::string& str);
std::string strip_copy(const std::string& str);

std::ifstream::pos_type file_size(const std::string& filename);
bool file_exists(const char *filename);
bool file_exists(const std::string& filename);

int countLeadingCharacters(const std::string& str, const char character);

#endif /* common_hpp */