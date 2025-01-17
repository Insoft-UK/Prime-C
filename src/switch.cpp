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



#include "switch.hpp"
#include "common.hpp"

#include <regex>
#include <sstream>

using namespace pp;

bool Switch::parse(std::string& str) {
    std::regex re;
    std::smatch match;
    
    Singleton *singleton = Singleton::shared();
    
    if (singleton->scope == Singleton::Scope::Global) {
        _sw = 0;
        return false;
    }
    
    /*
     eg. switch expresion
     Group  0 switch expresion
            1 expresion
     */
    re = R"(\bswitch *\((.+)\) *\{ *$)";
    if (regex_search(str, match, re)) {
        std::string s = match.str();
        
        auto it = std::sregex_token_iterator {
            s.begin(), s.end(), re, {1}
        };
        if (it != std::sregex_token_iterator()) {
            std::ostringstream oss;
            oss << std::string(Singleton::shared()->nestingLevel * INDENT_WIDTH, ' ') << "LOCAL sw" << ++_sw << " := " << *it << ";\n" << std::string((Singleton::shared()->nestingLevel - 1) * 2, ' ') << "CASE";
            str.replace(match.position(), match.str().length(), oss.str());
            oss.str("");
            oss << "sw" << _sw;
            _expressions.push_back({oss.str(), countLeadingCharacters(str, ' ')});
            _level.push_back(Singleton::shared()->nestingLevel);
            if (verbose) std::cout
                << MessageType::Verbose
                << "switch"
                << ": '" << *it << "' for expression defined\n";
                
        }
        return true;
    }
    
    if (!_expressions.size()) return false;
    TExpression exp = _expressions.back();
    
    re = R"(\bCASE *(\-?\d+) *\:)";
    if (regex_search(str, match, re)) {
        str.replace(match.position(), match.str().length(), std::string(Singleton::shared()->nestingLevel * INDENT_WIDTH, ' ') + "IF " + exp.expression + " == " + match.str(1) + " THEN");
        return true;
    }
    
    if (_level.front() == singleton->nestingLevel) {
        re = R"(\bBREAK;)";
        if (regex_search(str, match, re)) {
            str.replace(match.position(), match.str().length(),"END;");
        }
        
        re = R"(\bDEFAULT:)";
        if (regex_search(str, match, re)) {
            str.replace(match.position(), match.str().length(), std::string(Singleton::shared()->nestingLevel * INDENT_WIDTH, ' ') + "DEFAULT");
        }
        
        
    }
    
    if (_level.front() == singleton->nestingLevel) {
        re = R"(^ *\} *$)";
        if (regex_match(str, match, re)) {
            if (verbose) std::cout
                << MessageType::Verbose
                << "switch"
                << ": '" << _expressions.back().expression << "' expression removed!\n";
            _expressions.pop_back();
            _level.pop_back();
            str.replace(match.position(), match.str().length(),"END;");
            return true;
        }
    }
    
    

    
    return false;
}
