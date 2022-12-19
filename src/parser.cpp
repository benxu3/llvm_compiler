#include "lexer.hpp"
#include "parser.hpp"
#include "ExprAST.hpp"
#include "PrototypeAST.hpp"
#include "NumberExprAST.hpp"
#include "VariableExprAST.hpp"
#include "BinaryExprAST.hpp"
#include "FunctionAST.hpp"
#include "CallExprAST.hpp"
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//

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
    std::vector<std::unique_ptr<ExprAST>> Args;

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

// get token precedence; gets the precedence of the pending binary operator token
static int GetTokPrecedence() {
    if (!isascii(CurTok))
        return -1;
    
    int TokPrec = BinopPrecedence[CurTok];
    if (TokPrec <= 0)
        return -1;
    return TokPrec;
}

// expression ::= primary binorphs
static std::unique_ptr<ExprAST> ParseExpression() {
    auto LHS = ParsePrimary();
    if (!LHS)
        return nullptr;

    return ParseBinOpRHS(0, std::move(LHS));
}

// binorphs ::= ('+' primary)*
static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) {

    // if given a binop, find its precedence
    while (1) {
        int TokPrec = GetTokPrecedence();

        // if the binop binds at least as tightly as the current binop,
        // consume, otherwise exit
        if (TokPrec < ExprPrec)
            return LHS;

        int BinOp = CurTok;
        getNextToken(); // consume binop

        // parse primary expression after binop
        auto RHS = ParsePrimary();
        if (!RHS)
            return nullptr;
        
        // if binop binds less tightly with RHS than the operator after RHS,
        // let the pending operator take RHS as its LHS
        int NextPrec = GetTokPrecedence();
        if (TokPrec < NextPrec) {
            RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
            if (!RHS) 
                return nullptr;
        }

        // merge LHS/RHS
        LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
    }
}

// prototype ::= id '(' id* ')'
static std::unique_ptr<PrototypeAST> ParsePrototype(){
    if (CurTok != tok_identifier)
        return LogErrorP("Expected function name in prototype");
    
    std::string FnName = IdentifierStr;
    getNextToken();

    if (CurTok != '(')
        return LogErrorP("Expected '(' in prototype");

    // read list of argument names
    std::vector<std::string> ArgNames;
    while (getNextToken() == tok_identifier)
        ArgNames.push_back(IdentifierStr);
    if (CurTok != ')')
        return LogErrorP("Expected ')' in prototype");
    
    getNextToken(); // consume ')'
    return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

// definition ::= 'def' prototype expression
static std::unique_ptr<FunctionAST> ParseDefinition() {
    getNextToken(); // consume def
    auto Proto = ParsePrototype();
    if (!Proto)
        return nullptr;
    
    if (auto E = ParseExpression())
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    return nullptr;
}

// external ::= 'extern' prototype
static std::unique_ptr<PrototypeAST> ParseExtern() {
    getNextToken; // consume extern
    return ParsePrototype();
}

// toplevelexpr ::= expression
static std::unique_ptr<FunctionAST> ParseTopLevelExpr(){
    if (auto E = ParseExpression()){
        // make an "anonymous" proto
        auto Proto = std::make_unique<PrototypeAST>("", std::vector<std::string>());
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}

//===----------------------------------------------------------------------===//
// Top-Level parsing
//===----------------------------------------------------------------------===//

static void HandleDefinition() {
  if (ParseDefinition()) {
    fprintf(stderr, "Parsed a function definition.\n");
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

static void HandleExtern() {
  if (ParseExtern()) {
    fprintf(stderr, "Parsed an extern\n");
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

static void HandleTopLevelExpression() {
  // Evaluate a top-level expression into an anonymous function.
  if (ParseTopLevelExpr()) {
    fprintf(stderr, "Parsed a top-level expr\n");
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

/// top ::= definition | external | expression | ';'
static void MainLoop() {
  while (true) {
    fprintf(stderr, "ready> ");
    switch (CurTok) {
    case tok_eof:
      return;
    case ';': // ignore top-level semicolons.
      getNextToken();
      break;
    case tok_def:
      HandleDefinition();
      break;
    case tok_extern:
      HandleExtern();
      break;
    default:
      HandleTopLevelExpression();
      break;
    }
  }
}
