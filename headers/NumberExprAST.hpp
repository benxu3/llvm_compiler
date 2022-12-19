#ifndef NumberExprAST_H
#define NumberExprAST_H

#include "ExprAST.hpp"

// number expression ast; expression class for numeric literals
class NumberExprAST : public ExprAST {
    double Val;

    public:
        NumberExprAST(double Val);
};

#endif