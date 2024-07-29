/*******************************************************************************
 * Copyright (c) 2024 - 2024.  Jiazhen LUO
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 ******************************************************************************/

/**
 * @file test_canonical_boolean_functions.cpp 
 * @author Jiazhen LUO
 * @date 2024/7/26
 */

#include "jazz/boolean-algebra.h"
#include <gtest/gtest.h>

using namespace std;
using namespace jazz;

#define TEST_CANONICAL_BOOLEAN_FUNCTIONS(b0, b1, b2, b3, NAME) \
    TEST(TestCanonicalBooleanFunctions, NAME) {                \
        auto f = Canonical::NAME();                            \
        auto truth_table = f.getTruthTable();                  \
        EXPECT_EQ(truth_table.size(), 4);                      \
        EXPECT_TRUE(truth_table[0].isEqual(b0));               \
        EXPECT_TRUE(truth_table[1].isEqual(b1));               \
        EXPECT_TRUE(truth_table[2].isEqual(b2));               \
        EXPECT_TRUE(truth_table[3].isEqual(b3));               \
    }

TEST_CANONICAL_BOOLEAN_FUNCTIONS(0, 0, 0, 0, Constant_0);
TEST_CANONICAL_BOOLEAN_FUNCTIONS(0, 0, 0, 1, And);
TEST_CANONICAL_BOOLEAN_FUNCTIONS(0, 0, 1, 0, X_And_Not_Y);
TEST_CANONICAL_BOOLEAN_FUNCTIONS(0, 0, 1, 1, LeftProjection);
TEST_CANONICAL_BOOLEAN_FUNCTIONS(0, 1, 0, 0, Not_X_And_Y);
TEST_CANONICAL_BOOLEAN_FUNCTIONS(0, 1, 0, 1, RightProjection);
TEST_CANONICAL_BOOLEAN_FUNCTIONS(0, 1, 1, 0, Xor);
TEST_CANONICAL_BOOLEAN_FUNCTIONS(0, 1, 1, 1, Or);
TEST_CANONICAL_BOOLEAN_FUNCTIONS(1, 0, 0, 0, Nor);
TEST_CANONICAL_BOOLEAN_FUNCTIONS(1, 0, 0, 1, Equivalence);
TEST_CANONICAL_BOOLEAN_FUNCTIONS(1, 0, 1, 0, Not_Y);
TEST_CANONICAL_BOOLEAN_FUNCTIONS(1, 0, 1, 1, X_Or_Not_Y);
TEST_CANONICAL_BOOLEAN_FUNCTIONS(1, 1, 0, 0, Not_X);
TEST_CANONICAL_BOOLEAN_FUNCTIONS(1, 1, 0, 1, Not_X_Or_Y);
TEST_CANONICAL_BOOLEAN_FUNCTIONS(1, 1, 1, 0, Nand);
TEST_CANONICAL_BOOLEAN_FUNCTIONS(1, 1, 1, 1, Constant_1);
