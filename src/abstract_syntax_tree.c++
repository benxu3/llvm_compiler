#include <string>
#include <vector>
#include <memory>

// expression ast; base class for all expression nodes
class ExprAST {
    public: 
        virtual ~ExprAST() {}
};

// number expression ast; expression class for numeric literals
class NumberExprAST : public ExprAST {
    double Val;

    public:
        NumberExprAST(double Val) : Val(Val) {}
};

// variable expression ast; expression class for referencing a variable
class VariableExprAST : public ExprAST {
    std::string Name;

    public:
        VariableExprAST(const std::string &Name) : Name(Name) {}
};

// binary expression ast; expression class for a binary operator
class BinaryExprAST : public ExprAST {
  char Op;
  std::unique_ptr<ExprAST> LHS, RHS;

  public:
    BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS) 
    : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {} 
};

// prototype ast; this class represents the "prototype" for a function
// which captures its name, and its argument names 
// (and therefore implicitly, the number of arguments the function takes)
class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args; 

    public:
        PrototypeAST(const std::string &name, std::vector<std::string> Args)
        : Name(name), Args(std::move(Args)) {}

        const std::string &getName() const { return Name; }
};

// function ast; this class represents a function definition 
class FunctionAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;

    public:
        FunctionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExprAST> Body)
        : Proto(std::move(Proto)), Body(std::move(Body)) {}
};