#include <iostream>

#include "parser.h"
#include "evaluator.h"

#include "repl.h"

int main() {
    yasc::Repl repl {
        yasc::Parser{},
        yasc::Evaluator{},
        std::cin,
        std::cout
    };
    repl.run();
}
