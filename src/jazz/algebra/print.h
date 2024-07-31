/**
 * @brief
 * @file print.h
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

#ifndef BOOLEAN_ALGEBRA_PRINT_H
#define BOOLEAN_ALGEBRA_PRINT_H

#include "class_hierarchy.h"
#include <iosfwd>
#include <memory>
#include <string>

namespace jazz {

    unsigned int getPrintContextId();

    // Required by ClassHierarchy<>.
    class PrintContextInfo {
    public:
        PrintContextInfo(const char *name, const char *parent_name, unsigned int id)
            : name(name), parent_name(parent_name), id(id) {}

        const char *className() const { return name; }
        const char *parentName() const { return parent_name; }
        unsigned typeInfo() const { return id; }

    private:
        const char *name;
        const char *parent_name;
        unsigned int id;
    };

    using PrintContextClassHierarchy = ClassHierarchy<PrintContextInfo>;

#define JAZZ_DECLARE_PRINT_CONTEXT_COMMON(classname)                          \
public:                                                                       \
    friend class function_options;                                            \
    friend class RegisteredClass;                                             \
    static const jazz::PrintContextClassHierarchy &getClassHierarchyStatic(); \
    classname();

#define JAZZ_DECLARE_PRINT_CONTEXT_BASE(classname)                                                                        \
    JAZZ_DECLARE_PRINT_CONTEXT_COMMON(classname)                                                                          \
    virtual const jazz::PrintContextClassHierarchy &getClassInfo() const { return classname::getClassHierarchyStatic(); } \
    virtual const char *class_name() const { return classname::getClassHierarchyStatic().class_info.className(); }        \
    virtual classname *duplicate() const { return new classname(*this); }


#define JAZZ_DECLARE_PRINT_CONTEXT(classname, parentname)                                                                  \
    JAZZ_DECLARE_PRINT_CONTEXT_COMMON(classname)                                                                           \
    using ParentType = parentname;                                                                                         \
    const jazz::PrintContextClassHierarchy &getClassInfo() const override { return classname::getClassHierarchyStatic(); } \
    const char *class_name() const override { return classname::getClassHierarchyStatic().class_info.className(); }        \
    classname *duplicate() const override { return new classname(*this); }                                                 \
                                                                                                                           \
private:

#define JAZZ_IMPLEMENT_PRINT_CONTEXT(classname, parentname)                                                                   \
    const jazz::PrintContextClassHierarchy &classname::getClassHierarchyStatic() {                                            \
        static jazz::PrintContextClassHierarchy reg_info =                                                                    \
                jazz::PrintContextClassHierarchy(jazz::PrintContextInfo(#classname, #parentname, jazz::getPrintContextId())); \
        return reg_info;                                                                                                      \
    }


    class PrintContext {
        JAZZ_DECLARE_PRINT_CONTEXT_BASE(PrintContext);

    public:
        explicit PrintContext(std::ostream &os, unsigned options = 0) : os(os), options(options) {}
        virtual ~PrintContext() = default;

        std::ostream &os;
        unsigned options;
    };

    class PrintDefault : public PrintContext {
        JAZZ_DECLARE_PRINT_CONTEXT(PrintDefault, PrintContext);

    public:
        explicit PrintDefault(std::ostream &os, unsigned options = 0) : PrintContext(os, options) {}
    };

    class PrintTree : public PrintContext {
        JAZZ_DECLARE_PRINT_CONTEXT(PrintTree, PrintContext);

    public:
        explicit PrintTree(std::ostream &os, unsigned options = 0) : PrintContext(os, options) {}
    };

    class Basic;
    class PrintFunctorInterface {
    public:
        PrintFunctorInterface() = default;
        virtual ~PrintFunctorInterface() = default;
        virtual PrintFunctorInterface *duplicate() const = 0;
        virtual void operator()(const Basic &b, const PrintContext &c, unsigned level) const = 0;
    };

    template<typename T, typename C>
    class PrintFunctorPtrFunction : public PrintFunctorInterface {
    public:
        using Functor = void (*)(const T &, const C &, unsigned);
        explicit PrintFunctorPtrFunction(Functor f) : f(f) {}
        PrintFunctorInterface *duplicate() const override { return new PrintFunctorPtrFunction(*this); }
        void operator()(const Basic &b, const PrintContext &c, unsigned level) const override {
            f(dynamic_cast<const T &>(b), dynamic_cast<const C &>(c), level);
        }

    private:
        Functor f;
    };

    template<typename T, typename C>
    class PrintFunctorMemberFunction : public PrintFunctorInterface {
    public:
        using Functor = void (T::*)(const C &, unsigned) const;
        explicit PrintFunctorMemberFunction(Functor f) : f(f) {}
        PrintFunctorInterface *duplicate() const override { return new PrintFunctorMemberFunction(*this); }
        void operator()(const Basic &b, const PrintContext &c, unsigned level) const override {
            (dynamic_cast<const T &>(b).*f)(dynamic_cast<const C &>(c), level);
        }

    private:
        Functor f;
    };

    class PrintFunctor {
    public:
        PrintFunctor() = default;
        PrintFunctor(const PrintFunctor &other) : p(other.p == nullptr ? nullptr : other.p->duplicate()) {}
        explicit PrintFunctor(std::unique_ptr<PrintFunctorInterface> p) : p(std::move(p)) {}

        template<typename T, typename C>
        PrintFunctor(void (*f)(const T &, const C &, unsigned)) : p(new PrintFunctorPtrFunction<T, C>(f)) {}


        template<typename T, typename C>
        PrintFunctor(void (T::*f)(const C &, unsigned) const) : p(new PrintFunctorMemberFunction<T, C>(f)) {}

        PrintFunctor &operator=(const PrintFunctor &other) {
            if (this != &other)
                p.reset(other.p == nullptr ? nullptr : other.p->duplicate());
            return *this;
        }

        ~PrintFunctor() = default;

        void operator()(const Basic &b, const PrintContext &c, unsigned level) const {
            (*p)(b, c, level);
        }

        bool isValid() const {
            return p != nullptr;
        }

    private:
        std::unique_ptr<PrintFunctorInterface> p;
    };


};// namespace jazz

#endif//BOOLEAN_ALGEBRA_PRINT_H
