#ifndef CallExprAST_H
#define CallExprAST_H

#include "ExprAST.hpp"
#include <string>
#include <vector>
#include <memory>

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
  std::string Callee;
  std::vector<std::unique_ptr<ExprAST>> Args;

public:
  CallExprAST(const std::string &Callee,
              std::vector<std::unique_ptr<ExprAST>> Args)
      : Callee(Callee), Args(std::move(Args)) {}
};


#endif