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

#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <cstring>
#include <iomanip>
#include <cmath>
#include <sys/time.h>
#include <ctime>
#include <vector>

#include "timer.hpp"
#include "singleton.hpp"
#include "common.hpp"

#include "preprocessor.hpp"
#include "strings.hpp"
#include "calc.hpp"

#include "version_code.h"

#define NAME "Prime-C Compiler"

using namespace pp;

static Preprocessor preprocessor = Preprocessor();
static Strings strings = Strings();

static std::string _basename;


void terminator() {
    std::cout << MessageType::CriticalError << "An internal pre-processing problem occurred. Please review the syntax before this point.\n";
    exit(0);
}
 
void (*old_terminate)() = std::set_terminate(terminator);


void translatePrimeCToPPL(const std::string& pathname, std::ofstream &outfile);

// MARK: - Utills



/*
 The decimalToBase24 function converts a given
 base 10 integer into its base 24 representation using a
 specific set of characters. The character set is
 comprised of the following 24 symbols:

     •    Numbers: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
     •    Letters: C, D, F, H, J, K, M, N, R, U, V, W, X, Y
     
 Character Selection:
 The choice of characters was made to avoid confusion
 with common alphanumeric representations, ensuring
 that each character is visually distinct and easily
 recognizable. This set excludes characters that closely
 resemble each other or numerical digits, promoting
 clarity in representation.
 */
static std::string decimalToBase24(long num) {
    if (num == 0) {
        return "C";
    }

    const std::string base24Chars = "0123456789CDFHJKMNRUVWXY";
    std::string base24;

    while (num > 0) {
        int remainder = num % 24;
        base24 = base24Chars[remainder] + base24; // Prepend character
        num /= 24; // Integer division
    }

    return base24;
}


uint32_t utf8_to_utf16(const char *str) {
    uint8_t *utf8 = (uint8_t *)str;
    uint16_t utf16 = *utf8;
    
    if ((utf8[0] & 0b11110000) == 0b11100000) {
        utf16 = utf8[0] & 0b11111;
        utf16 <<= 6;
        utf16 |= utf8[1] & 0b111111;
        utf16 <<= 6;
        utf16 |= utf8[2] & 0b111111;
        return utf16;
    }
    
    // 110xxxxx 10xxxxxx
    if ((utf8[0] & 0b11100000) == 0b11000000) {
        utf16 = utf8[0] & 0b11111;
        utf16 <<= 6;
        utf16 |= utf8[1] & 0b111111;
        return utf16;
    }
    
    return utf16;
}

// Function to remove whitespaces around specific operators using regular expressions
std::string removeWhitespaceAroundOperators(const std::string& str) {
    // Regular expression pattern to match spaces around the specified operators
    // Operators: {}[]()≤≥≠<>=*/+-▶.,;:!^
    std::regex re(R"(\s*([{}[\]()≤≥≠<>=*\/+\-▶.,;:!^&|%])\s*)");

    // Replace matches with the operator and no surrounding spaces
    std::string result = std::regex_replace(str, re, "$1");
    
    return result;
}

// MARK: - C To PPL Translater...

/*
 Parses and converts C-style expressions like a+=b to PPL form like a := a + b
 */
std::string expandAssignment(const std::string& expression) {
    std::string str = expression;
    std::regex re;
    
    re = R"(([A-Za-z]\w* *(?:\[.*\])*)([*\/+\-&|^%]|(?:>>|<<))=)";
    str = regex_replace(str, re, "$1:=$1$2");
    
    
    re = R"(%)";
    str = regex_replace(str, re, " MOD ");
    
    return str;
}

void translateCLogicalOperatorsToPPL(std::string& str) {
    str = regex_replace(str, std::regex(R"(&&)"), " AND ");
    str = regex_replace(str, std::regex(R"(\|\|)"), " OR ");
    str = regex_replace(str, std::regex(R"(!)"), " NOT ");
    str = regex_replace(str, std::regex(R"(\^\^)"), " XOR ");
}

void removeTemplateSyntax(std::string& str) {
    str = regex_replace(str, std::regex(R"(< *LOCAL *>)"), "");
}

void removeTypeCastingSyntax(std::string& str) {
    str = regex_replace(str, std::regex(R"(\( *LOCAL *\))"), "");
}

