/**
 * @file boolean.h
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

#ifndef BOOLEAN_ALGEBRA_BOOLEAN_H
#define BOOLEAN_ALGEBRA_BOOLEAN_H

#include "basic.h"
#include "expr.h"

namespace jazz {
    class Boolean : public Basic {
        JAZZ_DECLARE_REGISTERED_CLASS(Boolean, Basic);

    public:
        explicit Boolean(bool v) : value(v) {
            setFlags(STATUS_FLAG_EVALUATED);
        }

        Expr eval() const override {
            return hold();
        }

    public:
        bool val() const {
            return value;
        }

        bool isTrue() const {
            return value;
        }

        bool isFalse() const {
            return !value;
        }

        bool isTrivial() const override {
            return true;
        }

        bool trivialValue() const override {
            return value;
        }

        Boolean op_and(const Boolean &other) const {
            return Boolean(value && other.value);
        }

        Boolean op_or(const Boolean &other) const {
            return Boolean(value || other.value);
        }

        Boolean op_not() const {
            return Boolean(!value);
        }

        Boolean op_xor(const Boolean &other) const {
            return Boolean(value ^ other.value);
        }

        Boolean op_nand(const Boolean &other) const {
            return Boolean(!(value && other.value));
        }

        bool isEqual(const Basic& other) const override;

        static Expr from_bool(bool v) {
            return Expr(v ? True() : False());
        }

        static Basic *True() {
            static Boolean true_value(true);
            true_value.addRef();
            return &true_value;
        }

        static Basic *False() {
            static Boolean false_value(false);
            false_value.addRef();
            return &false_value;
        }

    protected:
        void doPrint(const jazz::PrintContext &context, unsigned level) const;

    protected:
        bool value;
    };

}// namespace jazz

#endif//BOOLEAN_ALGEBRA_BOOLEAN_H
