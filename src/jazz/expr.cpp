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
#include "operations.h"
#include "symbol.h"

#include <string>
#include <unordered_set>

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

namespace jazz {

    std::vector<Expr> getExprVars(const Expr &expr) {
        std::vector<Expr> vars;
        std::unordered_set<Expr, ExprHash, ExprEqual> var_set;

        if (!expr.isTrivial()) {
            auto num = expr.numOperands();
            for (size_t i = 0; i < num; i++) {
                const auto &op = expr.operand(i);
                if (op.isType(TYPE_FLAG_SYMBOL)) {
                    if (var_set.insert(op).second) {
                        vars.push_back(op);
                    }
                } else {
                    auto vars2 = op.getVars(0);
                    for (auto &v : vars2) {
                        if (var_set.insert(v).second) {
                            vars.push_back(v);
                        }
                    }
                }
            }
        }

        return vars;
    }

    std::vector<Expr> Expr::getVars(int sort_flag) const {
        auto vars = getExprVars(*this);
        if (sort_flag == SYMBOL_LIST_SORT_NAME_ASC) {
            auto cmp = [](const Expr &a, const Expr &b) {
                JAZZ_ASSERT(a.isType(TYPE_FLAG_SYMBOL) && b.isType(TYPE_FLAG_SYMBOL));
                return expr_cast<Symbol>(a).getName() < expr_cast<Symbol>(b).getName();
            };
            std::sort(vars.begin(), vars.end(), cmp);
        }
        return vars;
    }

    void Expr::printTruthTable() const {
        auto vars = getVars();
        auto num_vars = vars.size();

        std::vector<std::size_t> name_len(num_vars);
        std::size_t name_len_total = 0;
        for (size_t i = 0; i < num_vars; i++) {
            name_len[i] = expr_cast<Symbol>(vars[i]).getName().size() + 2;
            name_len_total += name_len[i];
        }

        // print header
        char output[] = "v";
        std::cout << " ** Truth table for " << output << " = " << *this << " **" << std::endl;
        std::cout << std::string(name_len_total + num_vars + std::strlen(output) + 2, '-') << std::endl;
        for (size_t i = 0; i < num_vars; i++) {
            printf(" %s |", expr_cast<Symbol>(vars[i]).getName().c_str());
        }
        std::cout << " " << output << " " << std::endl;
        std::cout << std::string(name_len_total + num_vars + std::strlen(output) + 2, '-') << std::endl;

        // print truth table
        if (num_vars < sizeof(unsigned long) * 8) {
            unsigned long v = 0;
            unsigned long max = (1 << num_vars);

            while (v < max) {
                Expr eval_expr = *this;
                for (size_t i = 0; i < num_vars; i++) {
                    bool bit = v & (1 << (num_vars - i - 1));
                    eval_expr = eval_expr.subs(vars[i] == bit);
                    printf(" %s |", bit ? "1" : "0");
                }

                if (!eval_expr.isTrivial()) {
                    std::cerr << "internal error: truth table evaluation failed" << std::endl;
                    return;
                }

                printf(" %s\n", eval_expr.trivialValue() ? "1" : "0");
                v++;
            }
            std::cout << std::string(name_len_total + num_vars + std::strlen(output) + 2, '-') << std::endl;
        } else {
            std::cerr << "truth table is too large to print" << std::endl;
            return;
        }
    }
}// namespace jazz
