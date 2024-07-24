/**
 * @file expr.h
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

#ifndef BOOLEAN_ALGEBRA_EXPR_H
#define BOOLEAN_ALGEBRA_EXPR_H


#include "basic.h"
#include "flags.h"

namespace jazz {

    enum SYMBOL_LIST_SORT_METHOD {
        SYMBOL_LIST_SORT_NONE = 0,
        SYMBOL_LIST_SORT_NAME_ASC,
    };

    class Expr;

    /**
     * Compare two expressions, required by std::map.
     */
    struct ExprLess {
        bool operator()(const Expr &e1, const Expr &e2) const;
    };

    /**
     * Hash an expression, required by std::unordered_map.
     */
    struct ExprHash {
        unsigned operator()(const Expr &e) const;
    };

    /**
     * Compare two expressions, required by std::unordered_map.
     */
    struct ExprEqual {
        bool operator()(const Expr &e1, const Expr &e2) const;
    };

    class Expr {
        template<class T>
        friend inline const T &expr_cast(const Expr &e);
        template<class T>
        friend inline bool is_a(const Expr &e);
        template<class T>
        friend inline bool is_exactly_a(const Expr &e);
        friend inline bool are_ex_trivially_equal(const Expr &e1, const Expr &e2);

    public:
        Expr(const Basic &other) : ptr(makeFromBasic(other)) {}
        // construct from bool. The implicit conversion is intended.
        Expr(bool v = false);
        explicit Expr(const char *name);

    public:
        void swap(Expr &other) noexcept {
            JAZZ_ASSERT(ptr->flags & STATUS_FLAG_DYNAMIC_ALLOC);
            JAZZ_ASSERT(other.ptr->flags & STATUS_FLAG_DYNAMIC_ALLOC);
            ptr.swap(other.ptr);
        }

        Expr eval() const {
            return ptr->eval();
        }

        // printing
        void print(const PrintContext &context, unsigned level = 0) const;
        void debugPrint() const;
        void debugPrintTree() const;
        void printTruthTable() const;
        void printTruthTable(const std::vector<Expr> &vars) const;

        int compare(const Expr &other) const;
        unsigned hashValue() const { return ptr->hashValue(); }
        void share(const Expr &other) const;

        // access to operands
        std::size_t numOperands() const { return ptr->numOperands(); }
        const Expr &operand(size_t i) const { return ptr->operand(i); }

        // pattern matching
        bool has(const Expr &e, unsigned options = 0) const {
            return ptr->has(e, options);
        }

        bool match(const Expr &other, ExprMap &map) const {
            return ptr->match(other, map);
        }

        bool isEqual(const Expr &other) const {
            return ptr->isEqual(*other.ptr);
        }

        Expr subs(const ExprMap &m, unsigned options = 0) const {
            return ptr->subs(m, options);
        }

        Expr subs(const Expr &expr, unsigned options = 0) const;

        bool isType(unsigned type_flag) const {
            return ptr->isType(type_flag);
        }

        bool isTrivial() const {
            return ptr->isTrivial();
        }

        bool trivialValue() const {
            return ptr->trivialValue();
        }

        Expr simplified() const {
            return ptr->simplified();
        }


        std::vector<Expr> getVars(int sort_flag = SYMBOL_LIST_SORT_NAME_ASC) const;

    private:
        static Ptr<Basic> makeFromBasic(const Basic &b) {
            if (b.flags & STATUS_FLAG_DYNAMIC_ALLOC) {
                return Ptr<Basic>(const_cast<Basic &>(b));
            } else {
                return Ptr<Basic>(b.duplicate());
            }
        }

    private:
        mutable Ptr<Basic> ptr;
    };// class Expr

    template<class T>
    bool is_a(const Expr &e) {
        return jazz::is_a<T>(*e.ptr);
    }

    template<class T>
    bool is_exactly_a(const Expr &e) {
        return jazz::is_exactly_a<T>(*e.ptr);
    }

    template<class T>
    const T &expr_cast(const Expr &e) {
        JAZZ_ASSERT(is_a<T>(e));
        return static_cast<const T &>(*e.ptr);
    }

    bool are_ex_trivially_equal(const Expr &e1, const Expr &e2) {
        return e1.ptr == e2.ptr;
    }

}// namespace jazz

#endif//BOOLEAN_ALGEBRA_EXPR_H
