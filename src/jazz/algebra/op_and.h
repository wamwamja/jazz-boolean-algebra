/**
 * @file op_and.h
 *
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

#ifndef BOOLEAN_ALGEBRA_OP_AND_H
#define BOOLEAN_ALGEBRA_OP_AND_H

#include "basic.h"
#include "expr.h"
#include <vector>

namespace jazz {
    class And : public Basic {
        JAZZ_DECLARE_REGISTERED_CLASS(And, Basic);

    public:
        And(const Expr &lhs, const Expr &rhs);
        unsigned precedence() const override { return 50; }

        std::size_t numOperands() const override;
        Expr &operand(int i) override;
        const Expr &operand(int i) const override;
        bool isType(unsigned type_flag) const override;
        Expr subs(const ExprMap &m, unsigned options) const override;
        bool isTrivial() const override;
        bool trivialValue() const override;

    protected:
        bool booleanIsFalse() const;
        void opAnd(const Expr &rhs);
        void simplifyAndList();
        void makeTrivialFalse();
        std::vector<jazz::Expr> subsChildren(const ExprMap &m, unsigned options = 0) const;
        void doPrint(const jazz::PrintContext &context, unsigned level) const;
        void addOperand(const Expr &expr);

    protected:
        Expr boolean;
        std::vector<Expr> operands;
    };
}// namespace jazz

#endif//BOOLEAN_ALGEBRA_OP_AND_H
