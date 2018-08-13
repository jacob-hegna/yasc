#ifndef __YASC_LIBSCHEME_ARITHMETIC_H_
#define __YASC_LIBSCHEME_ARITHMETIC_H_

#include <algorithm>
#include <numeric>
#include <memory>
#include <vector>

#include "../ast/value.h"
#include "../ast/procedure.h"
#include "../ast/number.h"
#include "../ast/pair.h"
#include "../ast/list.h"

namespace yasc {

    namespace arithmetic {
        namespace detail {
            template<typename T, typename Callable>
            T const unity = T{0};

            template<typename T>
            T const unity<T, std::multiplies<T>> = T{1};

            template<typename T>
            T const unity<T, std::divides<T>> = T{1};

            template<typename T, typename Callable>
            std::shared_ptr<T> call_procedure(Value const& value, Callable c = Callable{}) {
                switch(value.type()) {
                    case Value::Type::Number:
                        return std::make_shared<T>(value_cast<T>(value));
                    case Value::Type::Pair: {
                        auto const& pair = value_cast<Pair const&>(value);
                        auto car = call_procedure<T>(*pair.car(), c);
                        auto cdr = call_procedure<T>(*pair.cdr(), c);
                        return std::make_shared<T>(c(*car, *cdr));
                    }
                    case Value::Type::List: {
                        auto const& list = value_cast<List const&>(value);
                        auto acc = value_cast<T&>(*std::begin(list));
                        auto result = std::accumulate(++std::begin(list), std::end(list), acc,
                            [&] (auto const& acc, auto const& itr) {
                                return c(acc, value_cast<T const&>(itr));
                            }
                        );
                        return std::make_shared<T>(result);
                    }
                    case Value::Type::EmptyList:
                        return std::make_shared<T>(unity<T, Callable>);
                    default:
                        return std::shared_ptr<T>{nullptr};
                }
            }

            template<typename T, typename Callable>
            Value::Ptr get_procedure(Callable c = Callable{}) {
                return std::make_shared<Procedure>( [] (Value const& val) -> Value::Ptr {
                    return call_procedure<T, Callable>(val);
                });
            }
        };

        template<typename T>
        Value::Ptr get_plus() {
            return detail::get_procedure<T, std::plus<T>>();
        }

        template<typename T>
        Value::Ptr get_minus() {
            return detail::get_procedure<T, std::minus<T>>();
        }

        template<typename T>
        Value::Ptr get_multiplies() {
            return detail::get_procedure<T, std::multiplies<T>>();
        }

        template<typename T>
        Value::Ptr get_divides() {
            return detail::get_procedure<T, std::divides<T>>();
        }
    };
}

#endif // __YASC_LIBSCHEME_ARITHMETIC_H_
