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
#else
#define OVERRIDE_COMPUTE(classname)
#endif

namespace jazz {
    namespace simulation {

        using IntegerType = int;
        using UnsignedType = unsigned int;

        class InOut {
        public:
            enum {
                INPUT = 0,
                OUTPUT = 1,
                MAX_BIT_WIDTH = sizeof(IntegerType) * 8,
            };

        public:
            InOut() = default;
            InOut(std::string name, int width, int type) : m_name(std::move(name)), m_width(width), m_type(type) {}
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

            int type() const {
                return m_type;
            }

        private:
            std::string m_name;
            int m_width = 1;
            /// The value of the in/output, stored in little-endian order.
            bool m_value[MAX_BIT_WIDTH] = {false};
            int m_type = 0;
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

        public:
            Component() = default;

            virtual ~Component() {
                for (auto &component : m_sub_components) {
                    delete component;
                }

                for (auto &in_out : m_ins) {
                    delete in_out;
                }

                for (auto &in_out : m_outs) {
                    delete in_out;
                }
            }

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

            unsigned int countNandGate() const {
                if (m_sub_components.empty()) {
                    // If a component has no subcomponent, it must be a Nand gate.
                    return 1;
                } else {
                    unsigned int count = 0;
                    for (auto &sub_component : m_sub_components) {
                        count += sub_component->countNandGate();
                    }
                    return count;
                }
            }

            InOut *get(const char *name) {
                auto found = m_in_outs_lut.find(name);
                return found == m_in_outs_lut.end() ? nullptr : found->second;
            }

            InOut *in(const char *name) {
                auto found = m_in_outs_lut.find(name);
                return found == m_in_outs_lut.end() ? nullptr : (found->second->type() == InOut::INPUT ? found->second : nullptr);
            }

            InOut *out(const char *name) {
                auto found = m_in_outs_lut.find(name);
                return found == m_in_outs_lut.end() ? nullptr : (found->second->type() == InOut::OUTPUT ? found->second : nullptr);
            }

            /**
             * @brief Connect the in of this component to the output of another component.
             * @param in
             * @param source
             * @return
             */
            bool connect(InOut* in, const ComponentAndPort& source){
                JAZZ_ASSERT(in && source.component && source.port);
                if(in && source.component && source.port){
                    m_input_src[in] = source;
                    source.component->m_output_dest[source.port].push_back({this, in});
                    return true;
                }
                return false;
            }

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
             * @param out
             * @return
             */
            bool connect(const char *in_name, Component *c, const char *out) {
                return connect(in(in_name), c, c->get(out));
            }

            bool disconnect(InOut *in) {
                if (in) {
                    m_input_src.erase(in);
                    return true;
                }
                return false;
            }

            virtual void compute() {
                if (!m_is_computed) {
                    for (auto &output : m_outs) {
                        if (m_output_map.find(output) != m_output_map.end()) {
                            auto &component_and_port = m_output_map[output];
                            component_and_port.component->compute();
                            output->setValue(component_and_port.port->value());
                        } else {
                            // It's a nand gate.
                            for (auto &input : m_ins) {
                                auto source = inputSource(input);
                                // The input is connected to another component's output.
                                // Compute the output of the connected component.
                                if (source.component != nullptr && source.port->type() == InOut::OUTPUT) {
                                    source.component->compute();
                                }

                                input->setValue(source.port->value());
                            }

                            // Compute the output of the nand gate.
                            output->bit(0) = !(m_ins[0]->bit(0) && m_ins[1]->bit(0));
                        }
                    }

                    // TODO
                    // Different outputs is computed independently.
                    // It's better to mark each output than the whole component.
                    m_is_computed = true;
                }
            }

            void setInputValue(InOut *in, bool value) {
                JAZZ_ASSERT(in->width() == 1);
                if (in->bit(0) != value) {
                    in->bit(0) = value;
                    makeOutOfDate();
                }
            }

            void setInputValue(InOut *in, const bool *value) {
                bool value_changed = false;
                for (int i = 0; i < in->width(); i++) {
                    if (in->bit(i) != value[i]) {
                        value_changed = true;
                        break;
                    }
                }

                if (value_changed) {
                    in->setValue(value);
                    makeOutOfDate();
                }
            }

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
                m_sub_components.push_back(c);
                return c;
            }

            void makeOutOfDate() {
                m_is_computed = false;
                for (auto &dest : m_output_dest) {
                    for (auto &component_and_port : dest.second) {
                        component_and_port.component->makeOutOfDate();
                    }
                }
            }

            /**
         * @brief Set the output of this component to the output of a subcomponent.
         * @param out
         * @param c
         * @param c_out
         */
            void setOutput(InOut *out, Component *c, InOut *c_out) {
                if (out) {
                    if (c_out && c_out->width() == out->width()) {
                        m_output_map[out] = {c, c_out};
                    } else {
                        m_output_map.erase(out);
                    }
                }
            }

            const std::vector<InOut *> &allInput() {
                return m_ins;
            }

            const std::vector<InOut *> &allOutput() {
                return m_outs;
            }

            ComponentAndPort inputSource(InOut *in) {
                ComponentAndPort source{this, in};
                auto found = m_input_src.find(in);
                bool is_connected = found != m_input_src.end();
                while (is_connected) {
                    source = found->second;
                    found = source.component->m_input_src.find(source.port);
                    is_connected = found != source.component->m_input_src.end();
                }
                return source;
            }

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
            /// If this component has sub-components, a output of this component is mapped to the output of a sub-component.
            std::unordered_map<InOut *, ComponentAndPort> m_output_map;
            /// The destination of the output of this component.
            std::unordered_map<InOut *, std::vector<ComponentAndPort>> m_output_dest;


            bool m_is_computed = false;
        };
    }// namespace simulation
}// namespace jazz

#endif//JAZZ_BOOLEAN_ALGEBRA_COMPONENT_H
