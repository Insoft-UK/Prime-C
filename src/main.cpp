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
#include "structs.hpp"
#include "enums.hpp"
#include "alias.hpp"
#include "strings.hpp"
#include "bitwise.hpp"
#include "for_next.hpp"
#include "do_loop.hpp"
#include "calc.hpp"

#include "build.h"

#define NAME "Prime-C Compiler"

using namespace pp;

static Preprocessor preprocessor = Preprocessor();
static Enums enumerators = Enums();
static Structs structurs = Structs();
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

/*
 The `base24ToDecimal` function converts a
 base 24 string (using the characters
 `"0123456789CDFHJKMNRUVWXY"`) into its
 decimal (base 10) equivalent. It validates the
 input for empty strings and invalid characters,
 making it the inverse of the
 `decimalToBase24` function.
 */
//static long base24ToDecimal(const std::string& base24) {
//    if (base24.empty()) {
//        throw std::invalid_argument("Input string is empty.");
//    }
//
//    const std::string base24Chars = "0123456789CDFHJKMNRUVWXY";
//    long decimalValue = 0;
//
//    for (size_t i = 0; i < base24.length(); ++i) {
//        char currentChar = base24[base24.length() - 1 - i]; // Reverse position
//        size_t index = base24Chars.find(currentChar);
//
//        if (index == std::string::npos) {
//            throw std::invalid_argument("Invalid character in base 24 string.");
//        }
//
//        decimalValue += index * static_cast<long>(pow(24, i)); // Calculate value
//    }
//
//    return decimalValue;
//}

