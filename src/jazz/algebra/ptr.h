/**
 * @brief Ptr is a smart pointer that manages the life cycle of a RefCounted object.
 * @file ptr.h
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

#ifndef BOOLEAN_ALGEBRA_PTR_H
#define BOOLEAN_ALGEBRA_PTR_H

#include <stdexcept>
#include <utility>

namespace jazz {

    /**
     * @brief RefCounted is a base class that provides reference counting mechanism.
     */
    class RefCounted {
    public:
        RefCounted() : ref_count(0){};

        unsigned int addRef() {
            return ++ref_count;
        }

        unsigned int release() {
            return --ref_count;
        }

        unsigned int refCount() const {
            return ref_count;
        }

        void setRefCount(unsigned int count) {
            ref_count = count;
        }

    private:
        unsigned int ref_count = 0;
    };

    /**
     * @brief Ptr is a smart pointer that manages the life cycle of a RefCounted object.
     *
     * @tparam RefCountedT A derived class of RefCounted.
     */
    template<typename RefCountedT>
    class Ptr {
    public:
        /// bind to a newly created ptr
        explicit Ptr(RefCountedT *p) : ptr(p) {
            if (ptr == nullptr) {
                throw std::invalid_argument("Ptr: nullptr");
            }
            ptr->addRef();
        }

        /// bind to an already reference counted object
        explicit Ptr(RefCountedT &p) : ptr(&p) {
            ptr->addRef();
        }

        /// bind to an existing ptr
        Ptr(const Ptr &other) : ptr(other.ptr) {
            ptr->addRef();
        }

        Ptr(Ptr &&other) noexcept : ptr(other.ptr) {
            other.ptr = nullptr;
        }

        ~Ptr() {
            if (ptr != nullptr && ptr->release() == 0) {
                delete ptr;
            }
        }

        Ptr &operator=(const Ptr &other) {
            if (this != &other) {
                if (ptr && ptr->release() == 0) {
                    delete ptr;
                }
                ptr = other.ptr;
                ptr->addRef();
            }

            return *this;
        }

        Ptr &operator=(Ptr &&other) noexcept {
            if (this != &other) {
                if (ptr && ptr->release() == 0) {
                    delete ptr;
                }
                ptr = other.ptr;
                other.ptr = nullptr;
            }

            return *this;
        }

        RefCountedT *operator->() const {
            return ptr;
        }

        RefCountedT &operator*() const {
            return *ptr;
        }

        void swap(Ptr &other) noexcept {
            std::swap(ptr, other.ptr);
        }

        bool operator<(const Ptr &other) const {
            return ptr < other.ptr;
        }

        bool operator==(const Ptr &other) const {
            return ptr == other.ptr;
        }

    private:
        RefCountedT *ptr = nullptr;
    };

}// namespace jazz

#endif//BOOLEAN_ALGEBRA_PTR_H
