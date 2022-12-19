#include <string>
#include <vector>
#include <memory>
#include "ExprAST.hpp"
#include "NumberExprAST.hpp"
#include "VariableExprAST.hpp"
#include "BinaryExprAST.hpp"
#include "PrototypeAST.hpp"
#include "FunctionAST.hpp"
#include "CallExprAST.hpp"

//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//

// expression ast; base class for all expression nodes
ExprAST::~ExprAST() {}

// number expression ast; expression class for numeric literals
NumberExprAST::NumberExprAST(double val){
    Val = val;
}

// variable expression ast; expression class for referencing a variable
VariableExprAST::VariableExprAST(const std::string &name){
    Name = name;
}

// binary expression ast; expression class for a binary operator
BinaryExprAST::BinaryExprAST(char op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs){
    Op = op;
    LHS = std::move(lhs);
    RHS = std::move(rhs);
}

/// call expression ast; expression class for function calls
CallExprAST::CallExprAST(const std::string &callee, std::vector<std::unique_ptr<ExprAST>> args){
    Callee = callee;
    Args = std::move(Args);
}

// prototype ast; this class represents the "prototype" for a function
// which captures its name, and its argument names 
// (and therefore implicitly, the number of arguments the function takes)
PrototypeAST::PrototypeAST(const std::string &name, std::vector<std::string> args){
    Name = name;
    Args = std::move(args);
}

const std::string &PrototypeAST::getName() const{
    return Name;
}

// function ast; this class represents a function definition 
FunctionAST::FunctionAST(std::unique_ptr<PrototypeAST> proto, std::unique_ptr<ExprAST> body){
    Proto = std::move(proto);
    Body = std::move(body);
}