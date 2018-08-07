#include <iostream>
#include <string>
#include <variant>

#include "parse.h"
#include "evaluate.h"

int main() {
    using namespace yasc;
    //auto vals = std::vector<Value>(100, make_value<Numeric<double>>(.01));
    //auto expr = make_value<List>(make_value<Identifier>("+", make_value<Plus<double>>()), vals);
    auto expr = yasc::parse("(+ (+ 3 4) (+ 5 6) 7 8 9 10)");
    std::visit(PrintSchemeVisitor{}, Value{expr}); std::cout << std::endl;
    std::visit(PrintSchemeVisitor{}, std::visit(EvalSchemeVisitor{}, Value{expr}));
    std::cout << std::endl;
    return 0;
}
