/**
 * @file component.h 
 * @author Jiazhen LUO
 * @date 2024/7/30
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

#ifndef JAZZ_BOOLEAN_ALGEBRA_COMPONENT_H
#define JAZZ_BOOLEAN_ALGEBRA_COMPONENT_H

#include "jazz/config.h"
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef JAZZ_DEBUG
#define OVERRIDE_COMPUTE(classname)               \
    void compute() override {                     \
        printf("%s: %s\n", #classname, __func__); \
        Component::compute();                     \
    }

#define OVERRIDE_MAKE_OUT_OF_DATE(classname)      \
    void makeOutOfDate() override {               \
        printf("%s: %s\n", #classname, __func__); \
        Component::makeOutOfDate();               \
    }

#define OVERRIDE_FOR_DEBUG(classname) \
    OVERRIDE_COMPUTE(classname)       \
    OVERRIDE_MAKE_OUT_OF_DATE(classname)
#else
#define OVERRIDE_FOR_DEBUG(classname)
#endif

namespace jazz {
    namespace simulation {

        using IntegerType = int;
        using UnsignedType = unsigned int;

        class InOut {
        public:
            enum {
                MAX_BIT_WIDTH = sizeof(IntegerType) * 8,
            };

            enum TYPE {
                INPUT = 0,
                OUTPUT = 1,
            };

        public:
            InOut() = default;
            InOut(std::string name, int width, TYPE type) : m_name(std::move(name)), m_width(width), m_type(type) {}
            ~InOut() = default;

            void setName(const std::string &name) {
                m_name = name;
            }
            const std::string &name() const {
                return m_name;
            }

            void setWidth(int width) {
                m_width = width;
            }

            int width() const {
                return m_width;
            }

            void setValue(const bool *value) {
                for (int i = 0; i < m_width; i++) {
                    m_value[i] = value[i];
                }
            }

            void setValue(UnsignedType value) {
                for (int i = 0; i < m_width; i++) {
                    m_value[i] = (value >> i) & 1;
                }
            }

            void setValue(const InOut &other) {
                for (int i = 0; i < m_width; i++) {
                    m_value[i] = other.m_value[i];
                }
            }

            const bool *value() const {
                return m_value;
            }

            UnsignedType uintValue() const {
                UnsignedType result = 0;
                for (int i = 0; i < m_width; i++) {
                    result |= (m_value[i] << i);
                }
                return result;
            }

            IntegerType intValue() const {
                return static_cast<IntegerType>(uintValue());
            }

            const bool bit(int i) const {
                JAZZ_ASSERT(i < m_width);
                return m_value[i];
            }

            bool &bit(int i) {
                JAZZ_ASSERT(i < m_width);
                return m_value[i];
            }

            const bool operator[](int i) const {
                return bit(i);
            }

            bool &operator[](int i) {
                return bit(i);
            }

            TYPE type() const {
                return m_type;
            }

        private:
            std::string m_name;
            int m_width = 1;
            /// The value of the in/output, stored in little-endian order.
            bool m_value[MAX_BIT_WIDTH] = {false};
            TYPE m_type = INPUT;
        };

        /**
         * @brief The base class of all components.
         * A component is a logic gate or a combination of logic gates.
         */
        class Component {
        public:
            struct ComponentAndPort {
                Component *component;
                InOut *port;
            };

        protected:
            Component() = default;

        public:
            virtual ~Component();

            unsigned int numInput() const {
                return m_ins.size();
            }

            unsigned int numOutput() const {
                return m_outs.size();
            }

            /**
             * @brief Get the name of the component.
             *
             * @note The name of a component is unique
             * @return
             */
            virtual const char *name() const = 0;

            /**
             * @brief Get the label of the component.
             *
             * @return
             */
            const char *label() const {
                return m_label.c_str();
            }

            unsigned int countNandGate() const;

            InOut *get(const char *name) {
                auto found = m_in_outs_lut.find(name);
                return found == m_in_outs_lut.end() ? nullptr : found->second;
            }

            InOut *in(const char *name) {
                auto found = m_in_outs_lut.find(name);
                return found == m_in_outs_lut.end() ? nullptr : (found->second->type() == InOut::INPUT ? found->second : nullptr);
            }

            InOut *in(int i) {
                return m_ins[i];
            }

            InOut *out(int i) {
                return m_outs[i];
            }

            InOut *out(const char *name) {
                auto found = m_in_outs_lut.find(name);
                return found == m_in_outs_lut.end() ? nullptr : (found->second->type() == InOut::OUTPUT ? found->second : nullptr);
            }

            /**
             * @brief Connect the input of this component to the output of another component.
             *
             * There are two special cases:
             *  - The input-input case happens when a input is connected to the input of its parent.
             *  - The output-output case happens when a output is connected to the output of a subcomponent.
             * @param in
             * @param source
             * @return
             */
            bool connect(InOut *in, const ComponentAndPort &source);

            /**
             * @brief Connect the in of this component to the output of another component.
             * @param in
             * @param c
             * @param out
             * @return
             */
            bool connect(InOut *in, Component *c, InOut *out) {
                return connect(in, {c, out});
            }

            /**
             * @brief Connect the in of this component to the output of another component.
             * @param in_name The name of the in of this component.
             * @param c
             * @param out_name
             * @return
             */
            bool connect(const char *in_name, Component *c, const char *out_name) {
                return connect(in(in_name), c, c->get(out_name));
            }

            /**
             * @brief Disconnect the input of this component.
             * @param in
             * @return
             */
            bool disconnect(InOut *in);

            JAZZ_DEBUG_VIRTUAL void compute();

            /**
             * @brief Set the value of a input of this component to be the given value.
             * @param in
             * @param value
             */
            void setInputValue(InOut *in, bool value);

            /**
             * @brief Set the value of a input of this component to be the given value.
             * @param in
             * @param value
             */
            void setInputValue(InOut *in, const bool *value);

            /**
             * @brief Set the value of a input of this component to be the value of another port.
             * @param in
             * @param value
             */
            void setInputValue(InOut *in, InOut *value);


            struct Replacement {
                Component *component{nullptr};
                std::vector<Component *> parts;
            };

            void unpack(int level = 1);

            bool saveAsDot(const char *filename) const;

        protected:
            InOut *addIn(const char *name, int width) {
                m_ins.emplace_back(new InOut(name, width, InOut::INPUT));
                auto b = m_in_outs_lut.insert(std::make_pair(name, m_ins.back()));
                if (b.second) {
                    return m_ins.back();
                } else {
                    m_ins.pop_back();
                    return nullptr;
                }
            }

            InOut *addOut(const char *name, int width) {
                m_outs.emplace_back(new InOut(name, width, InOut::OUTPUT));
                auto b = m_in_outs_lut.insert(std::make_pair(name, m_outs.back()));
                if (b.second) {
                    return m_outs.back();
                } else {
                    m_outs.pop_back();
                    return nullptr;
                }
            }

            Component *addComponent(Component *c) {
                c->m_parent = this;
                m_sub_components.push_back(c);
                return c;
            }

            JAZZ_DEBUG_VIRTUAL void makeOutOfDate();

            const std::vector<InOut *> &allInput() {
                return m_ins;
            }

            const std::vector<InOut *> &allOutput() {
                return m_outs;
            }

            ComponentAndPort inputSource(InOut *in, bool recursive = true);

            bool replaceWithParts(Replacement &replacement);

        private:
            /**
             * @brief Set the output of this component to the output of a subcomponent.
             * @param out
             * @param c
             * @param c_out
             */
            void setOutput(InOut *out, Component *c, InOut *c_out) {
                if (out && c && c_out) {
                    if (c_out->width() == out->width()) {
                        m_output_map[out] = {c, c_out};
                        c->m_output_dest[c_out].push_back({this, out});
                    } else {
                        m_output_map.erase(out);
                    }
                }
            }

            void disconnectAll();
            void disconnectOutToIn(InOut *out, InOut *in);
            void disconnectInToIn(InOut *in_from, InOut *in_to);
            void disconnectOutToOut(InOut *out_from, InOut *out_to);

            bool isConnected(InOut *in) {
                return m_input_src.find(in) != m_input_src.end();
            }

            bool isConnected(InOut *in, InOut *out) {
                auto found = m_input_src.find(in);
                return found != m_input_src.end() && found->second.port == out;
            }

            void clearDeadConnections();

        protected:
            std::string m_label;

            /// All the inputs and outputs of the component
            std::vector<InOut *> m_ins;
            std::vector<InOut *> m_outs;
            /// Look-up table for inputs and outputs
            std::unordered_map<std::string, InOut *> m_in_outs_lut;

            /// Sub-components of the component
            std::vector<Component *> m_sub_components;

            // Record the data flow.
            /// Describe the signal source of each input.
            /// If an input is connected to the output of another component, the input is mapped to the output of that component.
            std::unordered_map<InOut *, ComponentAndPort> m_input_src;
            /// If this component has subcomponents, a output of this component is mapped to the output of a subcomponent.
            std::unordered_map<InOut *, ComponentAndPort> m_output_map;
            /// If this component has subcomponents, a input of this component is link to the input of a subcomponent.
            std::unordered_map<InOut *, std::vector<ComponentAndPort>> m_input_dest;
            /// The destination of the output of this component.
            std::unordered_map<InOut *, std::vector<ComponentAndPort>> m_output_dest;

            bool m_is_computed = false;

            Component *m_parent = nullptr;
        };
    }// namespace simulation
}// namespace jazz

#endif//JAZZ_BOOLEAN_ALGEBRA_COMPONENT_H
