#include "parser.hpp"

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//

parser parse;

int main() {
    // set standard binary operators 
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 30;
    BinopPrecedence['*'] = 40;

    // prime first token
    fprintf(stderr, "ready> ");
    parse.getNextToken();

    // run main "interpreter loop"
    parse.MainLoop();

    return 0;
}