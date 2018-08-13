#ifndef __YASC_AST_VALUE_H_
#define __YASC_AST_VALUE_H_

#include <type_traits>

#include "number_fwd.h"

namespace yasc {
    class Value {
    public:
        enum class Type {
            Number,
            Pair,
            List,
            EmptyList,
            Identifier,
            Procedure
        };

        using Ptr  = std::shared_ptr<Value>;

        constexpr explicit Value(Type type)
            : type_(type)
        {}

        virtual ~Value() = default;

        constexpr Type type() const {
            return type_;
        }

        virtual std::ostream& print(std::ostream&) const = 0;

        Value& operator=(Value const&) = default;

    private:
        Type type_;
    };

    inline std::ostream& operator<<(std::ostream& o, Value const& v) {
        return v.print(o);
    }

    class Pair;
    class EmptyList;
    class List;
    class Identifier;
    class Procedure;

    namespace detail {
        template<typename T>
        constexpr bool check_type(Value::Type type) {
            return false;
        }

        template<>
        constexpr bool check_type<Integer>(Value::Type type) {
            return Value::Type::Number == type;
        }

        template<>
        constexpr bool check_type<Rational>(Value::Type type) {
            return Value::Type::Number == type;
        }

        template<>
        constexpr bool check_type<Real>(Value::Type type) {
            return Value::Type::Number == type;
        }

        template<>
        constexpr bool check_type<Complex>(Value::Type type) {
            return Value::Type::Number == type;
        }

        template<>
        constexpr bool check_type<Pair>(Value::Type type) {
            return Value::Type::Pair == type;
        }

        template<>
        constexpr bool check_type<List>(Value::Type type) {
            return Value::Type::List == type;
        }

        template<>
        constexpr bool check_type<EmptyList>(Value::Type type) {
            return Value::Type::EmptyList == type;
        }

        template<>
        constexpr bool check_type<Identifier>(Value::Type type) {
            return Value::Type::Identifier == type;
        }

        template<>
        constexpr bool check_type<Procedure>(Value::Type type) {
            return Value::Type::Procedure == type;
        }
    };

    template<typename T>
    T& value_cast(Value& val) {
        assert(detail::check_type<std::remove_reference_t<T>>(val.type()));
        return static_cast<T&>(val);
    }

    template<typename T>
    T const& value_cast(Value const& val) {
        assert(detail::check_type<std::remove_const_t<std::remove_reference_t<T>>>(val.type()));
        return static_cast<T const&>(val);
    }

    template<typename T>
    T value_cast(Value* val) {
        assert(detail::check_type<std::remove_pointer_t<T>>(val->type()));
        return static_cast<T>(val);
    }
}; // end of namespace yasc

#endif // __YASC_AST_VALUE_H_
