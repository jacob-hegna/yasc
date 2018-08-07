#ifndef __YASC_EVALUATE_H_
#define __YASC_EVALUATE_H_

#include <iostream>
#include <memory>
#include <variant>

#include "ast/value.h"
#include "ast/list.h"
#include "ast/identifier.h"
#include "ast/function.h"
#include "ast/literal.h"

namespace yasc {
    struct PrintSchemeVisitor {
        void operator()(yasc::Literal::SPtr literal) {
            if(yasc::Literal::Type::NumericInt == literal->type()) {
                std::cout << std::static_pointer_cast<yasc::Numeric<int>>(literal)->get();
            }
            if(yasc::Literal::Type::NumericDouble == literal->type()) {
                std::cout << std::static_pointer_cast<yasc::Numeric<double>>(literal)->get();
            }
        }
        void operator()(yasc::Identifier::SPtr id) {
            std::cout << id->id();
        }
        void operator()(yasc::Function::SPtr fn) {
            std::cout << "fn";
        }
        void operator()(yasc::List::SPtr expr) {
            std::cout << "(";
            std::visit(*this, expr->getOperator());
            std::cout << " ";
            for(auto& operand : expr->getOperands()) {
                std::visit(*this, operand);
                std::cout << " ";
            }
            std::cout << ")";
        }
    };

    struct EvalSchemeVisitor {
        yasc::Value operator()(yasc::Identifier::SPtr id) {
            return id->val();
        }
        yasc::Value operator()(yasc::Literal::SPtr literal) {
            return literal;
        }
        yasc::Value operator()(yasc::Function::SPtr fn) {
            return fn;
        }
        yasc::Value operator()(yasc::List::SPtr expr) {
            auto fn = std::get<yasc::Function::SPtr>(expr->getOperator());

            std::vector<yasc::Value> values;
            auto operands = expr->getOperands();
            std::transform(operands.begin(), operands.end(), std::back_inserter(values), [] (yasc::Value val) {
                return std::visit(EvalSchemeVisitor{}, val);
            });

            return fn->apply(values);
        }
    };

    inline Value evaluate(Value const& ast) {
        return std::visit(EvalSchemeVisitor{}, ast);
    }
};

#endif // __YASC_EVAULATE_H_