// MARK: - Prime-C To PPL Translater...
void reformatPPLLine(std::string& str) {
    std::regex re;
    std::smatch match;
    
    Strings strings = Strings();
    
    /*
     While formatting the contents, strings may inadvertently undergo formating,
     leading to potential disruptions in the string's content.
     
     To address this issue, we prioritize the preservation of any existing strings.
     After we prioritize the preservation of any existing strings, we blank out the
     string/s.
     
     Subsequently, after formating, any strings that have been blanked out can be
     restored to their original state.
     */
    strings.preserveStrings(str);
    strings.blankOutStrings(str);
    
    str = removeWhitespaceAroundOperators(str);
    
    str = regex_replace(str, std::regex(R"(,)"), ", ");
    str = regex_replace(str, std::regex(R"(\{)"), "{ ");
    str = regex_replace(str, std::regex(R"(\})"), " }");
    str = regex_replace(str, std::regex(R"(^ +(\} *;))"), "$1\n");
    str = regex_replace(str, std::regex(R"(\{ +\})"), "{}");
    
    /*
     To prevent correcting over-modifications, first replace all double `==` with a single `=`.
     
     Converting standalone `=` to `==` initially can lead to unintended changes like `<=`, `>=`,
     `:=`, and `==` turning into `<==`, `>==`, `:==`, and `===`.
     
     By first reverting all double `==` back to a single `=`, and ensuring that only standalone
     `=` or `:=` with surrounding whitespace are targeted, we can then safely convert `=` to `==`
     without affecting other operators.
     */
    str = regex_replace(str, std::regex(R"(==)"), "=");

    // Ensuring that standalone `≥`, `≤`, `≠`, `=`, `:=`, `+`, `-`, `*` and `/` have surrounding whitespace.
    re = R"(≥|≤|≠|=|:=|\+|-|\*|\/|▶)";
    str = regex_replace(str, re, " $0 ");
    
    // We now hand the issue of Unary Minus/Operator
    
    // Ensuring that `≥`, `≤`, `≠`, `=`, `+`, `-`, `*` and `/` have a whitespace befor `-`.
    re = R"(([≥≤≠=\+|\-|\*|\/]) +- +)";
    str = regex_replace(str, re, "$1 -");
    
    // Ensuring that `-` in  `{ - `, `( - ` and `[ - ` situations have no surrounding whitespace.
    re = R"(([({[]) +- +)";
    str = regex_replace(str, re, "$1-");
    
    if (!regex_search(str, std::regex(R"(LOCAL [A-Za-z]\w* = )"))) {
        // We can now safely convert `=` to `==` without affecting other operators.
        str = regex_replace(str, std::regex(R"( = )"), " == ");
    }
    
    str = regex_replace(str, std::regex(R"(;(END|WHILE)\b)"), "; $1");
    str = regex_replace(str, std::regex(R"(\b *(AND|OR|NOT) *\b)"), " $1 ");
    
    if (Singleton::Scope::Global == Singleton::shared()->scope) {
        str = regex_replace(str, std::regex(R"(^ *END;$)"), "$0\n");
        str = regex_replace(str, std::regex(R"(^ *LOCAL +)"), "");
    }
    
    
    
    re = R"(\b(BEGIN|IF|WHILE|REPEAT|CASE|ELSE|DEFAULT)\b)";
    if (regex_search(str, re)) {
        str.insert(0, std::string((Singleton::shared()->nestingLevel - 1) * INDENT_WIDTH, ' '));
    }
    else {
        str.insert(0, std::string(Singleton::shared()->nestingLevel * INDENT_WIDTH, ' '));
    }
    
    strings.restoreStrings(str);
}

void capitalizePPLKeywords(std::string& str) {
    std::string result = str;
    std::regex re(R"(\b(begin|end|return|kill|if|then|else|xor|or|and|not|case|default|iferr|ifte|for|from|step|downto|to|do|while|repeat|until|break|continue|export|const|local|key)\b)", std::regex_constants::icase);
    
    // We turn any keywords that are in lowercase to uppercase
    for(std::sregex_iterator it = std::sregex_iterator(str.begin(), str.end(), re); it != std::sregex_iterator(); ++it) {
        std::string result = it->str();
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        str = str.replace(it->position(), it->length(), result);
    }
}

