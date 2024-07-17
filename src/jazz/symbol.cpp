

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

#include "symbol.h"
#include "utils.h"
#include "hash_seed.h"

namespace jazz {

    JAZZ_IMPLEMENT_REGISTERED_CLASS_OPT(Symbol, Basic, print_func<PrintContext>(&Symbol::doPrint));
    unsigned Symbol::serial_count = 0;


    int Symbol::compareSameType(const jazz::Basic &other) const {
        JAZZ_ASSERT(is_a<Symbol>(other));
        const auto *o = static_cast<const Symbol *>(&other);
        if (serial == o->serial) return 0;
        return serial < o->serial ? -1 : 1;
    }

    Symbol::Symbol() : serial(serial_count++) {}

    void Symbol::doPrint(const jazz::PrintContext &context, unsigned int level) const {
        context.os << name;
    }
    Expr Symbol::eval() const {
        return *this;
    }
    unsigned Symbol::computeHash() const {
        unsigned seed = makeHashSeed(typeid(*this));
        hash = goldenRatioHash(seed ^ serial);
        setFlags(STATUS_FLAG_HASH_CALCULATED);
        return hash;
    }
    bool Symbol::isType(unsigned int type_flag) const {
        return type_flag == TYPE_FLAG_SYMBOL;
    }
    Symbol::~Symbol() = default;
}// namespace jazz
