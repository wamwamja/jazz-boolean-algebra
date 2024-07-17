/**
 * @file test_not.cpp
 * Test the Not class
 */

/*
 * Copyright (c) 2024.  Jiazhen LUO
 *
 * This program is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "jazz/boolean-algebra.h"
#include <gtest/gtest.h>

using namespace std;
using namespace jazz;


TEST(TestNot, constant) {
        EXPECT_TRUE(Expr(true).isEqual(true));
        EXPECT_TRUE(Expr(false).isEqual(false));
        EXPECT_FALSE(Expr(true).isEqual(false));
        EXPECT_FALSE(Expr(false).isEqual(true));
}

TEST(TestNot, symbol) {
        Expr p("p");
        EXPECT_TRUE((p).isEqual(p));
        EXPECT_TRUE((!p).isEqual(!p));
        EXPECT_FALSE((p).isEqual(!p));
        EXPECT_FALSE((!p).isEqual(p));
}

TEST(TestNot, multipleNot) {
    Expr p("p");
    Expr q("q");
    EXPECT_TRUE((!!p).isEqual(p));
    EXPECT_TRUE((p).isEqual((!!p)));
    EXPECT_TRUE((!!!p).isEqual(!p));
    EXPECT_TRUE((!p).isEqual(!!!p));
}

TEST(TEstNot, identicalOr) {
    Expr p("p");
    EXPECT_TRUE((p | !p).isEqual(true));
    EXPECT_TRUE((!p | p).isEqual(true));
}

TEST(TestNot, identicalAnd) {
    Expr p("p");
    EXPECT_TRUE((p & !p).isEqual(false));
    EXPECT_TRUE((!p & p).isEqual(false));
}

TEST(TestNot, substitution) {
    Expr p("p");
    Expr q("q");
    EXPECT_TRUE((!q).subs(q == !p).simplified().isEqual(p));
    EXPECT_TRUE((q).subs(q == !p).isEqual(!p));
    EXPECT_TRUE((q).subs(q == !p).subs(p == !q).simplified().isEqual(q));
}