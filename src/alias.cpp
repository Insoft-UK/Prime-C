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


#include "alias.hpp"
#include "common.hpp"

#include <sstream>
#include <regex>

using namespace pp;

static Singleton *singleton = Singleton::shared();

static std::string base10ToBase32(unsigned int num) {
    if (num == 0) {
        return "0";  // Edge case: if the number is 0, return "0"
    }

    std::string result;
    const char digits[] = "0123456789ABCDEFGHIJKLMNabcdefgh";  // Base-32 digits
    
    // Keep dividing the number by 32 and store the remainders
    while (num > 0) {
        int remainder = num % 32;  // Get the current base-32 digit
        result += digits[remainder];  // Add the corresponding character
        num /= 32;  // Reduce the number
    }

    // The digits are accumulated in reverse order, so reverse the result string
    reverse(result.begin(), result.end());

    return result;
}

static void parseAlias(const std::string& str, Aliases::TIdentity& identity) {
    static int v = -1;
    static int f = -1;
    
    identity.identifier = str;
    if (identity.type == Aliases::Type::Variable) {
        identity.real = "v" + base10ToBase32(++v);
    }
    if (identity.type == Aliases::Type::Function) {
        identity.real = "f" + base10ToBase32(++f);
    }
    singleton->aliases.append(identity);
    
}

static void parseAliases(const std::string& str, Aliases::TIdentity& identity) {
    std::regex re(R"(([A-Za-z]\w*)(?:=[^=]*)?(?:,|;))");
    
    for(std::sregex_iterator it = std::sregex_iterator(str.begin(), str.end(), re); it != std::sregex_iterator(); ++it) {
        std::smatch match = *it;
        parseAlias(match[1].str(), identity);
    }
}

static void parseFunctionName(const std::string& str) {
    Aliases::TIdentity identity;
    identity.scope = Aliases::Scope::Global;
    identity.type = Aliases::Type::Function;
    parseAlias(str, identity);
}

static void parseParameters(const std::string& str) {
    Aliases::TIdentity identity;
    identity.scope = Aliases::Scope::Local;
    identity.type = Aliases::Type::Unknown;
    parseAliases(str, identity);
}

static void parseVariables(const std::string& str) {
    Aliases::TIdentity identity;
    identity.scope = Aliases::Scope::Auto;
    identity.type = Aliases::Type::Variable;
    parseAliases(str, identity);
}

bool Alias::parse(std::string& str) {
    std::string s;
    std::regex re;
    std::smatch match;
    std::ostringstream os;
    
    bool parsed = false;
    
    if (singleton->scope == Singleton::Scope::Global) {
        re = R"(^ *(?:export +)?(?:var +)?([A-Za-z]\w*)\(([A-Za-z]\w*(?:,[A-Za-z]\w*)*)?(?=\)))";
        auto it = std::sregex_token_iterator {
            str.begin(), str.end(), re, {1, 2}
        };
        if (it != std::sregex_token_iterator()) {
            parseFunctionName(*it++);
            parseParameters(*it++);
            parsed = true;
        }
    }
    
    re = R"(^ *(var|const) +.*)";
    if (regex_match(str, re)) {
        parseVariables(str);
        str = regex_replace(str, std::regex(R"(\bvar\b)"), "LOCAL");
        str = regex_replace(str, std::regex(R"(\bconst\b)"), "CONST");
        parsed = true;
    }
    
    if (!parsed) return false;
    
    return true;
}
