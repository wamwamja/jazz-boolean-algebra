/**
 * @file expr.cpp
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

#include "expr.h"
#include "boolean.h"
#include "symbol.h"

jazz::Expr::Expr(bool v) : ptr(v ? Ptr(Boolean::True()) : Ptr(Boolean::False())) {
}

jazz::Expr::Expr(const char *name) : ptr(Ptr<Basic>(create<Symbol>(name))) {}

void jazz::Expr::print(const jazz::PrintContext &context, unsigned int level) const {
    ptr->print(context, level);
}
void jazz::Expr::debugPrint() const {
    ptr->debugPrint();
}
void jazz::Expr::debugPrintTree() const {
    ptr->debugPrint();
}
void jazz::Expr::share(const jazz::Expr &other) const {
    if ((ptr->flags | other.ptr->flags) & STATUS_FLAG_NOT_SHAREABLE)
        return;

    if (ptr->refCount() <= other.ptr->refCount())
        ptr = other.ptr;
    else
        other.ptr = ptr;
}

int jazz::Expr::compare(const jazz::Expr &other) const {
    if (ptr == other.ptr)
        return 0;

    auto cmp = ptr->compare(*other.ptr);
    if (cmp == 0) {
        share(other);
    }

    return cmp;
}

bool jazz::ExprLess::operator()(const jazz::Expr &e1, const jazz::Expr &e2) const {
    return e1.compare(e2) < 0;
}

jazz::Expr jazz::Expr::subs(const Expr &expr, unsigned int options) const {
    if (expr.isType(TYPE_FLAG_RELATIONAL_EQUAL)) {
        // argument is a relational equal, convert it to a map
        ExprMap map;
        map[expr.operand(0)] = expr.operand(1);
        return ptr->subs(map, options);
    }

    throw std::invalid_argument("Expr::subs: argument must be a relational equal expression.");
}
bool jazz::ExprEqual::operator()(const jazz::Expr &e1, const jazz::Expr &e2) const {
    return e1.compare(e2) == 0;
}
unsigned jazz::ExprHash::operator()(const jazz::Expr &e) const {
    return e.hashValue();
}
