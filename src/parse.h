#ifndef __PARSE_H_
#define __PARSE_H_

#include <memory>
#include <string>

#include "ast/list.h"

namespace yasc {
    std::shared_ptr<List> parse(std::string prog);
}

#endif // __PARSE_H_
