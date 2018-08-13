#ifndef __YASC_AST_LIST_H_
#define __YASC_AST_LIST_H_

#include <cassert>
#include <numeric>
#include <memory>
#include <iostream>
#include <functional>
#include <vector>

#include "value.h"
#include "pair.h"

namespace yasc {
    // r6rs: "Scheme also has a distinguished empty list, which is the last cdr
    // in a chain of pairs that form a list"
    class EmptyList : public Value {
    public:
        EmptyList()
            : Value(Value::Type::EmptyList)
        {}

        std::ostream& print(std::ostream& o) const override {
            o << "\\0";
            return o;
        }
    };

    inline std::shared_ptr<EmptyList> get_empty_list() {
        return std::make_shared<EmptyList>();
    }

    namespace detail {
        // never checks for nullptr
        class ListIterator {
        public:
            enum class Loc {
                Car,
                Cdr
            };

            ListIterator()
                : pair_{nullptr}
                , loc_{Loc::Car}
            {}

            ListIterator(Pair* pair, Loc loc)
                : pair_{pair}
                , loc_{loc}
            {}

            ListIterator(ListIterator const&) = default;

            Value& operator*() const {
                if(Loc::Car == loc_) {
                    return *pair_->car();
                } else {
                    return *pair_->cdr();
                }
            }

            ListIterator& operator++() {
                if(Loc::Car == loc_ && Value::Type::EmptyList == pair_->cdr()->type()) {
                    loc_ = Loc::Cdr;
                } else {
                    pair_ = value_cast<Pair*>(pair_->cdr());
                }
                return *this;
            }

            ListIterator operator++(int) {
                auto pre_inc = *this;
                if(Loc::Car == loc_ && Value::Type::EmptyList == pair_->cdr()->type()) {
                    loc_ = Loc::Cdr;
                } else {
                    pair_ = value_cast<Pair*>(pair_->cdr());
                }
                return pre_inc;
            }

            bool operator==(ListIterator const& rhs) const {
                return (pair_ == rhs.pair_) && (loc_ == rhs.loc_);
            }
            bool operator!=(ListIterator const& rhs) const {
                return !(*this == rhs);
            };

        private:
            Pair* pair_;
            Loc loc_;
        };
    };

    class List : public Value {
    public:
        using iterator = detail::ListIterator;

        List()
            : Value{Value::Type::List}
            , head_{get_empty_list(), get_empty_list()}
            , back_{&head_}
            , size_{0}
        {}

        ~List() override = default;

        void push_back(Value::Ptr val) {
            if(size() == 0) {
                head_.set_car(std::move(val));
            } else {
                assert(Value::Type::EmptyList == back_->cdr()->type());
                back_->set_cdr(std::make_shared<Pair>(std::move(val), get_empty_list()));
                back_ = value_cast<Pair*>(back_->cdr());
            }
            ++size_;
        }

        int size() const {
            return size_;
        }

        Value* car() {
            return head_.car();
        }

        Value* cdr() {
            return head_.cdr();
        }

        iterator begin() const {
            return iterator{const_cast<Pair*>(&head_), iterator::Loc::Car};
        }

        iterator end() const {
            return iterator{back_, iterator::Loc::Cdr};
        }

        std::ostream& print(std::ostream& o) const override {
            o << "(";
            for(auto& val : *this) {
                o << val << " ";
            }
            o << ")";
            return o;
        }

    private:
        Pair head_;
        Pair* back_;
        int size_;
    };

    namespace detail {
        template<class T>
        std::shared_ptr<Pair> make_list_helper(T&& car) {
             return std::make_shared<Pair>(
                std::forward<T>(car),
                std::make_shared<EmptyList>()
            );
        }

        template<class T, class...Ts>
        std::shared_ptr<Pair> make_list_helper(T&& car, Ts&&...cdr) {
             return std::make_shared<Pair>(
                std::forward<T>(car),
                make_list_helper<Ts...>(std::forward<Ts>(cdr)...)
            );
        }
    }

    template<class T>
    Pair make_list(T&& car) {
        return Pair{
            std::forward<T>(car),
            std::shared_ptr<EmptyList>(nullptr)
        };
    }

    template<class T, class...Ts>
    Pair make_list(T&& car, Ts&&...cdr) {
        return Pair{
            std::forward<T>(car),
            detail::make_list_helper<Ts...>(std::forward<Ts>(cdr)...)
        };
    }
}; // end of namespace yasc

#endif // __YASC_AST_LIST_H_
