#ifndef BinaryExprAST_H
#define BinaryExprAST_H

#include "ExprAST.hpp"
#include <memory>

// binary expression ast; expression class for a binary operator
class BinaryExprAST : public ExprAST {
  char Op;
  std::unique_ptr<ExprAST> LHS, RHS;

  public:
    BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS);
};

#endif