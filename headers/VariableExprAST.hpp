#ifndef VariableExprAST_H
#define VariableExprAST_H

#include "ExprAST.hpp"
#include <string>

// variable expression ast; expression class for referencing a variable
class VariableExprAST : public ExprAST {
    std::string Name;

    public:
        VariableExprAST(const std::string &Name);
};

#endif