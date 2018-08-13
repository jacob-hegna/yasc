#ifndef __PARSER_H_
#define __PARSER_H_

#include <string>

#include "ast/value.h"

namespace yasc {
    class Parser {
    public:
        Parser() = default;

        Value::Ptr operator()(std::string prog);
    private:
        Value::Ptr parse(std::string const& prog, std::string::iterator& itr);
    };
}

#endif // __PARSER_H_
