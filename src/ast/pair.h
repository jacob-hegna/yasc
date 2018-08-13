#ifndef __YASC_AST_PAIR_H_
#define __YASC_AST_PAIR_H_

#include <memory>

#include "value.h"

namespace yasc {
    class Pair : public Value {
    public:
        template<typename CarT, typename CdrT> // must be moveable into Value
        Pair(CarT&& car, CdrT&& cdr)
            : Value(Value::Type::Pair)
            , car_{std::move(car)}
            , cdr_{std::move(cdr)}
        {}

        ~Pair() override = default;

        template<typename T>
        void set_car(T&& car) {
            car_ = std::move(car);
        }

        template<typename T>
        void set_cdr(T&& cdr) {
            cdr_ = std::move(cdr);
        }

        Value* car() const {
            return car_.get();
        }

        Value* cdr() const {
            return cdr_.get();
        }

        std::ostream& print(std::ostream& o) const override {
            o << "(" << *car() << ", " << *cdr() << ")";
            return o;
        }
    private:
        Value::Ptr car_;
        Value::Ptr cdr_;
    };
};

#endif
