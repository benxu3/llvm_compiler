#include <map>

// binop precedence; holds the precedence for each binary operator
static std::map<char, int> BinopPrecedence;

class parser {
    public:
        int getNextToken();
        void MainLoop();
};