#ifndef __YASC_AST_FUNCTION_H_
#define __YASC_AST_FUNCTION_H_

#include <memory>
#include <vector>

#include "value.h"

namespace yasc {
    class Function {
    public:
        using Ptr = std::unique_ptr<Function>;
        using SPtr = std::shared_ptr<Function>;

        virtual ~Function() = default;
        virtual Value apply(std::vector<Value> vals) = 0;
    };
} // end of namespace yasc

#endif // __YASC_AST_FUNCTION_H_
