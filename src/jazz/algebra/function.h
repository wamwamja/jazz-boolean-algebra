/**
 * @file function.h 
 * @author Jiazhen LUO
 * @date 2024/7/25
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

#ifndef JAZZ_BOOLEAN_ALGEBRA_FUNCTION_H
#define JAZZ_BOOLEAN_ALGEBRA_FUNCTION_H

#include "expr.h"
#include "print.h"

#include <utility>
#include <vector>

namespace jazz {
    class BooleanFunction : public Basic {
        JAZZ_DECLARE_REGISTERED_CLASS(BooleanFunction, Basic);

    public:
        explicit BooleanFunction(Expr e, const std::vector<Expr> &vars=std::vector<Expr>(), std::string name = "f");
        unsigned precedence() const override { return 70; }
        bool isType(unsigned type_flag) const override;

        std::size_t numOperands() const override;
        Expr &operand(int i) override;
        const Expr &operand(int i) const override;
        Expr subs(const ExprMap &m, unsigned options) const override;
        bool isTrivial() const override;
        bool trivialValue() const override;

        Expr simplified() const override;

        void printTruthTable() const;

        /**
         * Get the truth table of the function.
         *
         * For most cases, the truth table is a vector of determined boolean values.
         * But here we use expressions to represent the truth table, which can is more flexible.
         * For example,  f(x,y;z) = (x+y)z, the truth table is:
         * @verbatim
         * dec | x y | f(x,y,z)
         * -------------------
         *   0 | 0 0 | 0
         *   1 | 0 1 | z
         *   2 | 1 0 | z
         *   3 | 1 1 | z
         * @endverbatim
         * @return A vector of expressions representing the truth table.
         */
        std::vector<Expr> getTruthTable() const;

    protected:
        unsigned computeHash() const override;
        void doPrint(const jazz::PrintContext &context, unsigned level) const;

    protected:
        Expr expr;
        std::vector<Expr> variables;// Symbol
        std::string function_name = "f";
    };

}// namespace jazz

#endif//JAZZ_BOOLEAN_ALGEBRA_FUNCTION_H
