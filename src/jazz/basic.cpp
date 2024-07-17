/**
 * @file basic.cpp
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

#include "basic.h"
#include "expr.h"
#include "hash_seed.h"
#include "utils.h"
#include "wildcard.h"

namespace jazz {
    JAZZ_IMPLEMENT_REGISTERED_CLASS_OPT(Basic, void, print_func<PrintContext>(&Basic::printDelegate));
}

// Implicitly assumes that the other class is of the exact same type.
jazz::Basic::Basic(const jazz::Basic &other) : flags(other.flags & ~STATUS_FLAG_DYNAMIC_ALLOC), hash(other.hash) {
}
jazz::Basic &jazz::Basic::operator=(const jazz::Basic &other) {
    unsigned fl = other.flags & ~STATUS_FLAG_DYNAMIC_ALLOC;
    if (typeid(this) != typeid(&other)) {
        // other is a derived class
        fl &= ~STATUS_FLAG_HASH_CALCULATED;
    } else {
        hash = other.hash;
    }

    flags = fl;
    setRefCount(1);
    return *this;
}

int jazz::Basic::compareSameType(const jazz::Basic &other) const {
    // This default implementation should be overridden by derived classes.
    return comparePointer(this, &other);
}

bool jazz::Basic::isEqualSameType(const Basic &other) const {
    // This default implementation should be overridden by derived classes.
    return compareSameType(other) == 0;
}

unsigned int jazz::Basic::computeHash() const {
    hash = makeHashSeed(typeid(*this));
    for (int i = 0; i < numOperands(); ++i) {
        hash = rotateLeft(hash) ^ operand(i).hashValue();
    }
    if (flags & STATUS_FLAG_EVALUATED) {
        setFlags(STATUS_FLAG_HASH_CALCULATED);
    }
    return hash;
}
unsigned jazz::Basic::precedence() const {
    return 70;
}
jazz::Basic *jazz::Basic::duplicate() const {
    auto *copy = new Basic(*this);
    copy->flags = STATUS_FLAG_DYNAMIC_ALLOC;
    return copy;
}
void jazz::Basic::accept(jazz::Visitor &v) const {
    if (auto *p = dynamic_cast<Visitor *>(&v))
        p->visit(*this);
}
const jazz::Basic &jazz::Basic::setFlags(unsigned int flag) const {
    flags |= flag;
    return *this;
}
const jazz::Basic &jazz::Basic::clearFlags(unsigned int flag) const {
    flags &= ~flag;
    return *this;
}
const jazz::Basic &jazz::Basic::hold() const {
    return setFlags(STATUS_FLAG_EVALUATED);
}
unsigned jazz::Basic::hashValue() const {
    if (flags & STATUS_FLAG_HASH_CALCULATED) {
        return hash;
    } else {
        return computeHash();
    }
}
void jazz::Basic::printDispatch(const jazz::RegisteredClassHierarchy &class_hierarchy, const PrintContext &c, unsigned int level) const {
    auto pHierarchy = &class_hierarchy;
    auto pPrintContext = &c.getClassInfo();

next_class:
    auto pdt = pHierarchy->class_info.printDispatchTable();

next_context:
    auto id = pPrintContext->class_info.typeInfo();

    if (id >= pdt.size() || !(pdt[id].isValid())) {
        // Method not found, try parent print_context class
        auto parent_print = pPrintContext->getParent();
        if (parent_print) {
            pPrintContext = parent_print;
            goto next_context;
        }

        // Method still not found, try parent class
        auto parent_hierarchy = pHierarchy->getParent();
        if (parent_hierarchy) {
            pHierarchy = parent_hierarchy;
            pPrintContext = &c.getClassInfo();
            goto next_class;
        }

        // Method still not found. This shouldn't happen because basic (the
        // base class of the algebraic hierarchy) registers a method for
        // print_context (the base class of the print context hierarchy),
        // so if we end up here, there's something wrong with the class
        // registry.
        throw(std::runtime_error(std::string("basic::print(): method for ") + getClassName() + "/" + c.class_name() + " not found"));
    } else {
        // Call method
        pdt[id](*this, c, level);
    }
}
void jazz::Basic::print(const jazz::PrintContext &c, unsigned int level) const {
    printDispatch(getClassHierarchy(), c, level);
}
void jazz::Basic::debugPrint() const {
    print(PrintDefault(std::cerr), 0);
    std::cerr << std::endl;
}
void jazz::Basic::debugPrintTree() const {
    print(PrintTree(std::cerr), 0);
}
jazz::Expr jazz::Basic::subs(const jazz::ExprMap &m, unsigned int options) const {
    auto num = numOperands();
    for (int i = 0; i < num; ++i) {
        const Expr &e = operand(i);
        // it will stop recursion when numOperands() == 0.
        // so the substitution is done from the bottom up.
        const Expr &new_e = e.subs(m, options);
        if (!are_ex_trivially_equal(e, new_e)) {
            // something changed, clone the object
            auto *copy = duplicate();
            copy->clearFlags(STATUS_FLAG_HASH_CALCULATED | STATUS_FLAG_EXPANDED);
            copy->operand(i) = new_e;

            // substitute the rest of the operands
            ++i;
            for (; i < num; ++i) {
                copy->operand(i) = operand(i).subs(m, options);
            }

            // substitute the expression itself
            return copy->subsOneLevel(m, options);
        }
    }

    // nothing changed or no subexpressions.
    return subsOneLevel(m, options);
}

jazz::Expr jazz::Basic::map(const jazz::MapFunction &f) const {
    throw(std::runtime_error(std::string("Basic::map(): ") + getClassName() + std::string(" is not implemented")));
}

jazz::Expr jazz::Basic::eval() const {
    throw(std::runtime_error(std::string("Basic::eval(): ") + getClassName() + std::string(" is not implemented")));
}

void jazz::Basic::printDelegate(const jazz::PrintContext &c, unsigned int level) const {
    c.os << "[" << getClassName() << " object]";
}

std::size_t jazz::Basic::numOperands() const {
    return 0;
}

jazz::Expr jazz::Basic::subsOneLevel(const jazz::ExprMap &m, unsigned int options) const {
    if (options & SUBS_OPTION_NO_PATTERN) {
        // no pattern matching, to test the expressions is in the map and replace it.
        Expr expr = *this;
        auto found = m.find(expr);
        if (found != m.end()) {
            // found in the map, return the replacement.
            return found->second;
        } else {
            // not in the map, return the original expression.
            return expr;
        }
    } else {
        for (auto &e : m) {
            ExprMap replace_list;
            // if the expression matches a pattern, replace it.
            if (match(e.first, replace_list)) {
                return e.second.subs(replace_list, options | SUBS_OPTION_NO_PATTERN);
                // avoid infinite recursion when re-substituting the wildcards
            }
        }
    }

    return *this;
}

int jazz::Basic::compare(const jazz::Basic &other) const {
    auto hash_this = hashValue();
    auto hash_other = other.hashValue();

    if (hash_this < hash_other) return -1;
    if (hash_this > hash_other) return 1;

    auto &typeid_this = typeid(*this);
    auto &typeid_other = typeid(other);

    if (typeid_this == typeid_other) {
        return compareSameType(other);
    } else {
        return typeid_this.before(typeid_other) ? -1 : 1;
    }
}

const jazz::Expr &jazz::Basic::operand(int i) const {
    throw(std::range_error(std::string("Basic::operand(): ") + getClassName() + std::string(" has no operands")));
}

jazz::Expr &jazz::Basic::operand(int i) {
    ensureIfModifiable();
    throw(std::range_error(std::string("Basic::operand(): ") + getClassName() + std::string(" has no operands")));
}

bool jazz::Basic::has(const jazz::Expr &pattern, unsigned int options) const {
    ExprMap map;
    if (match(pattern, map)) {
        return true;
    }

    for (int i = 0; i < numOperands(); ++i) {
        if (operand(i).has(pattern, options)) {
            return true;
        }
    }

    return false;
}

bool jazz::Basic::match(const jazz::Expr &pattern, jazz::ExprMap &replace_list) const {
    if (is_exactly_a<Wildcard>(pattern)) {
        // Wildcard matches anything, but check whether we have already found a match
        // for that wildcard before.
        for (auto &e : replace_list) {
            if (e.first.isEqual(pattern)) {
                return isEqual(expr_cast<Basic>(e.second));
            }
        }

        replace_list[pattern] = *this;
        return true;
    } else {
        // 1. they are of the same type
        auto &other = expr_cast<Basic>(pattern);
        if (typeid(*this) != typeid(other)) {
            return false;
        }

        // 2. they have the same number of operands
        if (numOperands() != pattern.numOperands()) {
            return false;
        }

        if (numOperands() == 0) {
            // atomic
            return isEqualSameType(expr_cast<Basic>(pattern));
        }

        // if(!matchSameType()) @TODO what is this?


        // 3. match each subexpressions.
        ExprMap temp_replace_list;
        for (int i = 0; i < numOperands(); ++i) {
            if (!operand(i).match(pattern.operand(i), temp_replace_list)) {
                return false;
            }
        }

        replace_list = temp_replace_list;
        return true;
    }
}
bool jazz::Basic::isEqual(const jazz::Basic &other) const {
    if (isTrivial() && other.isTrivial())
        return trivialValue() == other.trivialValue();
    else
        return (hashValue() == other.hashValue()) && (typeid(*this) == typeid(other)) && isEqualSameType(other);
}
void jazz::Basic::ensureIfModifiable() const {
    if (refCount() > 1)
        throw std::runtime_error("Basic::ensureIfModifiable(): object is shared so can not be modified");
    clearFlags(STATUS_FLAG_HASH_CALCULATED | STATUS_FLAG_EVALUATED);
}

bool jazz::Basic::isType(unsigned int info) const {
    return false;
}
bool jazz::Basic::isTrivial() const {
    return false;
}
bool jazz::Basic::trivialValue() const {
    throw std::runtime_error("Basic::trivialValue(): not trivial");
}
jazz::Expr jazz::Basic::simplified() const {
    if (isTrivial()) {
        return trivialValue();
    } else {
        return *this;
    }
}
