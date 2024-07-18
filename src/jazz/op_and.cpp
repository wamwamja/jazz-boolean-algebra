/**
 * @file op_and.cpp
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

#include "op_and.h"
#include "boolean.h"
#include "op_not.h"
#include "op_or.h"
#include "utils.h"
#include <algorithm>

namespace jazz {
    JAZZ_IMPLEMENT_REGISTERED_CLASS_OPT(And, Basic, print_func<PrintContext>(&And::doPrint));
    JAZZ_IMPLEMENT_COMPARE_SAME_TYPE(And, other) {
        JAZZ_ASSERT(is_a<And>(other));
        auto this_op = operands;
        auto other_op = dynamic_cast<const And &>(other).operands;
        if (this_op.size() != other_op.size())
            return comparePointer(this, &other);

        std::sort(this_op.begin(), this_op.end(), ExprLess());
        std::sort(other_op.begin(), other_op.end(), ExprLess());
        auto n = this_op.size();
        for (int i = 0; i < n; ++i) {
            if (!this_op[i].isEqual(other_op[i]))
                return comparePointer(this, &other);
        }

        return 0;
    }
}// namespace jazz


bool jazz::And::booleanIsFalse() const {
    return expr_cast<Boolean>(boolean).isFalse();
}
void jazz::And::opAnd(const Expr &rhs) {
    if (booleanIsFalse()) {
        return;
    }

    if (is_a<And>(rhs)) {
        boolean = Expr(booleanIsFalse() && expr_cast<And>(rhs).booleanIsFalse());
        if (booleanIsFalse()) {
            operands.clear();
        } else {
            operands.insert(operands.end(), expr_cast<And>(rhs).operands.begin(), expr_cast<And>(rhs).operands.end());
            clearFlags(STATUS_FLAG_SIMPLIFIED);
        }
    } else if (is_exactly_a<Boolean>(rhs)) {
        if (expr_cast<Boolean>(rhs).isFalse()) {
            boolean = Expr(false);
            operands.clear();
        }
    } else {
        addOperand(rhs);
    }

    simplifyAndList();
}
jazz::And::And(const jazz::Expr &lhs, const jazz::Expr &rhs) {
    if (is_a<And>(lhs)) {
        boolean = expr_cast<And>(lhs).boolean;
        operands = expr_cast<And>(lhs).operands;
        opAnd(rhs);
    } else if (is_a<And>(rhs)) {
        boolean = expr_cast<And>(rhs).boolean;
        operands = (expr_cast<And>(rhs).operands);
        opAnd(lhs);
    } else {

        if (lhs.isTrivial() && rhs.isTrivial()) {
            boolean = Expr(lhs.trivialValue() && rhs.trivialValue());
        } else if (lhs.isTrivial()) {
            if (lhs.trivialValue()) {
                boolean = true;
                operands.push_back(rhs);
            } else {
                boolean = Expr(false);
            }
        } else if (rhs.isTrivial()) {
            if (rhs.trivialValue()) {
                boolean = true;
                operands.push_back(lhs);
            } else {
                boolean = Expr(false);
            }
        } else {
            boolean = Expr(true);
            operands.push_back(lhs);
            operands.push_back(rhs);
        }
    }

    simplifyAndList();
}
void jazz::And::doPrint(const jazz::PrintContext &context, unsigned int level) const {
    if (precedence() <= level)
        context.os << "(";

    if (isTrivial()) {
        context.os << (trivialValue() ? "1" : "0");
    } else {
        auto n_operands = operands.size();
        if (n_operands == 1) {
            // if there is only one operand, print it without braces
            operands[0].print(context, 0);
        } else {
            for (auto i = 0; i < n_operands; i++) {
                if (i > 0) {
                    context.os << " & ";
                }
                operands[i].print(context, precedence());
            }
        }
    }

    if (precedence() <= level)
        context.os << ")";
}
bool jazz::And::isType(unsigned int type_flag) const {
    return type_flag == TYPE_FLAG_AND;
}
std::size_t jazz::And::numOperands() const {
    if (booleanIsFalse()) {
        return 1;
    } else {
        return operands.size();
    }
}
jazz::Expr &jazz::And::operand(int i) {
    if (booleanIsFalse()) {
        if (i == 0)
            return boolean;
        else
            throw std::out_of_range("And::operand");
    } else {
        return operands[i];
    }
}
const jazz::Expr &jazz::And::operand(int i) const {
    return const_cast<And *>(this)->operand(i);
}

void jazz::And::simplifyAndList() {

    if (flags & STATUS_FLAG_SIMPLIFIED)
        return;
    setFlags(STATUS_FLAG_SIMPLIFIED);

    // simplify the operands
    for (auto &operand : operands) {
        operand = operand.simplified();
    }

    // handle duplicate operands: p & p = p
    std::sort(operands.begin(), operands.end(), ExprLess());
    auto last = std::unique(operands.begin(), operands.end(), ExprEqual());
    operands.erase(last, operands.end());


    // p & !p => 0
    for (auto &operand : operands) {
        if (is_exactly_a<Not>(operand)) {
            auto &not_op = expr_cast<Not>(operand);
            for (auto &op : operands) {
                if (op.isEqual(expr_cast<Not>(operand).expr)) {
                    makeTrivialFalse();
                    return;
                }
            }
        }
    }

    // handle trivial cases
    std::vector<Expr> new_operands;
    new_operands.reserve(operands.size());
    for (auto &operand : operands) {
        if (operand.isTrivial()) {
            if (!operand.trivialValue()) {
                makeTrivialFalse();
                return;
            }
        } else {
            new_operands.push_back(operand);
        }
    }

    operands = std::move(new_operands);
}

jazz::Expr jazz::And::subs(const jazz::ExprMap &m, unsigned int options) const {
    // if the boolean part is false, substitute false with the map.
    if (booleanIsFalse()) {
        return Expr(false).subs(m, options);
    }

    // if the boolean part is true,
    // - there are no operands, substitute true with the map.
    if (operands.empty()) {
        return Expr(true).subs(m, options);
    }

    // - there are operands, do substitution for each operand
    auto new_operands = subsChildren(m, options);
    if (!new_operands.empty()) {
        auto expr = duplicate();
        expr->operands.clear();
        for (const auto &new_operand : new_operands) {
            expr->opAnd(new_operand);
        }
        return *expr;
    } else {
        return *this;
    }
}

std::vector<jazz::Expr> jazz::And::subsChildren(const jazz::ExprMap &m, unsigned int options) const {
    std::vector<Expr> res;
    res.reserve(operands.size());
    for (const auto &operand : operands) {
        res.push_back(operand.subs(m, options));
    }
    return res;
}
bool jazz::And::isTrivial() const {
    return booleanIsFalse() || operands.empty();
}
bool jazz::And::trivialValue() const {
    if (isTrivial()) {
        return !booleanIsFalse();
    } else {
        throw std::runtime_error("And::trivialValue this object is not trivial.");
    }
}
void jazz::And::makeTrivialFalse() {
    boolean = false;
    operands.clear();
}
void jazz::And::addOperand(const jazz::Expr &expr) {
    operands.push_back(expr);
    clearFlags(STATUS_FLAG_SIMPLIFIED);
}
