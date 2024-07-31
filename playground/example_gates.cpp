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
 * @file example_gates.cpp 
 * @author Jiazhen LUO
 * @date 2024/7/30
 */

#include "jazz/simulation/logic_gates.h"

#define TEST_GATE(class_gate)                                           \
    do {                                                                \
        jazz::Gate::class_gate gate;                                    \
        unsigned int num_input = gate.numInput();                       \
        unsigned int num_row = 1 << num_input;                          \
        jazz::InOut *in_a = gate.in("a");                               \
        jazz::InOut *in_b = gate.in("b");                               \
        printf("== %s gate ==\n", #class_gate);                         \
        for (int i = 0; i < num_row; i++) {                             \
            auto a = (i >> 1) & 0x01;                                   \
            auto b = i & 0x01;                                          \
            if (num_input > 1) {                                        \
                gate.setInputValue(in_a, a);                            \
                gate.setInputValue(in_b, b);                            \
                gate.compute();                                         \
                printf(" %d %d | %d\n", a, b, gate.out("out")->bit(0)); \
            } else {                                                    \
                gate.setInputValue(in_a, b);                            \
                gate.compute();                                         \
                printf(" %d  | %d\n", b, gate.out("out")->bit(0));      \
            }                                                           \
        }                                                               \
        printf("nand_count = %d\n", gate.countNandGate());              \
        printf("\n");                                                   \
    } while (0)

int main() {
    TEST_GATE(Nand);
    TEST_GATE(Invert);
    TEST_GATE(And);
    TEST_GATE(Or);
    TEST_GATE(Xor);

    return 0;
}
