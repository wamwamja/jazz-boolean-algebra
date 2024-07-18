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

#include "op_or.h"
#include "boolean.h"
#include "op_not.h"
#include "utils.h"
#include <algorithm>


namespace jazz {
    JAZZ_IMPLEMENT_REGISTERED_CLASS_OPT(Or, Basic, print_func<PrintContext>(&Or::doPrint));
    JAZZ_IMPLEMENT_COMPARE_SAME_TYPE(Or, other) {
        JAZZ_ASSERT(is_a<Or>(other));
        auto this_op = operands;
        auto other_op = dynamic_cast<const Or &>(other).operands;
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


jazz::Or::Or(const jazz::Expr &lhs, const jazz::Expr &rhs) {
    if (is_a<Or>(lhs)) {
        boolean = expr_cast<Or>(lhs).boolean;
        operands = expr_cast<Or>(lhs).operands;
        opOr(rhs);
    } else if (is_a<Or>(lhs)) {
        boolean = expr_cast<Or>(rhs).boolean;
        operands = (expr_cast<Or>(rhs).operands);
        opOr(lhs);
    } else if (lhs.isTrivial() || rhs.isTrivial()) {
        // handle trivial cases
        if (lhs.isTrivial() && rhs.isTrivial()) {
            boolean = lhs.trivialValue() || rhs.trivialValue();
        } else if (lhs.isTrivial()) {
            if (lhs.trivialValue()) {
                boolean = true;
            } else {
                boolean = false;
                operands.push_back(rhs);
            }
        } else if (rhs.isTrivial()) {
            if (rhs.trivialValue()) {
                boolean = true;
            } else {
                boolean = false;
                operands.push_back(lhs);
            }
        }
    } else {
        operands.push_back(lhs);
        operands.push_back(rhs);
    }

    simplifyOrList();
}
void jazz::Or::opOr(const Expr &rhs) {
    if (booleanIsTrue())
        return;

    if (is_a<Or>(rhs)) {
        boolean = Expr(booleanIsTrue() || expr_cast<Or>(rhs).booleanIsTrue());
        if (booleanIsTrue()) {
            operands.clear();
        } else {
            operands.insert(operands.end(), expr_cast<Or>(rhs).operands.begin(), expr_cast<Or>(rhs).operands.end());
            clearFlags(STATUS_FLAG_SIMPLIFIED);
        }
    } else if (is_exactly_a<Boolean>(rhs)) {
        if (expr_cast<Boolean>(rhs).isTrue()) {
            boolean = Expr(true);
            operands.clear();
        } else {
            // p V false = p, do nothing
        }
    } else {
        addOperand(rhs);
    }
    simplifyOrList();
}
bool jazz::Or::booleanIsTrue() const {
    return expr_cast<Boolean>(boolean).isTrue();
}
void jazz::Or::printOr(const jazz::PrintContext &context, const char *open_brace, const char *close_brace, const char *mul_symbol, unsigned int level) const {
    if (precedence() <= level)
        context.os << open_brace << '(';
    if (isTrivial()) {
        context.os << (trivialValue() ? "1" : "0");
    } else {
        if (operands.size() == 1) {
            operands[0].print(context, 0);
        } else {
            bool is_first = true;
            for (const auto &operand : operands) {
                if (!is_first) {
                    context.os << " | ";
                } else
                    is_first = false;
                operand.print(context, precedence());
            }
        }
    }
    if (precedence() <= level)
        context.os << ')' << close_brace;
}
void jazz::Or::doPrint(const jazz::PrintContext &context, unsigned int level) const {
    printOr(context, "", "", " & ", level);
}
unsigned jazz::Or::computeHash() const {
    return Basic::computeHash();
}
bool jazz::Or::isType(unsigned int type_flag) const {
    return type_flag == TYPE_FLAG_OR;
}
std::size_t jazz::Or::numOperands() const {
    if (booleanIsTrue()) {
        return 1;
    } else
        return operands.size();
}
jazz::Expr &jazz::Or::operand(int i) {
    if (booleanIsTrue()) {
        if (i == 0)
            return boolean;
        else {}
        throw std::out_of_range("Or::operand");
    } else {
        return operands[i];
    }
}
const jazz::Expr &jazz::Or::operand(int i) const {
    return const_cast<Or *>(this)->operand(i);
}
jazz::Expr jazz::Or::subs(const jazz::ExprMap &m, unsigned int options) const {

    // if the boolean part is true, substitute true with the map.
    if (booleanIsTrue()) {
        return Expr(true).subs(m, options);
    }

    // if the boolean part is false,
    // - there is no operands, substitute false with the map.
    if (operands.empty()) {
        return Expr(false).subs(m, options);
    }

    // - there are operands, do substitution for each operand
    auto new_operands = subsChildren(m, options);
    if (!new_operands.empty()) {
        auto expr = duplicate();
        expr->operands.clear();
        for (const auto &new_operand : new_operands) {
            expr->opOr(new_operand);
        }
        return expr->simplified();
    } else {
        return *this;
    }
}

std::vector<jazz::Expr> jazz::Or::subsChildren(const jazz::ExprMap &m, unsigned int options) const {
    std::vector<Expr> res;
    res.reserve(operands.size());
    for (const auto &operand : operands) {
        res.push_back(operand.subs(m, options));
    }
    return res;
}

void jazz::Or::simplifyOrList() {

    if (flags & STATUS_FLAG_SIMPLIFIED)
        return;

    setFlags(STATUS_FLAG_SIMPLIFIED);

    // simplify each operand
    for (auto &operand : operands) {
        operand = operand.simplified();
    }

    // handle duplicate operands: p | p = p
    std::sort(operands.begin(), operands.end(), ExprLess());
    auto last = std::unique(operands.begin(), operands.end(), ExprEqual());
    operands.erase(last, operands.end());

    // p | !p => 1
    for (auto &operand : operands) {
        if (is_exactly_a<Not>(operand)) {
            auto &not_op = expr_cast<Not>(operand);
            for (auto &op : operands) {
                if (op.isEqual(expr_cast<Not>(operand).expr)) {
                    makeTrivialTrue();
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
            if (operand.trivialValue()) {
                makeTrivialTrue();
                return;
            } else {
                continue;
            }
        } else {
            new_operands.push_back(operand);
        }
    }

    operands = std::move(new_operands);
}
bool jazz::Or::isTrivial() const {
    return booleanIsTrue() || operands.empty();
}
bool jazz::Or::trivialValue() const {
    if (isTrivial()) {
        return booleanIsTrue();
    } else {
        throw std::runtime_error("Or::trivialValue this object is not trivial.");
    }
}
void jazz::Or::makeTrivialTrue() {
    boolean = true;
    operands.clear();
}
void jazz::Or::addOperand(const jazz::Expr &expr) {
    operands.push_back(expr);
    clearFlags(STATUS_FLAG_SIMPLIFIED);
}
jazz::Expr jazz::Or::simplified() const {
    if (isTrivial()) {
        return trivialValue();
    } else {
        // not trivial so that boolean is false.
        if (operands.size() == 1) {
            return operands[0];
        } else {
            return *this;
        }
    }
}
