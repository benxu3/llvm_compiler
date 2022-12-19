#include <string>
#include "lexer.hpp"

//===----------------------------------------------------------------------===//
// Lexer
//===----------------------------------------------------------------------===//

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