static std::string getBuildCode(void) {
    std::string str;
    int majorVersionNumber = BUILD_NUMBER / 100000;
    str = std::to_string(majorVersionNumber) + decimalToBase24(BUILD_NUMBER - majorVersionNumber * 100000);
    return str;
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
    
    re = R"(([A-Za-z]\w*(?:\[.*\])*)([*\/+\-&|^%]|(?:>>|<<))=)";
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


// MARK: - Prime-C To PPL Translater...
void capitalizePPLKeywords(std::string& str) {
    std::string result = str;
    std::regex re;
    
    // We turn any keywords that are in lowercase to uppercase
    re = R"(\b(begin|end|return|kill|if|then|else|xor|or|and|not|case|default|iferr|ifte|for|from|step|downto|to|do|while|repeat|until|break|continue|export|const|local|key)\b)";
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
    
    static std::vector<std::string> openBrace;
    static std::vector<std::string> closingBrace;
    
    Singleton *singleton = Singleton::shared();
    
    static bool multiLineComment = false;
    
    ln = regex_replace(ln, std::regex(R"(\t)"), " "); // Tab to space, future reg-ex will not require to deal with '\t', only spaces.
    
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
    
    
    if (multiLineComment) {
        multiLineComment = !regex_search(ln, std::regex(R"(\*\/ *$)"));
        ln = std::string("");
        return;
    }
    
    if (std::regex_match(ln, std::regex(R"(^\ *\/\*)"))) {
        multiLineComment = true;
        ln = std::string("");
        return;
    }
    
    if (preprocessor.disregard == true) {
        preprocessor.parse(ln);
        ln = std::string("");
        return;
    }
    
    if (preprocessor.ppl) {
        // We're presently handling PPL code.
        preprocessor.parse(ln);
        if (!preprocessor.ppl) {
            // Signaling PPL code ending with the #end preprocessor, we clear the line to discard #end, and return to the calling function.
            ln = std::string("");
            return;
        }
        ln += '\n';
        return;
    }
    
    if (preprocessor.python) {
        // We're presently handling Python code.
        preprocessor.parse(ln);
        ln += '\n';
        return;
    }
    
    // Remove any leading white spaces before or after.
    trim(ln);
    
    re = R"(\#pragma mode *\(.*\)$)";
    if (std::regex_match(ln, re)) {
        ln += '\n';
        return;
    }
    
    if (preprocessor.parse(ln)) {
        if (preprocessor.python) {
            // Indicating Python code ahead with the #PYTHON preprocessor, we maintain the line unchanged and return to the calling function.
            ln += '\n';
            return;
        }

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
    Bitwise::parse(ln);
    
    
    
    Alias::parse(ln);
    ln = singleton->aliases.resolveAllAliasesInText(ln);
    
    
    if (enumerators.parse(ln)) {
        ln = std::string("");
        return;
    }
    
    // PPL uses := instead of C's = for assignment. Converting all = to PPL style :=
    re = R"(([^:=]|^)(?:=)(?!=))";
    ln = std::regex_replace(ln, re, "$1 := ");
    
    re = R"( *\b(void|fn)\b *)";
    ln = std::regex_replace(ln, re, "");
    
    re = R"(\]\[)";
    ln = std::regex_replace(ln, re, ",");
    
    if (ln == "{") {
        singleton->setNestingLevel(singleton->nestingLevel + 1);
        
        if (singleton->nestingLevel == 1) {
            ln = "\nBEGIN\n";
            return;
        }
        
        if (!openBrace.empty()) {
            ln = openBrace.back();
            openBrace.pop_back();
            return;
        }
    }
    
    if (ln == "}") {
        
        singleton->setNestingLevel(singleton->nestingLevel - 1);
        
        if (0 == singleton->nestingLevel) {
            structurs.removeAllLocalStructs();
            ln = "END;\n";
            return;
        }
        if (singleton->nestingLevel < 0) {
            std::cout << MessageType::Error << "unexpected '}' missing '{'\n";
        }
        
        if (!closingBrace.empty()) {
            ln = closingBrace.back();
            closingBrace.pop_back();
            return;
        }
    }
    

    if (singleton->scope == Singleton::Scope::Global) {
        re = R"(^ *(KS?A?_[A-Z\d][a-z]*) *$)";
        std::sregex_token_iterator it = std::sregex_token_iterator {
            ln.begin(), ln.end(), re, {1}
        };
        if (it != std::sregex_token_iterator()) {
            std::string s = *it;
            ln = "KEY " + s + "()";
        }
        
        re = R"(^main\b)";
        ln = std::regex_replace(ln, re, "START");
        
        re = R"(var\b)";
        ln = std::regex_replace(ln, re, "");
    }

    
    
    if (singleton->scope == Singleton::Scope::Local) {
        singleton->switches.parse(ln);
        
        translateCLogicalOperatorsToPPL(ln);
        
        
//        ForNext::parse(ln);
//        DoLoop::parse(ln);
        
        re = R"(var\b)";
        ln = std::regex_replace(ln, re, "LOCAL");
        re = R"(const\b)";
        ln = std::regex_replace(ln, re, "CONST");
        
        re = R"(^(if|while|for)\((.+)\)$)";
        ln = std::regex_replace(ln, re, "$1 $2");
        
        if (ln.substr(0, 3) == "if ") {
            openBrace.push_back(std::string(" THEN\n"));
            closingBrace.push_back("END;\n");
        }
        
        if (ln.substr(0, 4) == "else") {
            openBrace.push_back(std::string("\n"));
            closingBrace.push_back("END;\n");
        }
        
        if (ln.substr(0, 6) == "while ") {
            openBrace.push_back(std::string(" DO\n"));
            closingBrace.push_back("END;\n");
        }
        
        re = R"(\bfor\b(.*);(.*);(.*))";
        if (regex_search(ln, match, re)) {
            std::string matched = match.str();
            std::sregex_token_iterator it = std::sregex_token_iterator {
                matched.begin(), matched.end(), re, {1, 2, 3}
            };
            std::string statements[3];
            if (it != std::sregex_token_iterator()) {
                if (it->matched) statements[0] = *it++;
                if (it->matched) statements[1] = *it++;
                if (it->matched) statements[2] = *it++;
            }
            trim(statements[0]);
            trim(statements[1]);
            trim(statements[2]);
            if (statements[1].empty()) statements[1] = "1";
            
            if (statements[0].empty()) {
                ln = ln.replace(match.position(), match.length(), "WHILE " + statements[1]);
            } else {
                ln = ln.replace(match.position(), match.length(), statements[0] + ";WHILE " + statements[1]);
            }
            
            openBrace.push_back(std::string(" DO\n"));
            
            if (!statements[2].empty()) {
                closingBrace.push_back(statements[2] + ";END;\n");
            }
            else {
                closingBrace.push_back("END;\n");
            }
        }
        
    }
    
    
    Calc::parse(ln);
    capitalizePPLKeywords(ln);
    
    strings.restoreStrings(ln);
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
        std::istringstream iss;
        
        if (!preprocessor.ppl && !preprocessor.python) {
            trim(utf8);
            utf8 = removeWhitespaceAroundOperators(utf8);
            re = R"( {2})";
            utf8 = std::regex_replace(utf8, re, " ");
            
            re = R"(\{(.+))";
            while (std::regex_search(utf8, re)) {
                utf8 = std::regex_replace(utf8, re, "{\n$1");
            }
            re = R"((.+)\{)";
            utf8 = std::regex_replace(utf8, re, "$1\n{");
                     
            // Make sure all `var` or `const` are on seperate lines.
            re = R"((\S+.*)\b(const|var)\b)";
            utf8 = regex_replace(utf8, re, "$1\n$2");
 
            // All } must also be on separate lines
            re = R"((\S+.*)(\}$))";
            utf8 = std::regex_replace(utf8, re, "$1\n$2");
           
            re = R"(\/\/.*$)";
            utf8 = regex_replace(utf8, re, "");
        }
        
        iss.str(utf8);
        
        while(getline(iss, str)) {
            translatePrimeCLine(str, outfile);
            ppl.append(str);
        }
        
        Singleton::shared()->incrementLineNumber();
    }
    
    ppl = removeWhitespaceAroundOperators(ppl);
    
    re = R"(END;\n(ELSE))";
    ppl = regex_replace(ppl, re, "$1");
    
    
    writeUTF16(ppl, outfile);
   
    
    infile.close();
    singleton.popPathname();
}


// MARK: - Command Line
void version(void) {
    std::cout << "Copyright (C) 2023-" << BUILD_DATE / 10000 << " Insoft. All rights reserved.\n";
    std::cout << "Insoft " << NAME << " version, " << BUILD_NUMBER / 100000 << "." << BUILD_NUMBER / 10000 % 10 << "." << BUILD_NUMBER / 1000 % 10
    << " (BUILD " << getBuildCode() << ")\n";
    std::cout << "Built on: " << CURRENT_DATE << "\n";
    std::cout << "Licence: MIT License\n\n";
    std::cout << "For more information, visit: http://www.insoft.uk\n";
}

void error(void) {
    std::cout << _basename << ": try '" << _basename << " -help' for more information\n";
    exit(0);
}

void info(void) {
    std::cout << "Copyright (c) 2023-" << BUILD_DATE / 10000 << " Insoft. All rights reserved.\n";
    int rev = BUILD_NUMBER / 1000 % 10;
    std::cout << "Insoft " << NAME << " version, " << BUILD_NUMBER / 100000 << "." << BUILD_NUMBER / 10000 % 10 << (rev ? "." + std::to_string(rev) : "")
    << " (BUILD " << getBuildCode() << "-" << decimalToBase24(BUILD_DATE) << ")\n\n";
}

void help(void) {
    int rev = BUILD_NUMBER / 1000 % 10;
    
    std::cout << "Copyright (C) 2023-" << BUILD_DATE / 10000 << " Insoft. All rights reserved.\n";
    std::cout << "Insoft " << NAME << " version, " << BUILD_NUMBER / 100000 << "." << BUILD_NUMBER / 10000 % 10 << (rev ? "." + std::to_string(rev) : "")
    << " (BUILD " << getBuildCode() << "-" << decimalToBase24(BUILD_DATE) << ")\n";
    std::cout << "\n";
    std::cout << "Usage: " << _basename << " <input-file> [-o <output-file>] [-b <flags>] [-l <pathname>]\n";
    std::cout << "\n";
    std::cout << "Options:\n";
    std::cout << "  -o <output-file>        Specify the filename for generated PPL code.\n";
    std::cout << "  -v                      Display detailed processing information.\n";
    std::cout << "\n";
    std::cout << "  Verbose Flags:\n";
    std::cout << "     e                    Enumerator\n";
    std::cout << "     p                    Preprocessor\n";
    std::cout << "     s                    Structs\n";
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
            if (args.find("e") != std::string::npos) enumerators.verbose = true;
            if (args.find("s") != std::string::npos) structurs.verbose = true;
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
    str = R"(#define __LIST_LIMIT 10000)";
    preprocessor.parse(str);
    str = R"(#define __VERSION )" + std::to_string(BUILD_NUMBER / 100000) + std::to_string(BUILD_NUMBER / 10000 % 10) + std::to_string(BUILD_NUMBER / 1000 % 10);
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
