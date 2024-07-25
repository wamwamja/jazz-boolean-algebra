/**
 * @file flags.h
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

#ifndef BOOLEAN_ALGEBRA_FLAGS_H
#define BOOLEAN_ALGEBRA_FLAGS_H

namespace jazz {
    enum STATUS_FLAGS {
        STATUS_FLAG_DYNAMIC_ALLOC = 0x0001,
        STATUS_FLAG_HASH_CALCULATED = 0x0002,
        STATUS_FLAG_EVALUATED = 0x0004,
        STATUS_FLAG_NOT_SHAREABLE = 0x0008,
        STATUS_FLAG_EXPANDED = 0x0010,
        STATUS_FLAG_SIMPLIFIED = 0x0020,
    };

    /** Flags to control the behavior of subs(). */
    enum SUBS_OPTION {
        SUBS_OPTION_NO_PATTERN = 0x0001,            ///< disable pattern matching
        SUBS_OPTION_SUBS_NO_PATTERN = 0x0001,       // for backwards compatibility
        SUBS_OPTION_algebraic = 0x0002,             ///< enable algebraic substitutions
        SUBS_OPTION_subs_algebraic = 0x0002,        // for backwards compatibility
        SUBS_OPTION_pattern_is_product = 0x0004,    ///< used internally by expairseq::subschildren()
        SUBS_OPTION_pattern_is_not_product = 0x0008,///< used internally by expairseq::subschildren()
        SUBS_OPTION_no_index_renaming = 0x0010,
        // To indicate that we want to substitute an index by something that
        // is not an index. Without this flag the index value would be
        // substituted in that case.
        SUBS_OPTION_really_subs_idx = 0x0020
    };

    /***
     * Enumerates the all the types of expressions.
     */
    enum TYPE_FLAGS {
        TYPE_FLAG_BOOLEAN,
        TYPE_FLAG_AND,
        TYPE_FLAG_OR,
        TYPE_FLAG_NOT,
        TYPE_FLAG_WILDCARD,
        TYPE_FLAG_SYMBOL,
        TYPE_FLAG_RELATIONAL,
        TYPE_FLAG_RELATIONAL_EQUAL,
        TYPE_FLAG_RELATIONAL_NOT_EQUAL,
        TYPE_FLAG_RELATIONAL_LESS,
        TYPE_FLAG_RELATIONAL_LESS_OR_EQUAL,
        TYPE_FLAG_RELATIONAL_GREATER,
        TYPE_FLAG_RELATIONAL_GREATER_OR_EQUAL,
        TYPE_FLAG_BOOLEAN_FUNCTION,
    };
}// namespace jazz

#endif//BOOLEAN_ALGEBRA_FLAGS_H
