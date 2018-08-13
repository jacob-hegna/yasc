#ifndef __YASC_REPL_H_
#define __YASC_REPL_H_

#include <string>
#include <iostream>

#include "parser.h"
#include "evaluator.h"

namespace yasc {
    class Repl {
    public:
        Repl(Parser const& parser, Evaluator const& eval, std::istream& in, std::ostream& out)
            : parser_{parser}
            , eval_{eval}
            , in_{in}
            , out_{out}
            , prompt_{"=> "}
        {}

        void set_prompt(std::string const& prompt) {
            prompt_ = prompt;
        }

        void run() {
            out_ << "yasc 0.0 repl. :q to exit." << std::endl;
            std::string input_str;
            while(!in_.eof()) {
                out_ << prompt_;

                std::getline(in_, input_str);
                if(":q" == input_str)
                    break;

                auto ast = parser_(input_str);
                auto result = eval_(*ast, Evaluator::get_scheme_context());

                out_ << *result;
                out_ << std::endl;
            }
            out_ << "Memento mori." << std::endl;
        }

    private:
        Parser parser_;
        Evaluator eval_;

        std::istream& in_;
        std::ostream& out_;

        std::string prompt_;
    };
}
#endif
