#ifndef __YASC_AST_IDENTIFIER_H_
#define __YASC_AST_IDENTIFIER_H_

#include <memory>
#include <string>

#include "value.h"

namespace yasc {
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
} // end of namespace yasc

#endif // __YASC_AST_IDENTIFIER_H_
