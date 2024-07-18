/**
 * @file test_or.cpp
 * @author Jiazhen LUO
 * @date on 2024-07-17.
 */

#include "jazz/boolean-algebra.h"
#include <gtest/gtest.h>

using namespace jazz;

TEST(TestOr, basic) {
    Expr p("p");
    EXPECT_TRUE((p | true).isEqual(true));
    EXPECT_TRUE(Expr(true).isEqual(p | true));
    EXPECT_TRUE((p | false).isEqual(p));
    EXPECT_TRUE(p.isEqual(p | false));
    EXPECT_TRUE((p | p).isEqual(p));
    EXPECT_TRUE(p.isEqual((p | p)));
    EXPECT_TRUE(p.isEqual((p | p | p)));
    EXPECT_TRUE((p | p | p).isEqual(p));
}

TEST(TestOr, equality) {
    Expr p("p");
    Expr q("q");
    Expr r("r");
    EXPECT_TRUE((p | q).isEqual(p | q));
    EXPECT_TRUE((p | q).isEqual(q | p));
    EXPECT_TRUE((p | q).isEqual(p | q | p));
    EXPECT_TRUE((p | q).isEqual(p | p | q));
    EXPECT_TRUE((p | q).isEqual(q | p | p));
    EXPECT_TRUE((p | q).isEqual(q | p | q));
    EXPECT_TRUE((p | q).isEqual(p | q | q));
    EXPECT_TRUE((p | q).isEqual(q | p | q));
    EXPECT_TRUE((p | q).isEqual(q | q | p));

    EXPECT_TRUE((p | q | r).isEqual(p | r | q));
    EXPECT_TRUE((p | q | r).isEqual(p | r | q | q | p | p | r));
}

TEST(TestOr, substitution) {
    Expr p("p");
    Expr q("q");
    Expr r = p | q;
    EXPECT_TRUE(r.subs(p == true).isEqual(true));
    EXPECT_TRUE(r.subs(q == true).isEqual(true));
    EXPECT_TRUE(Expr(true).isEqual(r.subs(p == true)));
    EXPECT_TRUE(Expr(true).isEqual(r.subs(q == true)));

    EXPECT_TRUE(r.subs(p == false).isEqual(q));
    EXPECT_TRUE(r.subs(q == false).isEqual(p));
    EXPECT_TRUE(q.isEqual(r.subs(p == false)));
    EXPECT_TRUE(p.isEqual(r.subs(q == false)));

    EXPECT_TRUE(r.subs(p == (p | q | r & true | false)).isEqual(p | q | p | r));
}
