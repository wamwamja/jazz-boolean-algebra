/**
 * @brief
 * @file operations.h
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

#ifndef BOOLEAN_ALGEBRA_OPERATORS_H
#define BOOLEAN_ALGEBRA_OPERATORS_H

#include <iosfwd>

namespace jazz {
    class Expr;

    Expr operator&(const Expr &lhs, const Expr &rhs);
    Expr operator|(const Expr &lhs, const Expr &rhs);
    Expr operator+(const Expr &lhs, const Expr &rhs);
    Expr operator*(const Expr &lhs, const Expr &rhs);
    Expr operator!(const Expr &expr);

    // Relational operators
    Expr operator==(const Expr &lhs, const Expr &rhs);
    Expr operator!=(const Expr &lhs, const Expr &rhs);
    Expr operator<(const Expr &lhs, const Expr &rhs);
    Expr operator<=(const Expr &lhs, const Expr &rhs);
    Expr operator>(const Expr &lhs, const Expr &rhs);
    Expr operator>=(const Expr &lhs, const Expr &rhs);

    std::ostream &operator<<(std::ostream &os, const Expr &e);
}// namespace jazz

#endif//BOOLEAN_ALGEBRA_OPERATORS_H
