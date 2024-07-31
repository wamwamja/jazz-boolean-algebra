/**
 * @file comp_nand.h 
 * @author Jiazhen LUO
 * @date 2024/7/30
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

#ifndef JAZZ_BOOLEAN_ALGEBRA_LOGIC_GATES_H
#define JAZZ_BOOLEAN_ALGEBRA_LOGIC_GATES_H

#include "jazz/simulation/component.h"

#ifdef JAZZ_DEBUG
#define OVERRIDE_COMPUTE(classname)               \
    void compute() override {                     \
        printf("%s: %s\n", #classname, __func__); \
        Component::compute();                     \
    }
#else
#define OVERRIDE_COMPUTE(classname)
#endif

namespace jazz {
    namespace simulation {
        class Nand : public Component {
        public:
            Nand() : Component() {
                addIn("a", 1);
                addIn("b", 1);
                addOut("out", 1);
            };

            ~Nand() override = default;

            const char *name() const override {
                return "Nand";
            }

            OVERRIDE_COMPUTE(Nand)
        };

        class Invert : public Component {
        public:
            Invert() : Component() {
                auto a = addIn("a", 1);
                auto out = addOut("out", 1);
                auto nand = addComponent(new Nand());

                nand->connect(nand->in("a"), this, a);
                nand->connect(nand->in("b"), this, a);
                setOutput(out, nand, nand->out("out"));
            };

            ~Invert() override = default;

            const char *name() const override {
                return "Invert";
            }

            OVERRIDE_COMPUTE(Invert)
        };

        class And : public Component {
        public:
            And() : Component() {
                InOut *a = addIn("a", 1);
                InOut *b = addIn("b", 1);
                InOut *out = addOut("out", 1);

                auto nand = addComponent(new Nand());
                auto invert = addComponent(new Invert());

                nand->connect(nand->in("a"), this, a);
                nand->connect(nand->in("b"), this, b);
                invert->connect(invert->in("a"), nand, nand->out("out"));

                setOutput(out, invert, invert->out("out"));
            };

            ~And() override = default;

            const char *name() const override {
                return "And";
            }

            OVERRIDE_COMPUTE(And)
        };

        class Or : public Component {
        public:
            Or() : Component() {
                InOut *a = addIn("a", 1);
                InOut *b = addIn("b", 1);
                InOut *out = addOut("out", 1);

                auto invert_a = addComponent(new Invert());
                auto invert_b = addComponent(new Invert());
                auto nand = addComponent(new Nand());

                invert_a->connect(invert_a->in("a"), this, a);
                invert_b->connect(invert_b->in("a"), this, b);
                nand->connect(nand->in("a"), invert_a, invert_a->out("out"));
                nand->connect(nand->in("b"), invert_b, invert_b->out("out"));

                setOutput(out, nand, nand->out("out"));
            };

            ~Or() override = default;

            const char *name() const override {
                return "Or";
            }

            OVERRIDE_COMPUTE(Or)
        };

        class Xor : public Component {
        public:
            Xor() : Component() {
                InOut *a = addIn("a", 1);
                InOut *b = addIn("b", 1);
                InOut *out = addOut("out", 1);

                auto nand_0 = addComponent(new Nand());
                auto nand_1 = addComponent(new Nand());
                auto nand_2 = addComponent(new Nand());
                auto nand_3 = addComponent(new Nand());

                nand_0->connect(nand_0->in("a"), this, a);
                nand_0->connect(nand_0->in("b"), this, b);

                nand_1->connect(nand_1->in("a"), this, a);
                nand_1->connect(nand_1->in("b"), nand_0, nand_0->out("out"));

                nand_2->connect(nand_2->in("a"), this, b);
                nand_2->connect(nand_2->in("b"), nand_0, nand_0->out("out"));

                nand_3->connect(nand_3->in("a"), nand_1, nand_1->out("out"));
                nand_3->connect(nand_3->in("b"), nand_2, nand_2->out("out"));

                setOutput(out, nand_3, nand_3->out("out"));
            };

            ~Xor() override = default;

            const char *name() const override {
                return "Xor";
            }

            OVERRIDE_COMPUTE(Or)
        };
    }// namespace simulation
}// namespace jazz


#endif//JAZZ_BOOLEAN_ALGEBRA_LOGIC_GATES_H
