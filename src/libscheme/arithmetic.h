#ifndef __YASC_LIBSCHEME_ARITHMETIC_H_
#define __YASC_LIBSCHEME_ARITHMETIC_H_

#include <algorithm>
#include <numeric>
#include <memory>
#include <vector>

#include "../ast/value.h"
#include "../ast/function.h"
#include "../ast/literal.h"

namespace yasc {
    template<typename T, typename BinaryCallable>
    class BinaryOp : public yasc::Function {
    public:
        ~BinaryOp() override = default;

        yasc::Value apply(std::vector<yasc::Value> operands) override {
            // work with numerics
            std::vector<typename yasc::Numeric<T>::SPtr> nums;
            std::transform(operands.begin(), operands.end(), std::back_inserter(nums),
                [] (yasc::Value const& val) {
                    return std::static_pointer_cast<yasc::Numeric<T>>(std::get<yasc::Literal::SPtr>(val));
                }
            );

            // accumulate onto the first element
            return std::static_pointer_cast<Literal>(std::accumulate(nums.begin()+1, nums.end(), *nums.begin(),
                [&] (typename yasc::Numeric<T>::SPtr acc, typename yasc::Numeric<T>::SPtr const& val) {
                    *acc = op_(*acc, *val);
                    return acc;
                }
            ));
       }
    private:
        static constexpr BinaryCallable op_ = BinaryCallable{};
    };

    template<class T>
    using Plus = BinaryOp<T, std::plus<Numeric<T>>>;

    template<class T>
    using Minus = BinaryOp<T, std::minus<Numeric<T>>>;

    template<class T>
    using Multiply = BinaryOp<T, std::multiplies<Numeric<T>>>;

    template<class T>
    using Divide = BinaryOp<T, std::divides<Numeric<T>>>;
}; // end of namespace yasc

#endif // __YASC_LIBSCHEME_ARITHMETIC_H_
