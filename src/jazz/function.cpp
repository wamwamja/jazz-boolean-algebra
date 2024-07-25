/**
 * @file function.cpp 
 * @author Jiazhen LUO
 * @date 2024/7/25
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

#include "function.h"
#include "boolean.h"
#include "flags.h"
#include "operations.h"
#include "symbol.h"
#include "utils.h"

namespace jazz {
    JAZZ_IMPLEMENT_REGISTERED_CLASS_OPT(BooleanFunction, Basic, print_func<PrintContext>(&BooleanFunction::doPrint));
    JAZZ_IMPLEMENT_COMPARE_SAME_TYPE(BooleanFunction, other) {
        JAZZ_ASSERT(is_a<BooleanFunction>(other));
        return comparePointer(this, &other);
    }
}// namespace jazz

bool jazz::BooleanFunction::isType(unsigned int type_flag) const {
    return type_flag == TYPE_FLAG_BOOLEAN_FUNCTION;
}

std::size_t jazz::BooleanFunction::numOperands() const {
    return 1;
}
jazz::Expr &jazz::BooleanFunction::operand(int i) {
    return expr;
}
const jazz::Expr &jazz::BooleanFunction::operand(int i) const {
    return expr;
}
jazz::Expr jazz::BooleanFunction::subs(const jazz::ExprMap &m, unsigned int options) const {
    throw std::runtime_error("Substitution not supported for BooleanFunction.");
}
bool jazz::BooleanFunction::isTrivial() const {
    return expr.isTrivial();
}
bool jazz::BooleanFunction::trivialValue() const {
    return expr.trivialValue();
}
jazz::Expr jazz::BooleanFunction::simplified() const {
    auto copy = duplicate();
    copy->expr = copy->expr.simplified();
    return copy;
}
unsigned jazz::BooleanFunction::computeHash() const {
    return Basic::computeHash();
}
void jazz::BooleanFunction::doPrint(const jazz::PrintContext &context, unsigned int level) const {
    std::cout << function_name << "(";
    auto num_vars = variables.size();
    for (std::size_t i = 0; i < num_vars; ++i) {
        variables[i].print(context, level);
        if (i < num_vars - 1) {
            std::cout << ", ";
        }
    }
    std::cout << ") = ";
}
jazz::BooleanFunction::BooleanFunction(jazz::Expr e, const std::vector<Expr> &vars, std::string name)
    : expr(std::move(e)), variables(vars), function_name(std::move(name)) {
    if (variables.size() < 2) {
        auto vs = expr.getVars();
        if (vs.size() < 2)
            throw std::runtime_error("BooleanFunction must have at least two variables.");
        variables = vs;
    }

    if (function_name.empty())
        function_name += "f";
}
void jazz::BooleanFunction::printTruthTable() const {
    auto num_vars = variables.size();

    std::vector<std::size_t> name_len(num_vars);
    std::size_t name_len_total = 0;
    for (size_t i = 0; i < num_vars; i++) {
        name_len[i] = expr_cast<Symbol>(variables[i]).getName().size() + 2;
        name_len_total += name_len[i];
    }

    // print header
    std::cout << " ** Truth table for " << function_name << "(";
    for (size_t i = 0; i < num_vars; i++) {
        printf("%s", expr_cast<Symbol>(variables[i]).getName().c_str());
        if (i < num_vars - 1) {
            std::cout << ", ";
        }
    }
    std::cout << ") = " << expr << " **" << std::endl;
    std::cout << std::string(name_len_total + num_vars + function_name.size() + 2, '-') << std::endl;
    for (size_t i = 0; i < num_vars; i++) {
        printf(" %s |", expr_cast<Symbol>(variables[i]).getName().c_str());
    }
    std::cout << " " << function_name << " " << std::endl;
    std::cout << std::string(name_len_total + num_vars + function_name.size() + 2, '-') << std::endl;

    // print truth table
    if (num_vars < sizeof(unsigned long) * 8) {
        unsigned long v = 0;
        unsigned long max = (1 << num_vars);

        while (v < max) {
            Expr eval_expr = duplicate()->expr;
            for (size_t i = 0; i < num_vars; i++) {
                bool bit = v & (1 << (num_vars - i - 1));
                eval_expr = eval_expr.subs(variables[i] == bit);
                printf(" %s |", bit ? "1" : "0");
            }
            std::cout << " " << eval_expr.simplified() << std::endl;
            v++;
        }
        std::cout << std::string(name_len_total + num_vars + function_name.size() + 2, '-') << std::endl;
    } else {
        std::cerr << "truth table is too large to print" << std::endl;
        return;
    }
}
