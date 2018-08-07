#ifndef __YASC_AST_VALUE_H_
#define __YASC_AST_VALUE_H_

#include <memory>
#include <variant>

namespace yasc {
    class Literal;
    class Function;
    class Identifier;
    class List;

    using Value = std::variant<
        std::shared_ptr<Literal>,
        std::shared_ptr<Identifier>,
        std::shared_ptr<Function>,
        std::shared_ptr<List>
    >;

    template<class T, class...Args>
    Value make_value(Args&&...args) {
        return Value{std::make_shared<T>(std::forward<Args>(args)...)};
    }

}; // end of namespace yasc

#endif // __YASC_AST_VALUE_H_
