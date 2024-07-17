/**
 * @brief class registration
 * @file registration.h
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

#ifndef BOOLEAN_ALGEBRA_REGISTRATION_H
#define BOOLEAN_ALGEBRA_REGISTRATION_H


#include "class_hierarchy.h"
#include "print.h"

#include <list>
#include <string>
#include <typeinfo>
#include <vector>

namespace jazz {
    class Expr;

    struct ReturnType {
        const std::type_info &type_info;
        unsigned rl;

        inline bool operator<(const ReturnType &rhs) const {
            if (type_info.before(rhs.type_info))
                return true;
            return rl < rhs.rl;
        }

        inline bool operator==(const ReturnType &rhs) const {
            return type_info == rhs.type_info && rl == rhs.rl;
        }

        inline bool operator!=(const ReturnType &rhs) const {
            return !(*this == rhs);
        }
    };

    template<typename T>
    inline ReturnType make_return_type(unsigned rl = 0) {
        return ReturnType{typeid(T), rl};
    }

    class RegisteredClass {
    public:
        RegisteredClass(const char *name, const char *parent_name, const std::type_info &type_info)
            : name(name), parent_name(parent_name), type_info(type_info) {}

        const char *className() const { return name; }
        const char *parentName() const { return parent_name; }
        const std::type_info &typeInfo() const { return type_info; }
        const std::vector<PrintFunctor> &printDispatchTable() const { return print_dispatch_table; }

        template<typename Context, typename T, typename C>
        RegisteredClass &print_func(void f(const T &, const C &c, unsigned)) {
            set_print_func(Context::getClassHierarchyStatic().class_info.typeInfo(), f);
            return *this;
        }

        template<typename Context, typename T, typename C>
        RegisteredClass &print_func(void (T::*f)(const C &c, unsigned) const) {
            set_print_func(Context::getClassHierarchyStatic().class_info.typeInfo(), f);
            return *this;
        }

        template<typename Context>
        RegisteredClass &print_func(const PrintFunctor &f) {
            set_print_func(Context::getClassHierarchyStatic().class_info.typeInfo(), f);
            return *this;
        }

        void set_print_func(unsigned id, const PrintFunctor &f) {
            if (id >= print_dispatch_table.size())
                print_dispatch_table.resize(id + 1);
            print_dispatch_table[id] = f;
        }


    private:
        const char *name;
        const char *parent_name;
        const std::type_info &type_info;
        // method table for print() dispatch
        std::vector<PrintFunctor> print_dispatch_table;
    };

    using RegisteredClassHierarchy = ClassHierarchy<RegisteredClass>;

#define JAZZ_DECLARE_REGISTERED_CLASS_COMMON(classname)                                    \
private:                                                                                   \
    static jazz::RegisteredClassHierarchy class_info;                                      \
                                                                                           \
public:                                                                                    \
    static jazz::RegisteredClassHierarchy &getClassHierarchySatic() { return class_info; } \
    class Visitor {                                                                        \
    public:                                                                                \
        virtual void visit(const classname &obj) = 0;                                      \
        virtual ~Visitor() = default;                                                      \
    };


#define JAZZ_DECLARE_REGISTERED_CLASS_NO_CONSTRUCTORS(classname, parent_name) \
    JAZZ_DECLARE_REGISTERED_CLASS_COMMON(classname)                           \
    using ParentType = parent_name;                                           \
    virtual const jazz::RegisteredClassHierarchy &getClassHierarchy() const { \
        return classname::getClassHierarchySatic();                           \
    }                                                                         \
                                                                              \
    virtual jazz::RegisteredClassHierarchy &getClassHierarchy() {             \
        return classname::getClassHierarchySatic();                           \
    }                                                                         \
    virtual const char *getClassName() const {                                \
        return classname::getClassHierarchy().class_info.className();         \
    }

#define JAZZ_DECLARE_REGISTERED_CLASS(classname, parent_name)                  \
    JAZZ_DECLARE_REGISTERED_CLASS_COMMON(classname)                            \
    using ParentType = parent_name;                                            \
    classname();                                                               \
    classname *duplicate() const override {                                    \
        auto *copy = new classname(*this);                                     \
        copy->setFlags(jazz::STATUS_FLAG_DYNAMIC_ALLOC);                       \
        return copy;                                                           \
    }                                                                          \
                                                                               \
    void accept(jazz::Visitor &v) const override {                             \
        if (Visitor *p = dynamic_cast<Visitor *>(&v))                          \
            p->visit(*this);                                                   \
        else                                                                   \
            ParentType::accept(v);                                             \
    }                                                                          \
    const jazz::RegisteredClassHierarchy &getClassHierarchy() const override { \
        return classname::getClassHierarchySatic();                            \
    }                                                                          \
                                                                               \
    jazz::RegisteredClassHierarchy &getClassHierarchy() override {             \
        return classname::getClassHierarchySatic();                            \
    }                                                                          \
    const char *getClassName() const override {                                \
        return classname::getClassHierarchy().class_info.className();          \
    }                                                                          \
                                                                               \
protected:                                                                     \
    int compareSameType(const jazz::Basic &other) const override;


#define JAZZ_IMPLEMENT_REGISTERED_CLASS_OPT(classname, parentname, options) \
    jazz::RegisteredClassHierarchy classname::class_info = jazz::RegisteredClassHierarchy(jazz::RegisteredClass(#classname, #parentname, typeid(classname)).options);


    /** Add or replace a print method. */
    template<class Alg, class Ctx, class T, class C>
    inline void set_print_func(void f(const T &, const C &c, unsigned)) {
        Alg::getClassHierarchySatic().class_info.set_print_func(Ctx::getClassHierarchySatic().class_info.typeInfo(), f);
    }

    /** Add or replace a print method. */
    template<class Alg, class Ctx, class T, class C>
    inline void set_print_func(void (T::*f)(const C &, unsigned)) {
        Alg::getClassHierarchySatic().class_info.set_print_func(Ctx::getClassHierarchySatic().class_info.typeInfo(), f);
    }

#define JAZZ_IMPLEMENT_COMPARE_SAME_TYPE(classname, other) \
    int classname::compareSameType(const Basic &other) const

}// namespace jazz

#endif//BOOLEAN_ALGEBRA_REGISTRATION_H
