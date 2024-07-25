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
    namespace canonical {

        BooleanFunction &makeFunction(const char *name, const std::vector<Expr> &vars, const Expr &expr) {
            return create<BooleanFunction>(expr, vars, name);
        }

        Expr Constant_0(const char *x, const char *y) {
            return makeFunction("f", {Expr(x), Expr(y)}, false);
        }
        Expr False(const char *x, const char *y) {
            return Constant_0(x, y);
        }
        Expr And(const char *x, const char *y) {
            return Expr(x) & Expr(y);
        }
        Expr X_And_Not_Y(const char *x, const char *y) {
            return Expr(x) & !Expr(y);
        }
        Expr NonImplication(const char *x, const char *y) {
            return X_And_Not_Y(x, y);
        }
        Expr LeftProjection(const char *x, const char *y) {
            std::vector<Expr> vars = {Expr(x), Expr(y)};
            return makeFunction("f", vars, vars[0]);
        }
        Expr Not_X_And_Y(const char *x, const char *y) {
            return !Expr(x) & Expr(y);
        }
        Expr ConverseNonImplication(const char *x, const char *y) {
            return Not_X_And_Y(x, y);
        }
        Expr RightProjection(const char *x, const char *y) {
            std::vector<Expr> vars = {Expr(x), Expr(y)};
            return makeFunction("f", vars, vars[1]);
        }
        Expr Xor(const char *x, const char *y) {
            Expr ex(x), ey(y);
            return (ex & !ey) | (!ex & ey);
        }
        Expr Or(const char *x, const char *y) {
            return Expr(x) | Expr(y);
        }
        Expr Nor(const char *x, const char *y) {
            return !(Expr(x) | Expr(y));
        }
        Expr Equivalence(const char *x, const char *y) {
            Expr ex(x), ey(y);
            return (ex & ey) | (!ex & !ey);
        }
        Expr Not_Y(const char *x, const char *y) {
            std::vector<Expr> vars = {Expr(x), Expr(y)};
            return makeFunction("f", vars, !vars[1]);
        }
        Expr RightComplementation(const char *x, const char *y) {
            return Not_Y(x, y);
        }
        Expr X_Or_Not_Y(const char *x, const char *y) {
            return Expr(x) | !Expr(y);
        }
        Expr ConverseImplication(const char *x, const char *y) {
            return X_Or_Not_Y(x, y);
        }
        Expr Not_X(const char *x, const char *y) {
            std::vector<Expr> vars = {Expr(x), Expr(y)};
            return makeFunction("f", vars, !vars[0]);
        }
        Expr LeftComplementation(const char *x, const char *y) {
            return Not_X(x, y);
        }
        Expr Not_X_Or_Y(const char *x, const char *y) {
            return !Expr(x) | Expr(y);
        }
        Expr Implication(const char *x, const char *y) {
            return Not_X_Or_Y(x, y);
        }
        Expr Nand(const char *x, const char *y) {
            return !(Expr(x) & Expr(y));
        }
        Expr Constant_1(const char *x, const char *y) {
            return makeFunction("f", {Expr(x), Expr(y)}, true);
        }
        Expr True(const char *x, const char *y) {
            return Constant_1(x, y);
        }
    }// namespace canonical
}// namespace jazz
