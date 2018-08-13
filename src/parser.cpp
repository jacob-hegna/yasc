#include <memory>
#include <string>
#include <cassert>

#include "parser.h"
#include "ast/list.h"
#include "ast/number.h"
#include "ast/value.h"
#include "ast/identifier.h"

#include "libscheme/arithmetic.h"

namespace {
    std::string get_token(std::string const& prog, std::string::iterator& itr) {
        std::string token;
        auto tok_end = prog.find_first_of(" )\n", itr - prog.begin());
        if(prog.size() != tok_end) {
            auto start = itr - prog.begin();
            token = prog.substr(start, tok_end - start);
        }
        return token;
    }
}

namespace yasc {
    Value::Ptr Parser::operator()(std::string prog) {
        auto itr = prog.begin() + 1;
        return (prog.empty())
            ? nullptr
            : parse(prog, itr);
    }

    Value::Ptr Parser::parse(std::string const& prog, std::string::iterator& itr) {
        using namespace yasc;
        auto list = std::shared_ptr<List>{nullptr};

        for(; itr < prog.end(); ++itr) {
            switch(*itr) {
                case '(':
                    if(nullptr != list) {
                        list->push_back(parse(prog, ++itr));
                        break;
                    }
                case ')': return list;
                case '+':
                    list = std::make_shared<List>();
                    list->push_back(std::make_shared<Identifier>("+", arithmetic::get_plus<Integer>()));
                    break;
                case '-':
                    list = std::make_shared<List>();
                    list->push_back(std::make_shared<Identifier>("-", arithmetic::get_plus<Integer>()));
                    break;
                case '*':
                    list = std::make_shared<List>();
                    list->push_back(std::make_shared<Identifier>("*", arithmetic::get_plus<Integer>()));
                    break;
                case '/':
                    list = std::make_shared<List>();
                    list->push_back(std::make_shared<Identifier>("/", arithmetic::get_plus<Integer>()));
                    break;
                case ' ': break;
                default:
                    auto tok = get_token(prog, itr);
                    assert(0 != tok.size());
                    itr += tok.size() - 1;
                    list->push_back(std::make_shared<Integer>(std::stod(tok)));
                    break;
            }
        }

        return list;
    }
};
