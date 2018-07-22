#include <algorithm>
#include <iostream>
#include <string>
#include <memory>
#include <optional>
#include <variant>
#include <vector>


namespace yasc {
    class Literal;
    class Identifier;
    class Expression;
    using Value = std::variant<
        std::shared_ptr<Literal>,
        std::shared_ptr<Identifier>,
        std::shared_ptr<Expression>
    >;

    class Literal {
    public:
        using Ptr = std::unique_ptr<Literal>;
        using SPtr = std::shared_ptr<Literal>;
    private:
    };

    template<typename T>
    class Numeric : public Literal {
    public:
        using Type = T;
        using Ptr = std::unique_ptr<Numeric>;
        using SPtr = std::shared_ptr<Numeric>;

        constexpr Numeric(Type const val)
            : val_(val)
            {}

    private:
        Type const val_;
    };

    class Identifier {
    public:
        using Ptr = std::unique_ptr<Identifier>;
        using SPtr = std::shared_ptr<Identifier>;
        explicit Identifier(std::string id)
            : id_(std::move(id))
            {}

        std::string id() const {
            return id_;
        }

        bool operator==(std::string const& rhs) {
            return id_ == rhs;
        }

        bool operator==(Identifier const& rhs) {
            return id_ == rhs.id();
        }

    private:
        std::string id_;
    };

    class Expression {
    public:
        using Ptr = std::unique_ptr<Expression>;
        using SPtr = std::unique_ptr<Expression>;

    private:
        std::vector<Value> payload_;
    };

    class Context {
    public:
        static Context prelude() {
            Context prelude;
            prelude.emplace("+");
            prelude.emplace("-");
            return prelude;
        }

        void push(Identifier::SPtr id) {
            ids_.emplace_back(std::move(id));
        }

        template<class...Args>
        void emplace(Args&&...args) {
            ids_.emplace_back(std::make_shared<Identifier>(std::forward<Args>(args)...));
        }

        Identifier::SPtr search(std::string id) {
            Identifier::SPtr contextId = nullptr;
            auto result = std::find_if(ids_.begin(), ids_.end(), [id] (Identifier::SPtr const& idPtr) {
                return *idPtr == id;
            });
            if(ids_.end() != result) {
                contextId = *result;
            }
            return contextId;
        }

    private:
        std::vector<Identifier::SPtr> ids_;
    };

};

namespace yasc::parsing {
    class Expression_Parser {
    public:
        explicit Expression_Parser(std::string expr, Context scope)
            : exprStr_(std::move(expr))
            , scope_(std::move(scope))
            {}

        Expression::Ptr parse() const {
            auto expr = std::make_unique<Expression>();

            return expr;
        }

    private:
        std::string exprStr_;
        Context scope_;
    };
}


std::vector<std::string> tokenize(std::string prog) {
    std::vector<std::string> tokens;
    unsigned int sentinel = -1;
    for(unsigned int i = 0; i < prog.size(); ++i) {
        auto c = prog.at(i);
        if(' ' == c) {
            if(i - 1 != sentinel) {
                tokens.emplace_back(prog.substr(sentinel + 1, i - sentinel - 1));
            }
            sentinel = i;
            continue;
        }
        if('(' == c || ')' == c) {
            if(i - 1 != sentinel) {
                tokens.emplace_back(prog.substr(sentinel + 1, i - sentinel - 1));
            }
            tokens.emplace_back(1, c);
            sentinel = i;
            continue;
        }
    }
    return tokens;
}

template<typename T>
std::optional<T> is_numeric(std::string str) {
    std::optional<T> result;
    if(std::all_of(str.begin(), str.end(), [] (char c) {return (c >= 48) && (c <= 57);})) {
        result = std::stoi(str);
    }
    return result;
}

yasc::Expression parse_expression(std::vector<std::string> tokens, yasc::Context context) {
    if("(" != tokens.at(0)) { return {}; }

    yasc::Expression expr;

    auto fn = context.search(tokens.at(1));
    if(!fn) {
        return {};
    }

}

int main() {
    std::string prog = "(+ 2179 333)";
    auto tokens = tokenize(prog);
    auto expr = parse_expression(tokens, yasc::Context::prelude());
}
