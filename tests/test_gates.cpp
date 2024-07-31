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
 * @file test_gates.cpp 
 * @author Jiazhen LUO
 * @date 2024/7/31
 */

#include "jazz/simulation/logic_gates.h"
#include <gtest/gtest.h>

using namespace jazz;

#define TEST_GATES(b0, b1, b2, b3, NAME)                   \
    TEST(TestGates, NAME) {                                \
        bool bs[] = {b0, b1, b2, b3};                      \
        do {                                               \
            jazz::simulation::NAME gate;                   \
            unsigned int num_input = gate.numInput();      \
            unsigned int num_row = 1 << num_input;         \
            jazz::InOut *in_a = gate.in("a");              \
            jazz::InOut *in_b = gate.in("b");              \
            for (int i = 0; i < num_row; i++) {            \
                auto a = (i >> 1) & 0x01;                  \
                auto b = i & 0x01;                         \
                if (num_input > 1) {                       \
                    gate.setInputValue(in_a, a);           \
                    gate.setInputValue(in_b, b);           \
                    gate.compute();                        \
                } else {                                   \
                    gate.setInputValue(in_a, b);           \
                    gate.compute();                        \
                }                                          \
                EXPECT_EQ(gate.out("out")->bit(0), bs[i]); \
            }                                              \
        } while (0);                                       \
    }

TEST_GATES(1, 1, 1, 0, Nand);
TEST_GATES(1, 0, 0, 0, Invert);// only the first 2 bits are used
TEST_GATES(0, 0, 0, 1, And);
TEST_GATES(0, 1, 1, 1, Or);
TEST_GATES(0, 1, 1, 0, Xor);
