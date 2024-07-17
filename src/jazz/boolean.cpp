/**
 * @file boolean.cpp
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

#include "boolean.h"

namespace jazz {
    JAZZ_IMPLEMENT_REGISTERED_CLASS_OPT(Boolean, Basic, print_func<PrintContext>(&Boolean::doPrint));
    JAZZ_IMPLEMENT_COMPARE_SAME_TYPE(Boolean, other) {
        JAZZ_ASSERT(is_a<Boolean>(other));
        const auto *o = static_cast<const Boolean *>(&other);
        return value == o->value ? 0 : (value < o->value ? -1 : 1);
    }

    void Boolean::doPrint(const jazz::PrintContext &context, unsigned int level) const {
        context.os << (value ? "1" : "0");
    }

    bool Boolean::isEqual(const Basic &other) const {
        if (other.isTrivial()) {
            return value == other.trivialValue();
        }
        else{
            return false;
        }
    }
}// namespace jazz