void translatePrimeCLine(std::string& ln, std::ofstream& outfile) {
    std::regex re;
    std::smatch match;
    std::ifstream infile;
    
    static std::vector<std::string> closingScope;
    
    Singleton *singleton = Singleton::shared();
    
    
    /*
     While parsing the contents, strings may inadvertently undergo parsing, leading
     to potential disruptions in the string's content.
     
     To address this issue, we prioritize the preservation of any existing strings.
     After we prioritize the preservation of any existing strings, we blank out the
     string/s.
     
     Subsequently, after parsing, any strings that have been blanked out can be
     restored to their original state.
     */
    strings.preserveStrings(ln);
    strings.blankOutStrings(ln);
    
    ln = regex_replace(ln, std::regex(R"(\s+)"), " "); // All multiple whitespaces in succesion to a single space, future reg-ex will not require to deal with '\t', only spaces.
    
    
    // Remove any leading white spaces before or after.
    trim(ln);
    removeWhitespaceAroundOperators(ln);
    
    re = R"(\#pragma mode *\(.*\)$)";
    if (std::regex_match(ln, re)) {
        ln += '\n';
        return;
    }
    
    if (preprocessor.parse(ln)) {
        if (!preprocessor.pathname.empty()) {
            // Flagged with #include preprocessor for file inclusion, we process it before continuing.
            translatePrimeCToPPL(preprocessor.pathname, outfile);
        }
        
        ln = std::string("");
        return;
    }
    
   
    
    
    /*
     In C++, the standard library provides support for regular expressions
     through the <regex> library, but it does not support lookbehind
     assertions (such as (?<!...)) directly, as they are not part of the
     regular expressions supported by the C++ Standard Library.
     
     However, we can work around this limitation by adjusting your regular
     expression to achieve the same result using alternative techniques.
     
     This approach doesn’t fully replicate lookbehind functionality, but
     it can be effective for simpler cases where a limited lookbehind is
     required.
     */
    std::string::const_iterator it(ln.cbegin());
    re = R"((?:[^<>=]|^)(>=|!=|<>|<=|=>)(?!=[<>=]))";
    
    while (std::regex_search(it, ln.cend(), match, re)) {
        // We will convert any >= != <= or => to PPLs ≥ ≠ ≤ and ▶
        std::string s = match.str(1);
        
        // Replace the operator with the appropriate PPL symbol.
        if (s == ">=") s = "≥";
        if (s == "!=") s = "≠";
        if (s == "<>") s = "≠";
        if (s == "<=") s = "≤";
        if (s == "=>") s = "▶";
        
        ln = ln.replace(match.position(1), match.length(1), s);
        
        // Reset the iterator to the beginning
        it = ln.cbegin();
        
    }

    ln = expandAssignment(ln);
    
    capitalizePPLKeywords(ln);
    
    
    ln = singleton->aliases.resolveAllAliasesInText(ln);
    
    re = R"(0x([\dA-F]+))";
    ln = std::regex_replace(ln, re, "#$1:64h");
    
    re = R"(0b([01]+))";
    ln = std::regex_replace(ln, re, "#$1:64b");
    
    re = R"(\( *G0 *,)";
    ln = std::regex_replace(ln, re, "(");
    
    re = R"(\b(sub|function) +)";
    ln = std::regex_replace(ln, re, "");
    
    re = R"(\b(List|U?Int\d{0,2}|Float(?:32|64))\b)";
    ln = std::regex_replace(ln, re, "LOCAL");
    
    re = R"(\bLOCAL<LOCAL> ([A-Za-z]\w*)\((\d+)\))";
    ln = std::regex_replace(ln, re, "LOCAL $1:=MAKELIST(0,1,$2)");
    
    
    removeTemplateSyntax(ln);
    removeTypeCastingSyntax(ln);
    
    if (singleton->switches.parse(ln)) {
        goto exit;
    }
    
    
    
    re = R"(\bCONST +LOCAL\b)";
    ln = std::regex_replace(ln, re, "CONST");
    
    re = R"(\btrue\b)";
    ln = std::regex_replace(ln, re, "1");
    
    re = R"(\bfalse\b)";
    ln = std::regex_replace(ln, re, "0");
    
    re = R"(\bSLEEP *;)";
    ln = std::regex_replace(ln, re, "");
    
    
    re = R"(\[([^\[\]]+)\])";
    ln = regex_replace(ln, std::regex(R"(\[([^\[\]]+)\])"), "[($1)+1]");
    
    re = R"(\[(\((\d+)\) *\+ *1)\])";
    while (std::regex_search(ln, match, re)) {
        int digit = atoi(match[2].str().c_str());
        ln = ln.replace(match.position(), match.length(), "[" + std::to_string(++digit) + "]");
    }
    
    re = R"(\]\[)";
    ln = std::regex_replace(ln, re, ",");
    
    re = R"((LOCAL [A-Za-z]\w*)\[.*\]( *= *.*))";
    ln = std::regex_replace(ln, re, "$1$2");
    
    re = R"(^ *\} *ELSE *\{ *$)";
    ln = regex_replace(ln, re, "ELSE");
    
    
    
    // Scope
    
    re = R"(^\{ *$)";
    ln = regex_replace(ln, re, "BEGIN");
    
    re = R"(^\} *$)";
    ln = regex_replace(ln, re, "END");
    
    re = R"((?:(?:\)|REPEAT|CASE) *\{|^ *BEGIN) *$)";
    if (std::regex_search(ln, re)) {
        singleton->setNestingLevel(singleton->nestingLevel + 1);
    }
    
    re = R"(^ *(?:\}|END|\} *UNTIL\(.+\);) *$)";
    if (std::regex_search(ln, re)) {
        if (std::regex_search(ln, match, std::regex(R"(^ *\} *UNTIL\((.+)\); *$)"))) {
            std::string statement;
            statement = trim_copy(match[1].str());
            ln = ln.replace(match.position(), match.length(), "UNTIL " + statement + ";");
        }
        
        if (closingScope.empty()) {
            ln = std::regex_replace(ln, re, "END;");
        } else {
            ln = std::regex_replace(ln, re, closingScope.back() + "END;");
            closingScope.pop_back();
        }
        
        singleton->setNestingLevel(singleton->nestingLevel - 1);
    }
    
    
    // PPL uses := instead of C's = for assignment. Converting all = to PPL style :=
    re = R"(([^:=]|^)(?:=)(?!=))";
    ln = std::regex_replace(ln, re, "$1 := ");
    
    
    if (singleton->scope == Singleton::Scope::Global) {
        re = R"(^ *(KS?A?_[A-Z\d][a-z]*) *$)";
        std::sregex_token_iterator it = std::sregex_token_iterator {
            ln.begin(), ln.end(), re, {1}
        };
        if (it != std::sregex_token_iterator()) {
            std::string s = *it;
            ln = "KEY " + s + "()";
        }
        
        re = R"(\b(export|LOCAL)\b +)";
        ln = std::regex_replace(ln, re, "");
        
        re = R"(^main\b)";
        ln = std::regex_replace(ln, re, "START");
    }
    
    if (singleton->scope == Singleton::Scope::Local) {
//        singleton->switches.parse(ln);
        
        translateCLogicalOperatorsToPPL(ln);
        
        re = R"(\bFOR\b *\((.*);(.*);(.*)\) *\{)";
        if (std::regex_search(ln, match, re)) {
            std::string init, condition, increment, ppl;
            
            init = trim_copy(match[1].str());
            condition = trim_copy(match[2].str());
            increment = trim_copy(match[3].str());
            
            if (!init.empty()) {
                ppl.append(init + ";");
            }
            ppl.append("WHILE " + (condition.empty() ? "1" : condition) + " DO");
            
            
            
            if (!increment.empty()) {
                closingScope.push_back(increment + ";");
            } else {
                closingScope.push_back("END;\n");
            }
            
            ln = ln.replace(match.position(), match.length(), ppl);
        }
        
        re = R"(\bIF\b *\((.*)\) *\{)";
        if (std::regex_search(ln, match, re)) {
            std::string statement, ppl;
            statement = trim_copy(match[1].str());
            closingScope.push_back("");
            ppl.append("IF " + statement + " THEN");
            ln = ln.replace(match.position(), match.length(), ppl);
        }
        
        re = R"(\bWHILE\b *\((.*)\) *\{)";
        if (std::regex_search(ln, match, re)) {
            std::string statement, ppl;
            statement = trim_copy(match[1].str());
            closingScope.push_back("");
            ppl.append("WHILE " + statement + " DO");
            ln = ln.replace(match.position(), match.length(), ppl);
        }
        
        re = R"(\bREPEAT\b *\{)";
        if (std::regex_search(ln, match, re)) {
            closingScope.push_back("");
            ln = ln.replace(match.position(), match.length(), "REPEAT");
        }

    }

    ln = regex_replace(ln, std::regex(R"( *:= *)"), " := ");
    
    re = R"(\b(?:LOCAL|CONST) +[A-Za-z]\w* *:= *([\d \+\-\*\/\(\)]*);)";
    if (std::regex_search(ln, match, re)) {
        std::string ppl = "[" + match[1].str() + "]";
        if (Calc::parse(ppl)) {
            ln = ln.replace(match.position(1), match.length(1), ppl);
        }
    }
    
    
    re = R"(\b([A-Za-z]\w*)\.push_back\((.*)\))";
    ln = regex_replace(ln, re, "CONCAT($1,$2)▶$1");
    
    re = R"(\b([A-Za-z]\w*)\.front\(\))";
    ln = regex_replace(ln, re, "$1(1)");
    
    re = R"(\b([A-Za-z]\w*)\.back\(\))";
    ln = regex_replace(ln, re, "$1(length($1))");
    
    re = R"(\b([A-Za-z]\w*)\.length\(\))";
    ln = regex_replace(ln, re, "length($1)");
    
    re = R"(\b([A-Za-z]\w*)\.at\((\d+)\))";
    ln = regex_replace(ln, re, "$1($2)");
    
    exit:
    strings.restoreStrings(ln);
    reformatPPLLine(ln);
    
    ln.append("\n");
}

