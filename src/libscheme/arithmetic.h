#ifndef __YASC_LIBSCHEME_ARITHMETIC_H_
#define __YASC_LIBSCHEME_ARITHMETIC_H_

#include <memory>
#include <vector>

#include "../ast/value.h"
#include "../ast/function.h"
#include "../ast/literal.h"

namespace yasc {
    template<typename T>
    class Plus : public yasc::Function {
    public:
        ~Plus() override = default;

        yasc::Value apply(std::vector<yasc::Value> operands) override {
            auto sum = std::make_shared<yasc::Numeric<T>>(0);
            for(auto const& summand : operands) {
                auto summand_literal = std::get<typename yasc::Literal::SPtr>(summand);
                *sum += *std::static_pointer_cast<yasc::Numeric<T>>(summand_literal);
            }
            return sum;
        }
    };
}; // end of namespace yasc

#endif // __YASC_LIBSCHEME_ARITHMETIC_H_
