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
 * @file example_arithmetics.cpp 
 * @author Jiazhen LUO
 * @date 2024/7/31
 */

#include "jazz/simulation/arithmetics.h"

using namespace jazz::simulation;

int main() {

    // half adder
    {
        HalfAdder half_adder;
        auto a = half_adder.get("a");
        auto b = half_adder.get("b");
        auto sum = half_adder.get("sum");
        auto carry = half_adder.get("carry");
        printf(" a b | sum carry \n");
        printf("-----|-----------\n");
        for (int i = 0; i < 4; i++) {
            auto va = (i >> 1) & 0x01;
            auto vb = i & 0x01;
            half_adder.setInputValue(a, va);
            half_adder.setInputValue(b, vb);
            half_adder.compute();
            printf(" %d %d | %3d %5d\n", va, vb, sum->bit(0), carry->bit(0));
        }
        printf("Num of nand gates = %d\n", half_adder.countNandGate());
        half_adder.saveAsDot("/Users/meitu/Temp/trash/half_adder.dot");

        half_adder.unpack(2);
        half_adder.saveAsDot("/Users/meitu/Temp/trash/half_adder_unpack.dot");
        printf(" a b | sum carry \n");
        printf("-----|-----------\n");
        for (int i = 0; i < 4; i++) {
            auto va = (i >> 1) & 0x01;
            auto vb = i & 0x01;
            half_adder.setInputValue(a, va);
            half_adder.setInputValue(b, vb);
            half_adder.compute();
            printf(" %d %d | %3d %5d\n", va, vb, sum->bit(0), carry->bit(0));
        }

        int aaaa = 1;
    }

    // full adder
    {
        FullAdder full_adder;
        full_adder.saveAsDot("/Users/meitu/Temp/trash/full_adder.dot");
        full_adder.unpack(1);
        full_adder.saveAsDot("/Users/meitu/Temp/trash/full_adder_unpack_1.dot");
        full_adder.unpack(1);
        full_adder.saveAsDot("/Users/meitu/Temp/trash/full_adder_unpack_2.dot");
        full_adder.unpack(1);
        full_adder.saveAsDot("/Users/meitu/Temp/trash/full_adder_unpack_3.dot");

        auto a = full_adder.get("a");
        auto b = full_adder.get("b");
        auto c = full_adder.get("c");
        auto sum = full_adder.get("sum");
        auto carry = full_adder.get("carry");
        printf(" a b c | carry sum\n");
        printf("-------|-----------\n");
        for (int i = 0; i < 8; i++) {
            auto va = (i >> 2) & 0x01;
            auto vb = (i >> 1) & 0x01;
            auto vc = i & 0x01;
            full_adder.setInputValue(a, va);
            full_adder.setInputValue(b, vb);
            full_adder.setInputValue(c, vc);
            full_adder.compute();
            printf(" %d %d %d | %5d %3d\n", va, vb, vc, carry->bit(0), sum->bit(0));
        }
        printf("Num of nand gates = %d\n", full_adder.countNandGate());
    }

    return 0;
}
