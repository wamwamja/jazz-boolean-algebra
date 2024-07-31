/**
 * @file arithmetics.h 
 * @author Jiazhen LUO
 * @date 2024/7/31
 */

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

#ifndef JAZZ_BOOLEAN_ALGEBRA_ARITHMETICS_H
#define JAZZ_BOOLEAN_ALGEBRA_ARITHMETICS_H

#include "jazz/simulation/component.h"
#include "jazz/simulation/logic_gates.h"

namespace jazz {
    namespace simulation {
        class HalfAdder : public Component {
        public:
            HalfAdder() : Component() {
                InOut *a = addIn("a", 1);
                InOut *b = addIn("b", 1);
                InOut *sum = addOut("sum", 1);
                InOut *carry = addOut("carry", 1);

                // version 1, made up of 6 nand gates
//                auto and_gate = addComponent(new And());
//                and_gate->connect(and_gate->in("a"), this, a);
//                and_gate->connect(and_gate->in("b"), this, b);
//                setOutput(carry, and_gate, and_gate->out("out"));
//
//                auto xor_gate = addComponent(new Xor());
//                xor_gate->connect(xor_gate->in("a"), this, a);
//                xor_gate->connect(xor_gate->in("b"), this, b);
//                setOutput(sum, xor_gate, xor_gate->out("out"));
                // version 2, made up of 5 nand gates
                Component* nand0 = addComponent(new Nand());
                Component* nand1 = addComponent(new Nand());
                Component* nand2 = addComponent(new Nand());
                Component* nand3 = addComponent(new Nand());
                Component* nand4 = addComponent(new Nand());

                nand0->connect(nand0->in("a"), this, a);
                nand0->connect(nand0->in("b"), this, b);

                nand1->connect(nand1->in("a"), nand0, nand0->out("out"));
                nand1->connect(nand1->in("b"), nand0, nand0->out("out"));
                setOutput(carry, nand1, nand1->out("out"));

                nand2->connect(nand2->in("a"), this, a);
                nand2->connect(nand2->in("b"), nand0, nand0->out("out"));
                nand3->connect(nand3->in("a"), this, b);
                nand3->connect(nand3->in("b"), nand0, nand0->out("out"));
                nand4->connect(nand4->in("a"), nand2, nand2->out("out"));
                nand4->connect(nand4->in("b"), nand2, nand3->out("out"));
                setOutput(sum, nand4, nand4->out("out"));


            };

            ~HalfAdder() override = default;

            const char *name() const override {
                return "HalfAdder";
            }

            OVERRIDE_COMPUTE(HalfAdder)
        };

    }// namespace simulation
}// namespace jazz

#endif//JAZZ_BOOLEAN_ALGEBRA_ARITHMETICS_H