void writeUTF16Line(const std::string& str, std::ofstream& outfile) {
    for ( int n = 0; n < str.length(); n++) {
        uint8_t *ascii = (uint8_t *)&str.at(n);
        if (str.at(n) == '\r') continue;
        
        // Output as UTF-16LE
        if (*ascii >= 0x80) {
            uint16_t utf16 = utf8_to_utf16(&str.at(n));
            
#ifndef __LITTLE_ENDIAN__
            utf16 = utf16 >> 8 | utf16 << 8;
#endif
            outfile.write((const char *)&utf16, 2);
            if ((*ascii & 0b11100000) == 0b11000000) n++;
            if ((*ascii & 0b11110000) == 0b11100000) n+=2;
            if ((*ascii & 0b11111000) == 0b11110000) n+=3;
        } else {
            outfile.put(str.at(n));
            outfile.put('\0');
        }
    }
}

void writeUTF16(const std::string& str, std::ofstream& outfile) {
    std::istringstream stream(str);
    std::string line;
    
    while (std::getline(stream, line)) {
        line.append("\n");
        writeUTF16Line(line, outfile);
    }
}

bool isMultilineComment(const std::string str) {
    std::regex re(R"(^ *\/*)");
    return std::regex_search(str, re);
}

bool isPythonBlock(const std::string str) {
    std::regex re(R"(^ *# *PYTHON *(\/\/.*)?$)");
    return std::regex_search(str, re);
}

