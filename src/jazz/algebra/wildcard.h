

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

#ifndef BOOLEAN_ALGEBRA_WILDCARD_H
#define BOOLEAN_ALGEBRA_WILDCARD_H

#include "basic.h"
#include "expr.h"

namespace jazz{
    class Wildcard : public Basic {
        JAZZ_DECLARE_REGISTERED_CLASS(Wildcard, Basic);

    public:
        explicit Wildcard(unsigned label);

        unsigned computeHash() const override;
        bool match(const Expr &pattern, ExprMap &map) const override;

    protected:
        void doPrint(const jazz::PrintContext &c, unsigned level) const;
        void doPrintTree(const PrintTree & c, unsigned level) const;
    private:
        unsigned label;
    };


    inline Expr wildcard(unsigned label) {
        return Expr(Wildcard(label));
    }

    bool hasWild(const Expr &e);

}

#endif//BOOLEAN_ALGEBRA_WILDCARD_H
