/**
 * @file test_and.cpp
 * @author Jiazhen LUO
 * @date on 2024-07-17.
 */

#include "jazz/boolean-algebra.h"
#include <gtest/gtest.h>

using namespace jazz;

TEST(TestAnd, basic) {
    Expr p("p");
    EXPECT_TRUE((p & true).isEqual(p));
    EXPECT_TRUE(Expr(p).isEqual(p & true));
    EXPECT_TRUE((p & false).isEqual(false));
    EXPECT_TRUE(Expr(false).isEqual(p & false));
    EXPECT_TRUE((p & p).isEqual(p));
    EXPECT_TRUE(p.isEqual((p & p)));
    EXPECT_TRUE(p.isEqual((p & p & p)));
    EXPECT_TRUE((p & p & p).isEqual(p));
}