bool isPPLBlock(const std::string str) {
    std::regex re(R"(^ *# *PPL *(\/\/.*)?$)");
    return std::regex_search(str, re);
}

void writePPLBlock(std::ifstream& infile, std::ofstream& outfile) {
    std::regex re(R"(^ *# *(END) *(?:\/\/.*)?$)");
    std::string str;
    
    Singleton::shared()->incrementLineNumber();
    
    while(getline(infile, str)) {
        if (std::regex_search(str, re)) {
            Singleton::shared()->incrementLineNumber();
            return;
        }
        
        str.append("\n");
        writeUTF16Line(str, outfile);
        Singleton::shared()->incrementLineNumber();
    }
}

void writePythonBlock(std::ifstream& infile, std::ofstream& outfile) {
    std::regex re(R"(^ *# *(END) *(?:\/\/.*)?$)");
    std::string str;
    
    writeUTF16Line("#PYTHON\n", outfile);
    Singleton::shared()->incrementLineNumber();
    
    while(getline(infile, str)) {
        if (std::regex_search(str, re)) {
            writeUTF16Line("#END\n", outfile);
            Singleton::shared()->incrementLineNumber();
            return;
        }
        
        str.append("\n");
        writeUTF16Line(str, outfile);
        Singleton::shared()->incrementLineNumber();
    }
}

