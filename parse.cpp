#include <memory>
#include <string>
#include <cassert>

#include "parse.h"

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

    std::shared_ptr<yasc::List> parse_helper(std::string const& prog, std::string::iterator& itr) {
        auto list = yasc::List::SPtr{nullptr};

        for(; itr < prog.end(); ++itr) {
            switch(*itr) {
                case '(':
                    if(nullptr != list) {
                        list->emplace_back(parse_helper(prog, ++itr));
                        break;
                    }
                case ')': return list;
                case '+': 
                    list = std::make_shared<yasc::List>(yasc::make_value<yasc::Identifier>("+", yasc::make_value<Plus<int>>()));
                    break;
                case ' ': break;
                default:
                    auto tok = get_token(prog, itr);
                    assert(0 != tok.size());
                    itr += tok.size() - 1;
                    list->emplace_back(yasc::make_value<yasc::Numeric<int>>(std::stoi(tok)));
                    break;
            }
        }

        return list;
    }
};

namespace yasc {
    std::shared_ptr<List> parse(std::string prog) {
        auto itr = prog.begin() + 1;
        return (prog.empty())
            ? nullptr
            : parse_helper(prog, itr);
    }
};