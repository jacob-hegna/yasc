#include <gtest/gtest.h>

#include "../ast/list.h"
#include "../ast/number.h"
#include "../ast/value.h"
#include "../libscheme/arithmetic.h"

constexpr double EPSILON = 0.001;
#define EXPECT_EQ_REAL(tested, expected) EXPECT_NEAR(tested, expected, EPSILON)

template<class T, class...Ts>
T ast_arithmetic(yasc::Value::Ptr proc, T car, Ts...cdr) {
    using namespace yasc;
    std::vector<T> vec {{car, cdr...}};
    List list{};
    for(auto& v : vec) {
        list.push_back(std::make_shared<Number<T>>(v));
    }
    auto result = value_cast<Procedure&>(*proc).apply(list);
    return value_cast<Number<T>*>(result.get())->get();
}

template<class T, class...Ts>
T ast_plus(T arg, Ts...args) { return ast_arithmetic(yasc::arithmetic::get_plus<yasc::Number<T>>(), arg, args...); }

template<class T, class...Ts>
T ast_minus(T arg, Ts...args) { return ast_arithmetic(yasc::arithmetic::get_minus<yasc::Number<T>>(), arg, args...); }

template<class T, class...Ts>
T ast_multiplies(T arg, Ts...args) { return ast_arithmetic(yasc::arithmetic::get_multiplies<yasc::Number<T>>(), arg, args...); }

template<class T, class...Ts>
T ast_divide(T arg, Ts...args) { return ast_arithmetic(yasc::arithmetic::get_divides<yasc::Number<T>>(), arg, args...); }

TEST(astEvalArithmetic, plusBinary) {
    // integer
    EXPECT_EQ(ast_plus(3, 4),      7);
    EXPECT_EQ(ast_plus(100, 102),  202);
    EXPECT_EQ(ast_plus(-100, 100), 0);
    EXPECT_EQ(ast_plus(0, 1),      1);
    EXPECT_EQ(ast_plus(1, 0),      1);
    EXPECT_EQ(ast_plus(5, -15),   -10);

    // real
    EXPECT_EQ_REAL(ast_plus(1.0, 2.0),   3.0);
    EXPECT_EQ_REAL(ast_plus(1.5, 2.3),   3.8);
    EXPECT_EQ_REAL(ast_plus(-7.2, 1.0), -6.2);
}

TEST(astEvalArithmetic, plusAssociative) {
    // int
    EXPECT_EQ(ast_plus(1, 2, 3),           6);
    EXPECT_EQ(ast_plus(1, ast_plus(2, 3)), 6);
    EXPECT_EQ(ast_plus(ast_plus(1, 2), 3), 6);

    EXPECT_EQ(ast_plus(3, -4, 5),           4);
    EXPECT_EQ(ast_plus(3, ast_plus(-4, 5)), 4);
    EXPECT_EQ(ast_plus(ast_plus(3, -4), 5), 4);

    // real
    EXPECT_EQ_REAL(ast_plus(1.1, 2.2, 3.3),           6.6);
    EXPECT_EQ_REAL(ast_plus(1.1, ast_plus(2.2, 3.3)), 6.6);
    EXPECT_EQ_REAL(ast_plus(ast_plus(1.1, 2.2), 3.3), 6.6);

    EXPECT_EQ_REAL(ast_plus(9.1, -8.2, 7.3),           8.2);
    EXPECT_EQ_REAL(ast_plus(9.1, ast_plus(-8.2, 7.3)), 8.2);
    EXPECT_EQ_REAL(ast_plus(ast_plus(9.1, -8.2), 7.3), 8.2);
}

TEST(astEvalArithmetic, subtractionBinary) {
    // int
    EXPECT_EQ(ast_minus(1, 2),     -1);
    EXPECT_EQ(ast_minus(100, 100),  0);
    EXPECT_EQ(ast_minus(1020, 20),  1000);


    // real
    EXPECT_EQ_REAL(ast_minus(1.0, 3.0),        -2.0);
    EXPECT_EQ_REAL(ast_minus(10.7, 1.5),        9.2);
    EXPECT_EQ_REAL(ast_minus(123.456, 123.455), 0.001);
}

TEST(astEvalArithmetic, subtractionAssociative) {
    // int
    EXPECT_EQ(ast_minus(1, 2, 3),            -4);
    EXPECT_EQ(ast_minus(ast_minus(1, 2), 3), -4);
    EXPECT_EQ(ast_minus(1, ast_minus(2, 3)),  2);

    EXPECT_EQ(ast_minus(5, -10, 3),            12);
    EXPECT_EQ(ast_minus(ast_minus(5, -10), 3), 12);
    EXPECT_EQ(ast_minus(5, ast_minus(-10, 3)), 18);

    // real
    EXPECT_EQ_REAL(ast_minus(1.1, 2.2, 3.3),            -4.4);
    EXPECT_EQ_REAL(ast_minus(ast_minus(1.1, 2.2), 3.3), -4.4);
    EXPECT_EQ_REAL(ast_minus(1.1, ast_minus(2.2, 3.3)),  2.2);
}

TEST(astEvalArithmetic, multiplyAssociative) {
    // int
    EXPECT_EQ(ast_multiplies(7, 8, 9),                 504);;
    EXPECT_EQ(ast_multiplies(ast_multiplies(7, 8), 9), 504);
    EXPECT_EQ(ast_multiplies(7, ast_multiplies(8, 9)), 504);

    // real
    EXPECT_EQ_REAL(ast_multiplies(-1.7, 17.3, 4.3),                 -126.463);
    EXPECT_EQ_REAL(ast_multiplies(ast_multiplies(-1.7, 17.3), 4.3), -126.463);
    EXPECT_EQ_REAL(ast_multiplies(-1.7, ast_multiplies(17.3, 4.3)), -126.463);
}

TEST(astEvalArithmetic, divideAssociative) {
    // int
    EXPECT_EQ(ast_divide(34, 10, 3),             1);
    EXPECT_EQ(ast_divide(ast_divide(34, 10), 3), 1);
    EXPECT_EQ(ast_divide(34, ast_divide(10, 3)), 11);

    // real
    EXPECT_EQ_REAL(ast_divide(34.2, -10.1, 3.3),             -1.026);
    EXPECT_EQ_REAL(ast_divide(ast_divide(34.2, -10.1), 3.3), -1.026);
    EXPECT_EQ_REAL(ast_divide(34.2, ast_divide(-10.1, 3.3)), -11.174);
}

TEST(astEvalArithmetic, distribution) {
    // int
    EXPECT_EQ(ast_multiplies(4, ast_plus(2, -3)),     -4);
    EXPECT_EQ(ast_multiplies(3, ast_minus(7, 3)),      12);
    EXPECT_EQ(ast_multiplies(-12, ast_plus(1, 5), 2), -144);
    EXPECT_EQ(ast_multiplies(2, 3, ast_plus(4, 5, 6)), 90);
}

