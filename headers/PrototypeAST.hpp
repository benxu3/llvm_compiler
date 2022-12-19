#ifndef PrototypeAST_H
#define PrototypeAST_H

#include <string>
#include <vector>

// prototype ast; this class represents the "prototype" for a function
// which captures its name, and its argument names 
// (and therefore implicitly, the number of arguments the function takes)
class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args; 

    public:
        PrototypeAST(const std::string &name, std::vector<std::string> Args);
        const std::string &getName() const;
};

#endif
