
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

#ifndef BOOLEAN_ALGEBRA_SYMBOL_H
#define BOOLEAN_ALGEBRA_SYMBOL_H


#include "basic.h"
#include "expr.h"

#include <string>
#include <utility>

namespace jazz {
    class Symbol : public Basic {
        JAZZ_DECLARE_REGISTERED_CLASS(Symbol, Basic);

    public:
        explicit Symbol(std::string name) : name(std::move(name)) {
            serial = serial_count++;
            setFlags(STATUS_FLAG_EVALUATED);
        }

        ~Symbol() override;;

    public:

        void setName(std::string name_) {
            this->name = std::move(name_);
        }

        std::string getName() const {
            return name;
        }

        Expr eval() const override;

        unsigned computeHash() const override;

        bool isType(unsigned type_flag) const override;

    protected:
        void doPrint(const jazz::PrintContext &context, unsigned level) const;

    protected:
        unsigned serial;
        std::string name;

    private:
        static unsigned serial_count;
    };
}// namespace jazz

#endif//BOOLEAN_ALGEBRA_SYMBOL_H
