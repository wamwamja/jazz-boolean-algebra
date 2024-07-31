/**
 * @brief
 * @file relational.h
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

#ifndef BOOLEAN_ALGEBRA_RELATIONAL_H
#define BOOLEAN_ALGEBRA_RELATIONAL_H

#include "basic.h"
#include "expr.h"

namespace jazz {
    class Relational : public Basic {
        JAZZ_DECLARE_REGISTERED_CLASS(Relational, Basic);

    public:
        enum RelationalOp {
            EQUAL = 0,
            NOT_EQUAL,
            LESS,
            LESS_OR_EQUAL,
            GREATER,
            GREATER_OR_EQUAL
        };

    public:
        Relational(const Expr &lhs, const Expr &rhs, RelationalOp op);

    public:
        unsigned precedence() const override { return 20; }
        std::size_t numOperands() const override;
        const Expr &operand(int i) const override;
        Expr &operand(int i) override;
        Expr subs(const ExprMap &m, unsigned options) const override;
        //        Expr canonical() const;

        Expr left() const { return lhs; }
        Expr right() const { return rhs; }

        bool isType(unsigned type_flag) const override;

    protected:
        unsigned computeHash() const override;
        void doPrint(const jazz::PrintContext &c, unsigned level) const;

    protected:
        Expr lhs;
        Expr rhs;
        RelationalOp op;
    };
}// namespace jazz

#endif//BOOLEAN_ALGEBRA_RELATIONAL_H