bool isBlockCommentStart(const std::string str) {
    std::regex re(R"(^ *\/\* *)");
    return std::regex_search(str, re);
}

void convertToLineComment(std::string& str) {
    std::regex re(R"(^ *\/\* *)");
    
    str = std::regex_replace(str, re, "//");
    str.append("\n");
}

void writeBlockAsLineComments(std::ifstream& infile, std::ofstream& outfile) {
    std::regex re;
    std::string str;
    
    Singleton::shared()->incrementLineNumber();
    
    re = R"( *\*\/(.*)$)";
    
    while(getline(infile, str)) {
        if (std::regex_search(str, re)) {
            str = std::regex_replace(str, re, "//$1");
            str.append("\n");
            writeUTF16Line(str, outfile);
            Singleton::shared()->incrementLineNumber();
            break;
        }
        str.insert(0, "// ");
        str.append("\n");
        writeUTF16Line(str, outfile);
        Singleton::shared()->incrementLineNumber();
    }
}

void translatePrimeCToPPL(const std::string& pathname, std::ofstream& outfile)
{
    Singleton& singleton = *Singleton::shared();
    std::ifstream infile;
    std::regex re;
    std::string utf8;
    std::string str;
    std::string ppl;

    singleton.pushPathname(pathname);
    
    infile.open(pathname,std::ios::in);
    if (!infile.is_open()) exit(2);
    
    while(getline(infile, utf8)) {
        if (isPythonBlock(utf8)) {
            writePythonBlock(infile, outfile);
            continue;
        }
        
        if (isPPLBlock(utf8)) {
            writePPLBlock(infile, outfile);
            continue;
        }
        
        // Convert any `/* comment */` to `// comment`
        re = R"(\/\*(.*)(?:(\*\/)))";
        utf8 = regex_replace(utf8, re, "//$1\n");
        
        if (isBlockCommentStart(utf8)) {
            convertToLineComment(utf8);
            writeUTF16Line(utf8, outfile);
            writeBlockAsLineComments(infile, outfile);
            continue;
        }
        
        re = R"(\/\/.*$)";
        utf8 = regex_replace(utf8, re, "");
        
        
        std::istringstream iss;
        iss.str(utf8);
        
        while(getline(iss, str)) {
            translatePrimeCLine(str, outfile);
            writeUTF16(str, outfile);
        }
        
        Singleton::shared()->incrementLineNumber();
    }
   
    infile.close();
    singleton.popPathname();
}


// MARK: - Command Line
void version(void) {
    std::cout << "Copyright (C) 2023-" << YEAR << " Insoft. All rights reserved.\n";
    std::cout << "Insoft " << NAME << " version, " << VERSION_NUMBER << " (BUILD " << VERSION_CODE << ")\n";
    std::cout << "Built on: " << DATE << "\n";
    std::cout << "Licence: MIT License\n\n";
    std::cout << "For more information, visit: http://www.insoft.uk\n";
}

void error(void) {
    std::cout << _basename << ": try '" << _basename << " -help' for more information\n";
    exit(0);
}

void info(void) {
    std::cout << "Copyright (c) 2023-" << YEAR << " Insoft. All rights reserved.\n";
    std::cout << "Insoft " << NAME << " version, " << VERSION_NUMBER << " (BUILD " << VERSION_CODE << ")\n\n";
}

