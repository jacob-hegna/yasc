#ifndef __YASC_AST_LIST_H_
#define __YASC_AST_LIST_H_

#include <memory>
#include <vector>

#include "value.h"

namespace yasc {
    class List {
    public:
        using Ptr = std::unique_ptr<List>;
        using SPtr = std::shared_ptr<List>;

        List(Value op, std::vector<Value> opers = {})
            : operator_(std::move(op))
            , operands_(opers)
        {}

		template<class...Ts>
        void emplace_back(Ts&&...Args) {
            operands_.emplace_back(std::forward<Ts>(Args)...);
        }

        Value getOperator() const {
            return operator_;
        }

        auto getOperands() const {
            return operands_;
        }

    private:
        Value operator_;
        std::vector<Value> operands_;
    };
}; // end of namespace yasc

#endif // __YASC_AST_LIST_H_
