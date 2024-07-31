/**
 * @brief
 * @file relational.cpp
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

#include "relational.h"
#include "hash_seed.h"
#include "utils.h"

namespace jazz {
    static void printOperator(const PrintContext &c, Relational::RelationalOp op) {
        switch (op) {
            case Relational::EQUAL:
                c.os << "==";
                break;
            case Relational::NOT_EQUAL:
                c.os << "!=";
                break;
            case Relational::LESS:
                c.os << "<";
                break;
            case Relational::LESS_OR_EQUAL:
                c.os << "<=";
                break;
            case Relational::GREATER:
                c.os << ">";
                break;
            case Relational::GREATER_OR_EQUAL:
                c.os << ">=";
                break;
            default:
                c.os << "(INVALID RELATIONAL OPERATOR)";
                break;
        }
    }
}// namespace jazz

namespace jazz {
    JAZZ_IMPLEMENT_REGISTERED_CLASS_OPT(Relational, Basic, print_func<PrintContext>(&Relational::doPrint));
    JAZZ_IMPLEMENT_COMPARE_SAME_TYPE(Relational, other) {
        JAZZ_ASSERT(is_a<Relational>(other));
        const auto &oth = static_cast<const Relational &>(other);
        if (this->op == oth.op && lhs.isEqual(oth.lhs) && rhs.isEqual(oth.rhs)) {
            return 0;
        }

        switch (this->op) {
            case EQUAL:
            case NOT_EQUAL:
                if (oth.op != op)
                    return (op < oth.op) ? -1 : 1;
                break;
            case LESS:
                if (oth.op != GREATER) return (op < oth.op) ? -1 : 1;
                break;
            case LESS_OR_EQUAL:
                if (oth.op != GREATER_OR_EQUAL) return (op < oth.op) ? -1 : 1;
                break;
            case GREATER:
                if (oth.op != LESS) return (op < oth.op) ? -1 : 1;
                break;
            case GREATER_OR_EQUAL:
                if (oth.op != LESS_OR_EQUAL) return (op < oth.op) ? -1 : 1;
                break;
        }

        int cmp = lhs.compare(oth.lhs);
        return cmp != 0 ? cmp : rhs.compare(oth.rhs);
    }

    Relational::Relational() = default;

    Relational::Relational(const Expr &lhs, const Expr &rhs, Relational::RelationalOp op) : lhs(lhs), rhs(rhs), op(op) {
    }
    std::size_t Relational::numOperands() const {
        return 2;
    }
    Expr Relational::subs(const ExprMap &m, unsigned int options) const {
        auto new_lhs = lhs.subs(m, options);
        auto new_rhs = rhs.subs(m, options);
        if (!are_ex_trivially_equal(lhs, new_lhs) || !are_ex_trivially_equal(rhs, new_rhs)) {
            return Relational(new_lhs, new_rhs, op).subsOneLevel(m, options);
        } else {
            return subsOneLevel(m, options);
        }
    }
    unsigned Relational::computeHash() const {
        auto v = makeHashSeed(typeid(*this));
        auto lhs_hash = lhs.hashValue();
        auto rhs_hash = rhs.hashValue();
        v = rotateLeft(v);
        switch (op) {
            case EQUAL:
            case NOT_EQUAL:
                if (lhs_hash > rhs_hash) {
                    v ^= lhs_hash;
                    lhs_hash = rhs_hash;
                } else {
                    v ^= rhs_hash;
                }
                break;
            case LESS:
            case LESS_OR_EQUAL:
                v ^= rhs_hash;
                break;
            case GREATER:
            case GREATER_OR_EQUAL:
                v ^= lhs_hash;
                lhs_hash = rhs_hash;
                break;
        }

        v = rotateLeft(v);
        v ^= lhs_hash;
        if (flags & STATUS_FLAG_EVALUATED) {
            setFlags(STATUS_FLAG_HASH_CALCULATED);
            hash = v;
        }

        return hash;
    }
    void Relational::doPrint(const jazz::PrintContext &c, unsigned int level) const {
        if (precedence() <= level)
            c.os << "(";
        lhs.print(c, precedence());
        printOperator(c, op);
        rhs.print(c, precedence());
        if (precedence() <= level)
            c.os << ")";
    }
    const Expr &Relational::operand(int i) const {
        JAZZ_ASSERT(i == 0 || i == 1);
        return i == 0 ? lhs : rhs;
    }
    Expr &Relational::operand(int i) {
        JAZZ_ASSERT(i == 0 || i == 1);
        return i == 0 ? lhs : rhs;
    }
    bool Relational::isType(unsigned int type_flag) const {
        if (type_flag == TYPE_FLAG_RELATIONAL) return true;
        switch (op) {
            case EQUAL:
                return type_flag == TYPE_FLAG_RELATIONAL_EQUAL;
            case NOT_EQUAL:
                return type_flag == TYPE_FLAG_RELATIONAL_NOT_EQUAL;
            case LESS:
                return type_flag == TYPE_FLAG_RELATIONAL_LESS;
            case LESS_OR_EQUAL:
                return type_flag == TYPE_FLAG_RELATIONAL_LESS_OR_EQUAL;
            case GREATER:
                return type_flag == TYPE_FLAG_RELATIONAL_GREATER;
            case GREATER_OR_EQUAL:
                return type_flag == TYPE_FLAG_RELATIONAL_GREATER_OR_EQUAL;
            default:
                return false;
        }
    }

}// namespace jazz
