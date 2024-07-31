/**
 * @file canonical.cpp 
 * @author Jiazhen LUO
 * @date 2024/7/24
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

#include "canonical.h"
#include "expr.h"
#include "function.h"
#include "operations.h"

namespace jazz {

    BooleanFunction &makeFunction(const char *name, const std::vector<Expr> &vars, const Expr &expr) {
        return create<BooleanFunction>(expr, vars, name);
    }


    Expr Canonical::Constant_0(const char *x, const char *y) {
        return makeFunction("f", {Expr(x), Expr(y)}, false);
    }
    Expr Canonical::False(const char *x, const char *y) {
        return Constant_0(x, y);
    }
    Expr Canonical::And(const char *x, const char *y) {
        return Expr(x) & Expr(y);
    }
    Expr Canonical::X_And_Not_Y(const char *x, const char *y) {
        return Expr(x) & !Expr(y);
    }
    Expr Canonical::NonImplication(const char *x, const char *y) {
        return X_And_Not_Y(x, y);
    }
    Expr Canonical::LeftProjection(const char *x, const char *y) {
        std::vector<Expr> vars = {Expr(x), Expr(y)};
        return makeFunction("f", vars, vars[0]);
    }
    Expr Canonical::Not_X_And_Y(const char *x, const char *y) {
        return !Expr(x) & Expr(y);
    }
    Expr Canonical::ConverseNonImplication(const char *x, const char *y) {
        return Not_X_And_Y(x, y);
    }
    Expr Canonical::RightProjection(const char *x, const char *y) {
        std::vector<Expr> vars = {Expr(x), Expr(y)};
        return makeFunction("f", vars, vars[1]);
    }
    Expr Canonical::Xor(const char *x, const char *y) {
        Expr ex(x), ey(y);
        return (ex & !ey) | (!ex & ey);
    }
    Expr Canonical::Or(const char *x, const char *y) {
        return Expr(x) | Expr(y);
    }
    Expr Canonical::Nor(const char *x, const char *y) {
        return !(Expr(x) | Expr(y));
    }
    Expr Canonical::Equivalence(const char *x, const char *y) {
        Expr ex(x), ey(y);
        return (ex & ey) | (!ex & !ey);
    }
    Expr Canonical::Not_Y(const char *x, const char *y) {
        std::vector<Expr> vars = {Expr(x), Expr(y)};
        return makeFunction("f", vars, !vars[1]);
    }
    Expr Canonical::RightComplementation(const char *x, const char *y) {
        return Not_Y(x, y);
    }
    Expr Canonical::X_Or_Not_Y(const char *x, const char *y) {
        return Expr(x) | !Expr(y);
    }
    Expr Canonical::ConverseImplication(const char *x, const char *y) {
        return X_Or_Not_Y(x, y);
    }
    Expr Canonical::Not_X(const char *x, const char *y) {
        std::vector<Expr> vars = {Expr(x), Expr(y)};
        return makeFunction("f", vars, !vars[0]);
    }
    Expr Canonical::LeftComplementation(const char *x, const char *y) {
        return Not_X(x, y);
    }
    Expr Canonical::Not_X_Or_Y(const char *x, const char *y) {
        return !Expr(x) | Expr(y);
    }
    Expr Canonical::Implication(const char *x, const char *y) {
        return Not_X_Or_Y(x, y);
    }
    Expr Canonical::Nand(const char *x, const char *y) {
        return !(Expr(x) & Expr(y));
    }
    Expr Canonical::Constant_1(const char *x, const char *y) {
        return makeFunction("f", {Expr(x), Expr(y)}, true);
    }
    Expr Canonical::True(const char *x, const char *y) {
        return Constant_1(x, y);
    }
}// namespace jazz
