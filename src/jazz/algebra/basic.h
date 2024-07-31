/**
* @file basic.h
*
* The basic class for all the objects in the library.
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

#ifndef BOOLEAN_ALGEBRA_BASIC_H
#define BOOLEAN_ALGEBRA_BASIC_H

#include "jazz/algebra/flags.h"
#include "jazz/algebra/print.h"
#include "jazz/algebra/ptr.h"
#include "jazz/algebra/registration.h"
#include "jazz/config.h"

#include <unordered_map>

namespace jazz {

    class Expr;
    struct ExprLess;
    struct ExprHash;
    struct ExprEqual;

    using ExprMap = std::unordered_map<Expr, Expr, ExprHash, ExprEqual>;

    class Visitor {
    public:
        Visitor() = default;
        virtual ~Visitor() = default;
    };

    struct MapFunction {
        MapFunction() = default;
        virtual ~MapFunction() = default;
        using ArgType = const Expr &;
        using ReturnType = Expr;
        virtual ReturnType operator()(ArgType arg) const = 0;
    };

    class Basic : public RefCounted {
        friend class Expr;
        JAZZ_DECLARE_REGISTERED_CLASS_NO_CONSTRUCTORS(Basic, void);

    public:
        Basic(const Basic &other);
        Basic &operator=(const Basic &other);
        virtual ~Basic() = default;

        virtual Basic *duplicate() const;

        // pattern matching
        /**
         * Check if the expression matches the pattern.
         * @param pattern       The pattern to match.
         * @param replace_list  For every wildcard in the pattern, the matching expression will be stored in this map.
         * @return
         */
        virtual bool match(const Expr &pattern, ExprMap &replace_list) const;

        /**
         * Check if the expression has the pattern.
         * @param pattern
         * @param options
         * @return
         */
        virtual bool has(const Expr &pattern, unsigned int options) const;

        /**
         * Substitute the expression with the map.
         * @param m       The map to substitute.
         * @param options
         * @return
         */
        virtual Expr subs(const ExprMap &m, unsigned options) const;

        virtual void accept(jazz::Visitor &v) const;
        virtual Expr map(const MapFunction &f) const;
        virtual Expr eval() const;

    public:
        const Basic &setFlags(unsigned flag) const;
        const Basic &clearFlags(unsigned flag) const;
        const Basic &hold() const;

        // access operands
        virtual std::size_t numOperands() const;
        virtual const Expr &operand(int i) const;
        virtual Expr &operand(int i);

        // type information
        /**
         * Check if the object is of the specified type.
         * @param info
         * @return
         */
        virtual bool isType(unsigned info) const;

        /**
         * Get the hash value of the object.
         * @return
         */
        unsigned hashValue() const;

        /**
         * Compare the object with another object.
         * @param other
         * @return
         */
        int compare(const Basic &other) const;

        /**
         * Check if the object is trivially true or false.
         * @return
         */
        virtual bool isTrivial() const;

        /**
         * Get the trivial value of the object.
         *
         * Must call isTrivial() first to ensure the object is trivial.
         * @return
         */
        virtual bool trivialValue() const;

        /**
         * Simplify the object.
         * @return
         */
        virtual Expr simplified() const;

    public:
        /**
         * Get the priority of the object.
         *
         * The higher the value, the higher the priority.
         * It's used for printing.
         * @return
         */
        virtual unsigned precedence() const;
        virtual void print(const PrintContext &c, unsigned level) const;
        virtual void debugPrint() const;
        virtual void debugPrintTree() const;
        void printDispatch(const RegisteredClassHierarchy &class_hierarchy, const PrintContext &c, unsigned level) const;

    protected:
        Basic() = default;

        /**
         * Comparison for objects that have the same type.
         * @param other
         * @return
         */
        virtual int compareSameType(const Basic &other) const;

        /**
         * Check the equality for objects that have the same type.
         * @param other
         * @return
         */
        virtual bool isEqualSameType(const Basic &other) const;

        /**
         * Test the equality of 2 objects.
         * @param other
         * @return
         */
        virtual bool isEqual(const Basic &other) const;

        /**
         * Compute the hash value of the object.
         * @return
         */
        virtual unsigned int computeHash() const;

        void printDelegate(const jazz::PrintContext &c, unsigned level) const;

        /**
         * Substitute the expression with the map, but only one level(no recursions).
         * @param m         The map to substitute.
         * @param options
         * @return
         */
        Expr subsOneLevel(const jazz::ExprMap &m, unsigned options) const;

        void ensureIfModifiable() const;

    protected:
        mutable unsigned flags = 0;
        mutable unsigned hash = 0;
    };


    // type check
    template<typename T>
    inline bool is_a(const Basic &b) {
        return dynamic_cast<const T *>(&b) != nullptr;
    }

    template<typename T>
    inline bool is_exactly_a(const Basic &b) {
        return typeid(T) == typeid(b);
    }

    // dynamic allocation
    template<typename B, typename... Args>
    inline B &create(Args &&...args) {
        return const_cast<B &>(static_cast<const B &>((new B(std::forward<Args>(args)...))->setFlags(STATUS_FLAG_DYNAMIC_ALLOC)));
    }

    template<typename B>
    inline B &create(std::initializer_list<Expr> il) {
        return const_cast<B &>(static_cast<const B &>((new B(il))->setFlags(STATUS_FLAG_DYNAMIC_ALLOC)));
    }

}// namespace jazz

#endif//BOOLEAN_ALGEBRA_BASIC_H
