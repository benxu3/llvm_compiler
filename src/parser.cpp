#include "lexer.hpp"
#include "ExprAST.hpp"
#include "PrototypeAST.hpp"
#include "NumberExprAST.hpp"
#include "VariableExprAST.hpp"
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// Curtok and getNextToken provide a token buffer
// CurTok is the current token the parser is consuming
// getNextToken reads another token from the lexer and updates CurTok
static int CurTok;
lexer lex;

static int getNextToken() {
    return CurTok = lex.gettok();
}

// LogError ; helper functions for error handling
std::unique_ptr<ExprAST> LogError(const char *Str) {
    fprintf(stderr, "LogError: %s\n", Str);
    return nullptr;
}

std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
    LogError(Str);
    return nullptr;
}

// numberexpr ::= number
static std::unique_ptr<ExprAST> ParseNumberExpr() {
    auto Result = std::make_unique<NumberExprAST>(NumVal);
    getNextToken(); //consumes the number
    return std::move(Result);
}

// parenexpr ::= '(' expression ')'
static std::unique_ptr<ExprAST> ParseParentExpr() {
    getNextToken();     // consumes '('
    auto V = ParseExpression();
    if (!V)
        return nullptr;

    if (CurTok != ')')
        return LogError("expected ')'");
    getNextToken();     // consumes ')'
    return V;
}

// identifierexpr   
//  ::= identifier
//  ::= identifier '(' expression* ')'
static std::unique_ptr<ExprAST> ParseIdentifierExpr() {
    std::string IdName = IdentifierStr;

    getNextToken(); // consume identifier

    if (CurTok != '(')
        return std::make_unique<VariableExprAST>(IdName);

    getNextToken(); // consume '(' 
    if (CurTok != ')'){
        while (1) {
            if (auto Arg = ParseExpression())
                Args.push_back(std::move(Arg));
            else
                return nullptr;
            
            if (CurTok == ')')
                break;
            
            if (CurTok != ',')
                return LogError("Expected ')' or ',' in argument list");
            getNextToken();
        }
    }

    getNextToken(); // consume '('
    return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

// primary
//  ::= identifierexpr
//  ::= numberexpr
//  ::= parenexpr
static std::unique_ptr<ExprAST> ParsePrimary() {
    switch (CurTok) {
    default:
        return LogError("unknown token when expecting an expression");
    case tok_identifier:
        return ParseIdentifierExpr();
    case tok_number:
        return ParseNumberExpr();
    case '(':
        return ParseParentExpr();
    }
}