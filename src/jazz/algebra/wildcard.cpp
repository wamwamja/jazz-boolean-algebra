
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

#include "wildcard.h"
#include "expr.h"
#include "hash_seed.h"
#include "utils.h"

namespace jazz {

    JAZZ_IMPLEMENT_REGISTERED_CLASS_OPT(Wildcard, Basic, print_func<PrintContext>(&Wildcard::doPrint));
    JAZZ_IMPLEMENT_COMPARE_SAME_TYPE(Wildcard, other) {
        JAZZ_ASSERT(is_a<Wildcard>(other));
        const Wildcard &o = static_cast<const Wildcard &>(other);
        if (label == o.label)
            return 0;
        else
            return label < o.label ? -1 : 1;
    }

    void Wildcard::doPrint(const jazz::PrintContext &c, unsigned int level) const {
        c.os << "$" << label;
    }
    Wildcard::Wildcard(unsigned int label) : label(label) {
        setFlags(STATUS_FLAG_EVALUATED | STATUS_FLAG_EXPANDED);
    }
    unsigned Wildcard::computeHash() const {
        unsigned seed = makeHashSeed(typeid(*this));
        hash = goldenRatioHash(seed ^ label);
        setFlags(STATUS_FLAG_HASH_CALCULATED);
        return hash;
    }
    bool Wildcard::match(const Expr &pattern, ExprMap &map) const {
        return isEqual(expr_cast<Basic>(pattern));
    }
    bool hasWild(const Expr &e) {
        if (is_a<Wildcard>(e))
            return true;
        for (size_t i = 0; i < e.numOperands(); ++i)
            if (hasWild(e.operand(i)))
                return true;
        return false;
    }
}// namespace jazz
