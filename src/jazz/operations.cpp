
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

#include "operations.h"
#include "op_and.h"
#include "op_not.h"
#include "op_or.h"
#include "relational.h"


static int my_ios_index() {
    static int i = std::ios_base::xalloc();
    return i;
}

static inline jazz::PrintContext *get_print_context(std::ios_base &s) {
    return static_cast<jazz::PrintContext *>(s.pword(my_ios_index()));
}

namespace jazz {
    inline Expr exOr(const Expr &lhs, const Expr &rhs) {
        return create<Or>(lhs, rhs);
    }

    inline Expr exAnd(const Expr &lhs, const Expr &rhs) {
        return create<And>(lhs, rhs);
    }

    Expr operator&(const Expr &lhs, const Expr &rhs) {
        if (lhs.isEqual(rhs))
            return lhs;
        else
            return exAnd(lhs, rhs);
    }
    Expr operator|(const Expr &lhs, const Expr &rhs) {
        if (lhs.isEqual(rhs))
            return lhs;
        else if (lhs.isTrivial()) {
            if (lhs.trivialValue()) {
                return true;
            } else {
                return rhs;
            }
        } else if (rhs.isTrivial()) {
            if (rhs.trivialValue()) {
                return true;
            } else {
                return lhs;
            }
        }

        return exOr(lhs, rhs);
    }

    Expr operator*(const Expr &lhs, const Expr &rhs) {
        return lhs & rhs;
    }
    Expr operator+(const Expr &lhs, const Expr &rhs) {
        return lhs | rhs;
    }
    Expr operator!(const Expr &expr) {
        if (expr.isTrivial())
            return !expr.isTrivial();
        else if (is_exactly_a<Not>(expr)) {
            auto &n_expr = expr_cast<Not>(expr);
            if (n_expr.notFlag()) {
                return n_expr.operand(0);
            } else {
                return create<Not>(n_expr.operand(0));
            }
        }
        return create<Not>(expr);
    }

    std::ostream &operator<<(std::ostream &os, const Expr &e) {
        PrintContext *context = get_print_context(os);
        if (context == nullptr)
            e.print(PrintDefault(os));
        else
            e.print(*context);
        return os;
    }
    Expr operator==(const Expr &lhs, const Expr &rhs) {
        return create<Relational>(lhs, rhs, Relational::EQUAL);
    }
    Expr operator!=(const Expr &lhs, const Expr &rhs) {
        return create<Relational>(lhs, rhs, Relational::NOT_EQUAL);
    }
    Expr operator<(const Expr &lhs, const Expr &rhs) {
        return create<Relational>(lhs, rhs, Relational::LESS);
    }
    Expr operator<=(const Expr &lhs, const Expr &rhs) {
        return create<Relational>(lhs, rhs, Relational::LESS_OR_EQUAL);
    }
    Expr operator>(const Expr &lhs, const Expr &rhs) {
        return create<Relational>(lhs, rhs, Relational::GREATER);
    }
    Expr operator>=(const Expr &lhs, const Expr &rhs) {
        return create<Relational>(lhs, rhs, Relational::GREATER_OR_EQUAL);
    }
}// namespace jazz
