#ifndef __YASC_AST_LITERAL_H_
#define __YASC_AST_LITERAL_H_

#include <memory>

namespace yasc {
    class Literal {
    public:
        enum class Type {
            NumericInt,
            NumericFloat,
            NumericDouble
        };

        using Ptr = std::unique_ptr<Literal>;
        using SPtr = std::shared_ptr<Literal>;

        virtual ~Literal() = default;

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

        ~Numeric() override = default;

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

} // end of namespace yasc

template<class T>
typename yasc::Numeric<T>::SPtr operator+(typename yasc::Numeric<T>::SPtr const& lhs, typename yasc::Numeric<T>::SPtr const& rhs) {
    return std::make_shared<yasc::Numeric<T>>(*lhs + *rhs);
}

#endif // __YASC_AST_LITERAL_H_
