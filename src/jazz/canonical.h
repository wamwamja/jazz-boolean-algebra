/**
 * @file canonical.h 
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

#ifndef JAZZ_BOOLEAN_ALGEBRA_CANONICAL_H
#define JAZZ_BOOLEAN_ALGEBRA_CANONICAL_H

namespace jazz {
    class Expr;

    /**
     * Canonical Boolean functions.
     */
    class Canonical {
    public:
        /// Boolean function 0000, f(x,y) = 0.
        static Expr Constant_0(const char *x = "x", const char *y = "y");
        /// Boolean function 0000, alias of Constant_0(), f(x,y) = 0.
        static Expr False(const char *x = "x", const char *y = "y");
        /// Boolean function 0001, f(x,y) = And(x,y).
        static Expr And(const char *x = "x", const char *y = "y");
        /// Boolean function 0010, f(x,y) = And(x,Not(y)).
        static Expr X_And_Not_Y(const char *x = "x", const char *y = "y");
        /// Boolean function 0010, alias of X_And_Not_Y(), f(x,y) = And(x,Not(y)).
        static Expr NonImplication(const char *x = "x", const char *y = "y");
        /// Boolean function 0011, f(x,y) = x.
        static Expr LeftProjection(const char *x = "x", const char *y = "y");
        /// Boolean function 0100, f(x,y) = And(Not(x),y).
        static Expr Not_X_And_Y(const char *x = "x", const char *y = "y");
        /// Boolean function 0100, alias of Not_X_And_Y(), f(x,y) = And(Not(x),y).
        static Expr ConverseNonImplication(const char *x = "x", const char *y = "y");
        /// Boolean function 0101, f(x,y) = y.
        static Expr RightProjection(const char *x = "x", const char *y = "y");
        /// Boolean function 0110, f(x,y) = Xor(x,y) = Or(And(x,Not(y)), And(Not(x),y)).
        static Expr Xor(const char *x = "x", const char *y = "y");
        /// Boolean function 0111, f(x,y) = Or(x,y).
        static Expr Or(const char *x = "x", const char *y = "y");
        /// Boolean function 1000, f(x,y) = Not(Or(x,y)).
        static Expr Nor(const char *x = "x", const char *y = "y");
        /// Boolean function 1001, f(x,y) = Or(And(x,y),And(Not(x),Not(y))).
        static Expr Equivalence(const char *x = "x", const char *y = "y");
        /// Boolean function 1010, f(x,y) = Not(y).
        static Expr Not_Y(const char *x = "x", const char *y = "y");
        /// Boolean function 1010, alias of Not_Y(), f(x,y) = Not(y).
        static Expr RightComplementation(const char *x = "x", const char *y = "y");
        /// Boolean function 1011, f(x,y) = Or(x,Not(y)).
        static Expr X_Or_Not_Y(const char *x = "x", const char *y = "y");
        /// Boolean function 1011, alias of X_Or_Not_Y(), f(x,y) = Or(x,Not(y)).
        static Expr ConverseImplication(const char *x = "x", const char *y = "y");
        /// Boolean function 1100, f(x,y) = Not(x).
        static Expr Not_X(const char *x = "x", const char *y = "y");
        /// Boolean function 1100, alias of Not_X(), f(x,y) = Not(x).
        static Expr LeftComplementation(const char *x = "x", const char *y = "y");
        /// Boolean function 1101, f(x,y) = Or(Not(x),y).
        static Expr Not_X_Or_Y(const char *x = "x", const char *y = "y");
        /// Boolean function 1101, alias of Not_X_Or_Y(), f(x,y) = Or(Not(x),y).
        static Expr Implication(const char *x = "x", const char *y = "y");
        /// Boolean function 1110, f(x,y) = Not(And(x,y)).
        static Expr Nand(const char *x = "x", const char *y = "y");
        /// Boolean function 1111, f(x,y) = 1.
        static Expr Constant_1(const char *x = "x", const char *y = "y");
        /// Boolean function 1111, alias of Constant_1(), f(x,y) = 1.
        static Expr True(const char *x = "x", const char *y = "y");
    };
}// namespace jazz

#endif//JAZZ_BOOLEAN_ALGEBRA_CANONICAL_H
