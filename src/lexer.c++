#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

//===----------------------------------------------------------------------===//
// Lexer
//===----------------------------------------------------------------------===//

// the lexer returns tokens [0-255] if it is an unknown character
// otherwise, it will return one of these tokens 
enum Token {
    tok_eof = -1,

    // commands
    tok_def = -2,
    tok_extern = -3,

    // primary 
    tok_identifier = -4,
    tok_number = -5,
};

static std::string IdentifierStr;   // if tok_identifier
static double NumVal;               // if tok_number

// return the next token from standard input
static int gettok(){
    static int LastChar = ' ';

    // skip whitespace
    while (isspace(LastChar)) {
        LastChar = getchar();
    }
        
    // identifier: [a-zA-Z][a-zA-Z0-9]*
    if (isalpha(LastChar)) {
        IdentifierStr = LastChar;
        while (isalnum((LastChar = getchar()))){
            IdentifierStr += LastChar;
        }
        
        if (IdentifierStr == "def") {
            return tok_def;
        }
            
        if (IdentifierStr == "extern") {
            return tok_extern;
        }
           
        return tok_identifier;
    }

    // number: [0-9.]+
    if (isdigit(LastChar) || LastChar == '.') {
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), 0);
        return tok_number;
    }

    if (LastChar == '#') {
        do {
            LastChar = getchar();
        } while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF) {
            return gettok();
        }
    }

    // check for end of file
    if (LastChar == EOF) {
        return tok_eof;
    } 
        
    // otherwise, return the character as its ascii value
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}