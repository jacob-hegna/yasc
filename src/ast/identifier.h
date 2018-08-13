#ifndef __YASC_AST_IDENTIFIER_H_
#define __YASC_AST_IDENTIFIER_H_

#include <memory>
#include <string>
#include <iostream>

#include "value.h"

namespace yasc {
    class Identifier : public Value {
    public:
        Identifier(std::string id, Value::Ptr val)
            : Value(Value::Type::Identifier)
            , id_(std::move(id))
            , val_(std::move(val))
        {}

        std::string id() const {
            return id_;
        }

        Value* val() {
            return val_.get();
        }

        bool operator==(std::string const& rhs) {
            return id_ == rhs;
        }

        bool operator==(Identifier const& rhs) {
            return id_ == rhs.id();
        }

        std::ostream& print(std::ostream& o) const override {
            o << id();
            return o;
        }

    private:
        std::string id_;
        Value::Ptr val_;
    };
} // end of namespace yasc

#endif // __YASC_AST_IDENTIFIER_H_
