# llvm_compiler

A compiler for the Kaleidoscope language written with LLVM; including a lexer, parser, AST, and code generation support. 

Kaleidoscope is a procedural language that allows you to define functions, use conditionals, do math, and so on. We extend Kaleidoscope to support if/then/else statements, for loops, user defined operators, JIT compilation (with a simple command line interface), and debug information. The only datatype in Kaleidoscope is a 64-bit floating point type. All values are therefore implicitly double precision and do not require type declarations. <br>

Code follows the LLVM Tutorial: [My First Language Frontend with LLVM][1] <br>
Kaleidoscope description taken from the [LLVM tutorial][2]

[1]: <https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html>
[2]: <https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl01.html#the-kaleidoscope-language>