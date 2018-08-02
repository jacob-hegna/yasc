#ifndef __AST_H_
#define __AST_H_

#include <algorithm>
#include <string>
#include <memory>
#include <numeric>
#include <iostream>
#include <variant>
#include <vector>

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

    class Literal {
    public:
        enum class Type {
            NumericInt,
            NumericFloat,
            NumericDouble
        };

        using Ptr = std::unique_ptr<Literal>;
        using SPtr = std::shared_ptr<Literal>;

        virtual Type type() const = 0;
    };

    template<typename T>
    class Numeric : public Literal {
    public:
        using Ptr = std::unique_ptr<Numeric>;
        using SPtr = std::shared_ptr<Numeric>;

        constexpr Numeric()
            : val_(0)
        {}

        constexpr explicit Numeric(T val)
            : val_(val)
        {}

        Numeric(Numeric<T> const& num)
            : val_(num.get())
        {}

        void set(T val) {
            val_ = val;
        }

        T get() const {
            return val_;
        }

        Literal::Type type() const override;

        Numeric<T> operator+(Numeric<T> const& rhs) {
            return Numeric(val_ + rhs.get());
        }
        Numeric<T> operator+=(Numeric<T> const& rhs) {
            val_ += rhs.get();
            return *this;
        }

        Numeric<T> operator-(Numeric<T> const& rhs) {
            return Numeric(val_ - rhs.get());
        }
        Numeric<T> operator-=(Numeric<T> const& rhs) {
            val_ -= rhs.get();
            return *this;
        }
    private:
        T val_;
    };

    template<>
    inline Literal::Type Numeric<int>::type() const {
        return Literal::Type::NumericInt;
    }

    template<>
    inline Literal::Type Numeric<float>::type() const {
        return Literal::Type::NumericFloat;
    }

    template<>
    inline Literal::Type Numeric<double>::type() const {
        return Literal::Type::NumericDouble;
    }

    class Function {
    public:
        using Ptr = std::unique_ptr<Function>;
        using SPtr = std::shared_ptr<Function>;
        virtual Value apply(std::vector<Value> vals) = 0;
    private:
    };

    class Identifier {
    public:
        using Ptr = std::unique_ptr<Identifier>;
        using SPtr = std::shared_ptr<Identifier>;
        Identifier(std::string id, Value val)
            : id_(std::move(id))
            , val_(std::move(val))
        {}

        std::string id() const {
            return id_;
        }

        Value val() const {
            return val_;
        }

        bool operator==(std::string const& rhs) {
            return id_ == rhs;
        }

        bool operator==(Identifier const& rhs) {
            return id_ == rhs.id();
        }

    private:
        std::string id_;
        Value val_;
    };

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

    template<class T, class...Args>
    Value make_value(Args&&...args) {
        return Value{std::make_shared<T>(std::forward<Args>(args)...)};
    }

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

template<class T>
typename yasc::Numeric<T>::SPtr operator+(typename yasc::Numeric<T>::SPtr const& lhs, typename yasc::Numeric<T>::SPtr const& rhs) {
    return std::make_shared<yasc::Numeric<T>>(*lhs + *rhs);
}

template<typename T>
class Plus : public yasc::Function {
    yasc::Value apply(std::vector<yasc::Value> operands) override {
        auto sum = std::make_shared<yasc::Numeric<T>>(0);
        for(auto const& summand : operands) {
            auto summand_literal = std::get<typename yasc::Literal::SPtr>(summand);
            *sum += *std::static_pointer_cast<yasc::Numeric<T>>(summand_literal);
        }
        return sum;
    }
};

#endif // __AST_H_
