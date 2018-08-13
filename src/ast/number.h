#ifndef __YASC_AST_NUMBER_H_
#define __YASC_AST_NUMBER_H_

#include <memory>
#include <numeric>
#include <complex>
#include <iostream>

namespace yasc {
    template<typename T>
    using fraction_t = std::pair<T, T>;

    template<typename T>
    std::ostream& operator<<(std::ostream& o, fraction_t<T> const& f) {
        o << f.first << "/" << f.second;
        return o;
    }

    template<typename T>
    fraction_t<T> operator+(fraction_t<T> const& lhs, fraction_t<T> const& rhs) {
        fraction_t<T> ret{};
        if(lhs.second == rhs.second) {
            ret.second = lhs.second;
            ret.first = rhs.first + lhs.first;
        } else {
            ret.second = std::lcm(lhs.second, rhs.second);
            ret.first  = (lhs.first * ret.second / lhs.second)
                       + (rhs.first * ret.second / rhs.second);
        }
        return ret;
    }

    template<typename T>
    fraction_t<T> operator-(fraction_t<T> const& f) {
        fraction_t<T> ret{f};
        ret.second *= -1;
        return ret;
    }

    template<typename T>
    fraction_t<T> operator-(fraction_t<T> const& lhs, fraction_t<T> const& rhs) {
        return lhs + (-rhs);
    }

    template<typename T>
    fraction_t<T> operator*(fraction_t<T> const& lhs, fraction_t<T> const& rhs) {
        fraction_t<T> ret{lhs};
        ret.first  *= rhs.first;
        ret.second *= rhs.second;
        return ret;
    }

    template<typename T>
    fraction_t<T> operator/(fraction_t<T> const& lhs, fraction_t<T> const& rhs) {
        fraction_t<T> ret{lhs};
        ret.first  *= rhs.second;
        ret.second *= rhs.first;
    }

    template<typename T>
    class Number : public Value {
    public:
        using Ptr = std::shared_ptr<Number>;

        constexpr Number()
            : Value(Value::Type::Number)
            , val_{}
        {}

        constexpr explicit Number(T val)
            : Value(Value::Type::Number)
            , val_{val}
        {}

        Number(Number<T> const& num)
            : Value(Value::Type::Number)
            , val_{num.get()}
        {}

        void set(T val) {
            val_ = val;
        }

        T get() const {
            return val_;
        }

        Number<T> operator+(Number<T> const& rhs) const { return Number(val_ + rhs.get()); }
        Number<T> operator-(Number<T> const& rhs) const { return Number(val_ - rhs.get()); }
        Number<T> operator*(Number<T> const& rhs) const { return Number(val_ * rhs.get()); }
        Number<T> operator/(Number<T> const& rhs) const { return Number(val_ / rhs.get()); }

        Number<T>& operator=(Number<T> const&) = default;
        Number<T>& operator=(Number<T>&&) = default;

        std::ostream& print(std::ostream& o) const override {
            o << get();
            return o;
        }

    private:
        T val_;
    };

    using Complex  = Number<std::complex<double>>;
    using Real     = Number<double>;
    using Rational = Number<fraction_t<int>>;
    using Integer  = Number<int>;
} // end of namespace yasc

#endif // __YASC_AST_NUMBER_H_
