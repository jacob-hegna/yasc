#ifndef __YASC_AST_PROCEDURE_H_
#define __YASC_AST_PROCEDURE_H_

#include <memory>
#include <vector>
#include <functional>

#include "value.h"
#include "number.h"
#include "pair.h"

namespace yasc {
    class Procedure : public Value {
    public:
        template<typename Callable>
        explicit Procedure(Callable impl)
            : Value(Value::Type::Procedure)
            , impl_{impl}
        {}

        Value::Ptr apply(Value const& v) const {
            return impl_(v);
        }

        std::ostream& print(std::ostream& o) const override {
            o << "[proc]";
            return o;
        }

    private:
        std::function<Value::Ptr(Value const&)> const impl_;
    };
} // end of namespace yasc

#endif // __YASC_AST_PROCEDURE_H_
