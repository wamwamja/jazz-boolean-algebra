/**
 * @file op_not.cpp
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

#include "op_not.h"
#include "flags.h"
#include "utils.h"

namespace jazz {
    JAZZ_IMPLEMENT_REGISTERED_CLASS_OPT(Not, Basic, print_func<PrintContext>(&Not::doPrint));
    JAZZ_IMPLEMENT_COMPARE_SAME_TYPE(Not, other) {
        auto &o = expr_cast<Not>(other);
        if (not_flag == o.not_flag && expr.isEqual(o.expr)) {
            return 0;
        } else {
            return hashValue() < other.hashValue() ? -1 : 1;
        }
    }
}// namespace jazz

bool jazz::Not::isType(unsigned int type_flag) const {
    return type_flag == TYPE_FLAG_NOT;
}
std::size_t jazz::Not::numOperands() const {
    return 1;
}
jazz::Expr &jazz::Not::operand(int i) {
    JAZZ_ASSERT(i == 0);
    return expr;
}
const jazz::Expr &jazz::Not::operand(int i) const {
    return expr;
}
jazz::Expr jazz::Not::subs(const jazz::ExprMap &m, unsigned int options) const {
    if (not_flag) {
        return create<Not>(expr.subs(m, options));
    }
    else{
        return expr.subs(m, options);
    }
}
bool jazz::Not::isTrivial() const {
    return expr.isTrivial();
}
bool jazz::Not::trivialValue() const {
    return not_flag ? !expr.trivialValue() : expr.trivialValue();
}
unsigned jazz::Not::precedence() const {
    // always with braces
    return 70;
}
jazz::Not::Not(const jazz::Expr &ex) {
    if (ex.isTrivial()) {
        not_flag = false;
        expr = !ex.trivialValue();
    } else if (is_exactly_a<Not>(ex)) {
        auto &other_not = expr_cast<Not>(ex);
        not_flag = !other_not.not_flag;
        expr = other_not.expr;
    } else {
        expr = ex;
        not_flag = true;
    }
}
unsigned jazz::Not::computeHash() const {
    Basic::computeHash();
    if (not_flag) {
        hash = rotateLeft(hash) ^ 0x9e3779b9;
    }
    return hash;
}
void jazz::Not::doPrint(const jazz::PrintContext &context, unsigned int level) const {
    if (precedence() <= level)
        context.os << "(";

    if (not_flag) context.os << "!";
    expr.print(context, precedence());

    if (precedence() <= level)
        context.os << ")";
}
jazz::Expr jazz::Not::simplified() const {
    if (expr.isTrivial()) {
        return trivialValue();
    } else {
        auto simplified_expr = expr.simplified();
        if (is_exactly_a<Not>(simplified_expr)) {
            bool other_not_flag = expr_cast<Not>(simplified_expr).not_flag;
            bool flag = not_flag ^ other_not_flag;
            if (flag) {
                return expr_cast<Not>(simplified_expr).expr;
            } else {
                return create<Not>(expr_cast<Not>(simplified_expr).expr);
            }
        } else {
            if (not_flag) {
                return create<Not>(simplified_expr);
            }else
                return simplified_expr;
        }
    }
}
bool jazz::Not::notFlag() const {
    return not_flag;
}
