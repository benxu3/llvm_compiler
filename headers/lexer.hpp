#ifndef LEXER_H
#define LEXER_H

static std::string IdentifierStr;   // if tok_identifier
static double NumVal;               // if tok_numbe

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

class lexer {
    public: 
        int gettok();
};

#endif