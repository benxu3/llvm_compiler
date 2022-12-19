#ifndef FunctionAST_H
#define FunctionAST_H

#include <memory>
#include "PrototypeAST.hpp"
#include "ExprAST.hpp"

// function ast; this class represents a function definition 
class FunctionAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;

    public:
        FunctionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExprAST> Body);
};

#endif