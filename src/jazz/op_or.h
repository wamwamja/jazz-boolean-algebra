/**
 * @brief Or is a class that represents the logical OR operation.
 * @file op_or.h
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

#ifndef BOOLEAN_ALGEBRA_OP_OR_H
#define BOOLEAN_ALGEBRA_OP_OR_H

#include "expr.h"
#include "print.h"

#include <vector>

namespace jazz {
    class Or : public Basic {
        JAZZ_DECLARE_REGISTERED_CLASS(Or, Basic);

    public:
        Or(const Expr &lhs, const Expr &rhs);
        unsigned precedence() const override { return 40; }
        bool isType(unsigned type_flag) const override;

        std::size_t numOperands() const override;
        Expr &operand(int i) override;
        const Expr &operand(int i) const override;
        Expr subs(const ExprMap &m, unsigned options) const override;
        bool isTrivial() const override;
        bool trivialValue() const override;

        Expr simplified() const override;

    protected:
        unsigned computeHash() const override;
        void opOr(const Expr &rhs);
        // p v p = p.
        void simplifyOrList();
        bool booleanIsTrue() const;

        void printOr(const jazz::PrintContext &context, const char *open_brace, const char *close_brace, const char *mul_symbol, unsigned level) const;
        void doPrint(const jazz::PrintContext &context, unsigned level) const;
        std::vector<Expr> subsChildren(const ExprMap &m, unsigned options = 0) const;
        void makeTrivialTrue();
        void addOperand(const Expr &expr);



    protected:
        Expr boolean;
        std::vector<Expr> operands;
    };

}// namespace jazz

#endif//BOOLEAN_ALGEBRA_OP_OR_H
