/**
 * @brief Not is a class that represents the logical NOT operation.
 * @file op_not.h
 */

#ifndef BOOLEAN_ALGEBRA_OP_NOT_H
#define BOOLEAN_ALGEBRA_OP_NOT_H

#include "basic.h"
#include "expr.h"

namespace jazz {
    class Not : public Basic {
        JAZZ_DECLARE_REGISTERED_CLASS(Not, Basic);
        friend class Basic;
        friend class And;
        friend class Or;

    public:
        explicit Not(const Expr &ex);

        unsigned precedence() const override;

        bool isType(unsigned type_flag) const override;
        std::size_t numOperands() const override;
        Expr &operand(int i) override;
        const Expr &operand(int i) const override;
        Expr subs(const ExprMap &m, unsigned options) const override;
        bool isTrivial() const override;
        bool trivialValue() const override;

        Expr simplified() const override;

    protected:
        unsigned computeHash() const override;
        void doPrint(const jazz::PrintContext &context, unsigned level) const;

    private:
        bool not_flag = true;
        Expr expr;
    };
}// namespace jazz

#endif//BOOLEAN_ALGEBRA_OP_NOT_H
