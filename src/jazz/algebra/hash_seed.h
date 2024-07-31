/**
 * @file hash_seed.h
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

#ifndef BOOLEAN_ALGEBRA_HASH_SEED_H
#define BOOLEAN_ALGEBRA_HASH_SEED_H

#include <cstring>
#include <typeinfo>

namespace jazz {
    /** Truncated multiplication with golden ratio, for computing hash values. */
    inline unsigned goldenRatioHash(unsigned long n) {
        return n * 0x4f1bbcddull;
    }

    inline unsigned makeHashSeed(const std::type_info &info) {
        auto ptr = info.name();
        auto v = goldenRatioHash((unsigned long) ptr);
        return v;
    }
}// namespace jazz

#endif//BOOLEAN_ALGEBRA_HASH_SEED_H
