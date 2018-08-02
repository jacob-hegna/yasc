#include <gtest/gtest.h>

#include "ast.h"
#include "parse.h"

template<class T>
typename yasc::Numeric<T>::SPtr astPlusHelper(T&& sum) {
    return std::make_shared<yasc::Numeric<T>>(sum);
}

template<class T, class...Ts>
typename yasc::Numeric<T>::SPtr astPlusHelper(T&& acc, Ts&&...nums) {
    auto lhs = yasc::make_value<yasc::Numeric<T>>(acc);
    auto rhs = astPlusHelper(std::forward<Ts>(nums)...);
    auto summands = std::vector<yasc::Value>{lhs, rhs};
    auto expr = yasc::make_value<yasc::List>(yasc::make_value<Plus<T>>(), summands);
    auto result = std::get<yasc::Literal::SPtr>(yasc::evaluate(expr));
    return std::static_pointer_cast<yasc::Numeric<T>>(result);
}

template<class T, class...Ts>
T astPlus(T&& acc, Ts&&...nums) {
    return astPlusHelper(std::forward<T>(acc), std::forward<Ts>(nums)...)->get();
}

template<class T>
typename yasc::Numeric<T>::SPtr astMinusHelper(T&& sum) {
    return std::make_shared<yasc::Numeric<T>>(sum);
}

template<class T, class...Ts>
typename yasc::Numeric<T>::SPtr astMinusHelper(T&& acc, Ts&&...nums) {
    auto lhs = yasc::make_value<yasc::Numeric<T>>(acc);
    auto rhs = astPlusHelper(std::forward<Ts>(nums)...);
    auto summands = std::vector<yasc::Value>{lhs, rhs};
    auto expr = yasc::make_value<yasc::List>(yasc::make_value<Minus<T>>(), summands);
    auto result = std::get<yasc::Literal::SPtr>(yasc::evaluate(expr));
    return std::static_pointer_cast<yasc::Numeric<T>>(result);
}

template<class T, class...Ts>
T astMinus(T&& acc, Ts&&...nums) {
    return astMinusHelper(std::forward<T>(acc), std::forward<Ts>(nums)...)->get();
}

TEST(astAddition, binary) {
    // int
    EXPECT_EQ(astPlus(3, 4),                    7);
    EXPECT_EQ(astPlus(100, 102),                202);
    EXPECT_EQ(astPlus(-100, 100),               0);
    EXPECT_EQ(astPlus(0, 1),                    1);
    EXPECT_EQ(astPlus(1, 0),                    1);
    EXPECT_EQ(astPlus(5, -15),                 -10);

    // double
    EXPECT_EQ(astPlus(1.0, 2.0),                3.0);
    EXPECT_EQ(astPlus(1.5, 2.3),                3.8);
    EXPECT_EQ(astPlus(-7.2, 1.0),              -6.2);
}

TEST(astAddition, associative) {
    // int
    EXPECT_EQ(astPlus(1, 2, 3),                 6);
    EXPECT_EQ(astPlus(1, astPlus(2, 3)),        6);
    EXPECT_EQ(astPlus(astPlus(1, 2), 3),        6);

    EXPECT_EQ(astPlus(3, -4, 5),                4);
    EXPECT_EQ(astPlus(3, astPlus(-4, 5)),       4);
    EXPECT_EQ(astPlus(astPlus(3, -4), 5),       4);

    // double
    EXPECT_EQ(astPlus(1.1, 2.2, 3.3),           6.6);
    EXPECT_EQ(astPlus(1.1, astPlus(2.2, 3.3)),  6.6);
    EXPECT_EQ(astPlus(astPlus(1.1, 2.2), 3.3),  6.6);

    EXPECT_EQ(astPlus(9.1, -8.2, 7.3),          8.2);
    EXPECT_EQ(astPlus(9.1, astPlus(-8.2, 7.3)), 8.2);
    EXPECT_EQ(astPlus(astPlus(9.1, -8.2), 7.3), 8.2);
}