void help(void) {
    std::cout << "Copyright (C) 2023-" << YEAR << " Insoft. All rights reserved.\n";
    std::cout << "Insoft " << NAME << " version, " << VERSION_NUMBER << " (BUILD " << VERSION_CODE << ")\n";
    std::cout << "\n";
    std::cout << "Usage: " << _basename << " <input-file> [-o <output-file>] [-b <flags>] [-l <pathname>]\n";
    std::cout << "\n";
    std::cout << "Options:\n";
    std::cout << "  -o <output-file>        Specify the filename for generated PPL code.\n";
    std::cout << "  -v                      Display detailed processing information.\n";
    std::cout << "\n";
    std::cout << "  Verbose Flags:\n";
    std::cout << "     a                    Aliases\n";
    std::cout << "     e                    Enumerator\n";
    std::cout << "     p                    Preprocessor\n";
    std::cout << "\n";
    std::cout << "Additional Commands:\n";
    std::cout << "  ansiart {-version | -help}\n";
    std::cout << "    -version              Display the version information.\n";
    std::cout << "    -help                 Show this help message.\n";
}


// MARK: - Main
int main(int argc, char **argv) {
    std::string in_filename, out_filename;

    if (argc == 1) {
        error();
        exit(100);
    }
    
    std::string args(argv[0]);
    _basename = basename(args);
    
    for (int n = 1; n < argc; n++) {
        args = argv[n];
        
        if (args == "-o") {
            if ( n + 1 >= argc ) {
                error();
                exit(101);
            }
            out_filename = argv[n + 1];
            if (out_filename.substr(out_filename.length() - 7).compare(".hpprgm") != 0) {
                out_filename += ".hpprgm";
            }

            n++;
            continue;
        }
        
        if ( args == "-help" ) {
            help();
            return 0;
        }

        
        if ( strcmp( argv[n], "-version" ) == 0 ) {
            version();
            return 0;
        }
        
        
        if ( args == "-v" ) {
            if ( n + 1 >= argc ) {
                error();
                exit(103);
            }
            args = argv[++n];
            
            if (args.find("a") != std::string::npos) Singleton::shared()->aliases.verbose = true;
            if (args.find("p") != std::string::npos) preprocessor.verbose = true;
        
            continue;
        }
        
        if (args == "-l") {
            if (++n >= argc) {
                error();
                return 0;
            }
            preprocessor.path = std::string(argv[n]);
            if (preprocessor.path.at(preprocessor.path.length() - 1) != '/') preprocessor.path.append("/");
            continue;
        }
        
        in_filename = argv[n];
        std::regex re(R"(.\w*$)");
        std::smatch extension;
        if (regex_search(in_filename, extension, re)) {
            if (".ppl" == extension.str()) preprocessor.ppl = true;
        }
    }
    
    if (!out_filename.length()) {
        out_filename = in_filename;
        if (out_filename.rfind(".")) {
            out_filename.replace(out_filename.rfind("."), out_filename.length() - out_filename.rfind("."), ".hpprgm");
        }
    }
    
    info();
    
    
    std::ofstream outfile;
    outfile.open(out_filename, std::ios::out | std::ios::binary);
    if(!outfile.is_open())
    {
        error();
        return 0;
    }
    
    // The "hpprgm" file format requires UTF-16LE.
    outfile.put(0xFF);
    outfile.put(0xFE);
    
    // Start measuring time
    Timer timer;
    
    std::string str;
    str = "#define __primec";
    preprocessor.parse(str);
    str = "#define __SCREEN G0";
    preprocessor.parse(str);
    str = "#define __SCREEN_WIDTH 320";
    preprocessor.parse(str);
    str = "#define __SCREEN_HEIGHT 240";
    preprocessor.parse(str);
    str = R"(#define __LIST_LIMIT 10000)";
    preprocessor.parse(str);
    
    
    translatePrimeCToPPL(in_filename, outfile);
    
    // Stop measuring time and calculate the elapsed time.
    long long elapsed_time = timer.elapsed();

    
    outfile.close();
    
    if (hasErrors() == true) {
        std::cout << "\e[48;5;160mERRORS\e[0m!\n";
        remove(out_filename.c_str());
        return 0;
    }

    // Display elasps time in secononds.
    std::cout << "Compiled in " << std::fixed << std::setprecision(2) << elapsed_time / 1e9 << " seconds\n";
    std::cout << "UTF-16LE File '" << out_filename << "' Succefuly Created.\n";
    
    
    
    
    
    return 0;
}
