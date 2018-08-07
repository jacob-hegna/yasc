#include <gtest/gtest.h>

#include "../ast/list.h"
#include "../ast/literal.h"
#include "../ast/value.h"
#include "../libscheme/arithmetic.h"
#include "../evaluate.h"

constexpr double EPSILON = 0.001;
#define EXPECT_EQ_DOUBLE(tested, expected) EXPECT_NEAR(tested, expected, EPSILON)

template<class Op, class T, class...Ts>
typename yasc::Numeric<T>::SPtr astBinaryHelper(T&& acc, Ts&&...nums) {
    std::vector<T> numsVec { {acc, nums...} };
    std::vector<yasc::Value> astNums;
    std::transform(numsVec.begin(), numsVec.end(), std::back_inserter(astNums),
        [] (T num) {
            return std::static_pointer_cast<yasc::Literal>(std::make_shared<yasc::Numeric<T>>(num));
        }
    );

    auto expr = yasc::make_value<yasc::List>(yasc::make_value<Op>(), astNums);
    auto result = std::get<yasc::Literal::SPtr>(yasc::evaluate(expr));
    return std::static_pointer_cast<yasc::Numeric<T>>(result);
}

template<class T, class...Ts>
T astPlus(T&& acc, Ts&&...nums) {
    return astBinaryHelper<yasc::Plus<T>>(std::forward<T>(acc), std::forward<Ts>(nums)...)->get();
}

template<class T, class...Ts>
T astMinus(T&& acc, Ts&&...nums) {
    return astBinaryHelper<yasc::Minus<T>>(std::forward<T>(acc), std::forward<Ts>(nums)...)->get();
}

template<class T, class...Ts>
T astMultiply(T&& acc, Ts&&...nums) {
    return astBinaryHelper<yasc::Multiply<T>>(std::forward<T>(acc), std::forward<Ts>(nums)...)->get();
}

template<class T, class...Ts>
T astDivide(T&& acc, Ts&&...nums) {
    return astBinaryHelper<yasc::Divide<T>>(std::forward<T>(acc), std::forward<Ts>(nums)...)->get();
}

TEST(astAddition, binary) {
    // int
    EXPECT_EQ(astPlus(3, 4),                                7);
    EXPECT_EQ(astPlus(100, 102),                            202);
    EXPECT_EQ(astPlus(-100, 100),                           0);
    EXPECT_EQ(astPlus(0, 1),                                1);
    EXPECT_EQ(astPlus(1, 0),                                1);
    EXPECT_EQ(astPlus(5, -15),                             -10);

    // double
    EXPECT_EQ_DOUBLE(astPlus(1.0, 2.0),                     3.0);
    EXPECT_EQ_DOUBLE(astPlus(1.5, 2.3),                     3.8);
    EXPECT_EQ_DOUBLE(astPlus(-7.2, 1.0),                   -6.2);
}

TEST(astAddition, associative) {
    // int
    EXPECT_EQ(astPlus(1, 2, 3),                             6);
    EXPECT_EQ(astPlus(1, astPlus(2, 3)),                    6);
    EXPECT_EQ(astPlus(astPlus(1, 2), 3),                    6);

    EXPECT_EQ(astPlus(3, -4, 5),                            4);
    EXPECT_EQ(astPlus(3, astPlus(-4, 5)),                   4);
    EXPECT_EQ(astPlus(astPlus(3, -4), 5),                   4);

    // double
    EXPECT_EQ_DOUBLE(astPlus(1.1, 2.2, 3.3),                6.6);
    EXPECT_EQ_DOUBLE(astPlus(1.1, astPlus(2.2, 3.3)),       6.6);
    EXPECT_EQ_DOUBLE(astPlus(astPlus(1.1, 2.2), 3.3),       6.6);

    EXPECT_EQ_DOUBLE(astPlus(9.1, -8.2, 7.3),               8.2);
    EXPECT_EQ_DOUBLE(astPlus(9.1, astPlus(-8.2, 7.3)),      8.2);
    EXPECT_EQ_DOUBLE(astPlus(astPlus(9.1, -8.2), 7.3),      8.2);
}

TEST(astSubtraction, binary) {
    // int
    EXPECT_EQ(astMinus(1, 2),                              -1);
    EXPECT_EQ(astMinus(100, 100),                           0);
    EXPECT_EQ(astMinus(1020, 20),                           1000);


    // double
    EXPECT_EQ_DOUBLE(astMinus(1.0, 3.0),                   -2.0);
    EXPECT_EQ_DOUBLE(astMinus(10.7, 1.5),                   9.2);
    EXPECT_EQ_DOUBLE(astMinus(123.456, 123.455),            0.001);
}

TEST(astSubtraction, associative) {
    // int
    EXPECT_EQ(astMinus(1, 2, 3),                           -4);
    EXPECT_EQ(astMinus(astMinus(1, 2), 3),                 -4);
    EXPECT_EQ(astMinus(1, astMinus(2, 3)),                  2);

    EXPECT_EQ(astMinus(5, -10, 3),                          12);
    EXPECT_EQ(astMinus(astMinus(5, -10), 3),                12);
    EXPECT_EQ(astMinus(5, astMinus(-10, 3)),                18);

    // double
    EXPECT_EQ_DOUBLE(astMinus(1.1, 2.2, 3.3),              -4.4);
    EXPECT_EQ_DOUBLE(astMinus(astMinus(1.1, 2.2), 3.3),    -4.4);
    EXPECT_EQ_DOUBLE(astMinus(1.1, astMinus(2.2, 3.3)),     2.2);
}

/*
 * skipping astMultiply, binary
 */

TEST(astMultiply, associative) {
    // int
    EXPECT_EQ(astMultiply(7, 8, 9),                         504);
    EXPECT_EQ(astMultiply(astMultiply(7, 8), 9),            504);
    EXPECT_EQ(astMultiply(7, astMultiply(8, 9)),            504);

    // double
    EXPECT_EQ_DOUBLE(astMultiply(-1.7, 17.3, 4.3),              -126.463);
    EXPECT_EQ_DOUBLE(astMultiply(astMultiply(-1.7, 17.3), 4.3), -126.463);
    EXPECT_EQ_DOUBLE(astMultiply(-1.7, astMultiply(17.3, 4.3)), -126.463);
}

TEST(astDivide, associative) {
    // int
    EXPECT_EQ(astDivide(34, 10, 3),                         1);
    EXPECT_EQ(astDivide(astDivide(34, 10), 3),              1);
    EXPECT_EQ(astDivide(34, astDivide(10, 3)),              11);

    // double
    EXPECT_EQ_DOUBLE(astDivide(34.2, -10.1, 3.3),               -1.026);
    EXPECT_EQ_DOUBLE(astDivide(astDivide(34.2, -10.1), 3.3),    -1.026);
    EXPECT_EQ_DOUBLE(astDivide(34.2, astDivide(-10.1, 3.3)),    -11.174);
}
