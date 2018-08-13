#ifndef __YASC_EVALUATOR_H_
#define __YASC_EVALUATOR_H_

#include <unordered_map>
#include <utility>

#include "ast/value.h"
#include "ast/procedure.h"
#include "ast/number.h"
#include "ast/list.h"
#include "ast/identifier.h"

#include "libscheme/arithmetic.h"

namespace yasc {
    using Context = std::unordered_map<std::string, Value::Ptr>;
    struct Expression {
        using Ptr = std::shared_ptr<Expression>;

        Value::Ptr value;
        Expression::Ptr next;
    };

    class Evaluator {
    public:
        static Context get_scheme_context() {
            Context ctx;
            ctx["+"] = arithmetic::get_plus<Integer>();
            ctx["-"] = arithmetic::get_minus<Integer>();
            ctx["*"] = arithmetic::get_multiplies<Integer>();
            ctx["/"] = arithmetic::get_divides<Integer>();
            return ctx;
        }

        std::pair<Value::Ptr, Context> value_reduce(Value* val, Context ctx) {
            auto reduction = Value::Ptr{nullptr};
            switch(val->type()) {
                case Value::Type::Number: {
                    auto num = value_cast<Integer*>(val);
                    return std::make_pair(std::make_shared<Integer>(*num), ctx);
                }
                case Value::Type::Identifier: {
                    auto id = value_cast<Identifier*>(val);
                    return std::make_pair(ctx.at(id->id()), ctx);
                }
                case Value::Type::List: {
                    auto list = value_cast<List*>(val);
                    auto new_list= List{};
                    for(auto& i : *list) {
                        auto [result, _] = value_reduce(&i, ctx);
                        new_list.push_back(result);
                    }
                    auto func = value_cast<Procedure*>(new_list.car());
                    reduction = func->apply(*new_list.cdr());
                    break;
                }
                default: break;
                    //reduction = std::make_shared<Value>(*val);
            }
            return std::make_pair(reduction, ctx);
        }

        Value::Ptr operator()(Expression* expr, Context ctx) {
            auto result = Value::Ptr{nullptr};
            while(expr) {
                std::tie(result, ctx) = value_reduce(expr->value.get(), ctx);
                expr = expr->next.get();
            }
            return result;
        }

        Value::Ptr operator()(Value& value, Context ctx) {
            auto [result, _] = value_reduce(&value, ctx);
            return result;
        }
    };
};

#endif
