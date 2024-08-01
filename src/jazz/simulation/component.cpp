/**
 * @file component.cpp 
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

#include "component.h"
#include <cstdio>

jazz::simulation::Component::~Component() {

    disconnectAll();

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
unsigned int jazz::simulation::Component::countNandGate() const {
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
bool jazz::simulation::Component::connect(jazz::simulation::InOut *in, const jazz::simulation::Component::ComponentAndPort &source) {
    if (in && source.component && source.port && in->width() == source.port->width()) {
        if (in->type() == InOut::INPUT) {
            auto found = m_input_src.find(in);
            if (found != m_input_src.end()) {
                // The input is already connected to another component.
                if (found->second.component == source.component && found->second.port == source.port) {
                    // The input is connected to the same source.
                    return true;
                } else {
                    // Disconnect the input from the source component.
                    disconnect(in);
                }
            }
            m_input_src[in] = source;
            if (source.port->type() == InOut::OUTPUT) {
                source.component->m_output_dest[source.port].push_back({this, in});
                return true;
            } else {// case input-input
                if (m_parent == source.component || m_parent->m_parent == source.component) {
                    // The source component is the parent of this component.
                    // The input of this component is connected to the input of the parent component.
                    source.component->m_input_dest[source.port].push_back({this, in});
                    return true;
                }
            }
        } else {
            // case output-output
            if (source.port->type() == InOut::OUTPUT) {
                if (source.component->m_parent == this || source.component->m_parent->m_parent == this) {
                    // The source component is the subcomponent of this component.
                    // The output of this component is connected to the output of the subcomponent.
                    auto found = m_output_map.find(in);
                    if (found != m_output_map.end()) {
                        // The output is already connected to a port
                        if (found->second.component == source.component && found->second.port == source.port) {
                            // The output is connected to the same source.
                            return true;
                        } else {
                            // Disconnect the output from the source component.
                            disconnect(in);
                        }
                    }

                    m_output_map[in] = source;
                    source.component->m_output_dest[source.port].push_back({this, in});
                    return true;
                }
            }
        }
    }
    return false;
}
bool jazz::simulation::Component::disconnect(jazz::simulation::InOut *in) {
    if (in) {
        if (in->type() == InOut::INPUT) {
            auto found = m_input_src.find(in);
            if (found != m_input_src.end()) {
                auto &source = found->second;
                if (source.port->type() == InOut::OUTPUT) {
                    source.component->disconnectOutToIn(source.port, in);
                } else {
                    source.component->disconnectInToIn(source.port, in);
                }
                m_input_src.erase(in);
                return true;
            }
        } else {
            auto found = m_output_map.find(in);
            if (found != m_output_map.end()) {
                auto &source = found->second;
                if (source.port->type() == InOut::OUTPUT) {
                    source.component->disconnectOutToOut(source.port, in);
                }
                m_output_map.erase(in);
                return true;
            }
        }
    }
    return false;
}
void jazz::simulation::Component::compute() {
    if (!m_is_computed) {
        for (auto &output : m_outs) {
            if (m_output_map.find(output) != m_output_map.end()) {
                // It's connected to a subcomponent's output.
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
void jazz::simulation::Component::setInputValue(jazz::simulation::InOut *in, bool value) {
    JAZZ_ASSERT(in->width() == 1);
    if (in->bit(0) != value) {
        in->bit(0) = value;
        makeOutOfDate();
    }
}
void jazz::simulation::Component::setInputValue(jazz::simulation::InOut *in, const bool *value) {
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
void jazz::simulation::Component::setInputValue(jazz::simulation::InOut *in, jazz::simulation::InOut *value) {
    setInputValue(in, value->value());
}
jazz::simulation::Component::ComponentAndPort jazz::simulation::Component::inputSource(jazz::simulation::InOut *in, bool recursive) {
    ComponentAndPort source{this, in};
    auto found = m_input_src.find(in);
    bool is_connected = found != m_input_src.end();

    if (is_connected && !recursive) {
        return found->second;
    } else {
        while (is_connected) {
            source = found->second;
            found = source.component->m_input_src.find(source.port);
            is_connected = found != source.component->m_input_src.end();
        }
        return source;
    }
}
void jazz::simulation::Component::disconnectOutToIn(InOut *out, InOut *in) {
    auto found = m_output_dest.find(out);
    if (found != m_output_dest.end()) {
        std::for_each(found->second.begin(), found->second.end(), [&](auto &item) {
            if (item.port == in) {
                item.port = nullptr;
                item.component = nullptr;
            }
        });
    }
}
void jazz::simulation::Component::disconnectInToIn(jazz::simulation::InOut *in_from, jazz::simulation::InOut *in_to) {
    auto found = m_input_dest.find(in_from);
    if (found != m_input_dest.end()) {
        std::for_each(found->second.begin(), found->second.end(), [&](auto &item) {
            if (item.port == in_to) {
                item.port = nullptr;
                item.component = nullptr;
            }
        });
    }
}
void jazz::simulation::Component::disconnectOutToOut(jazz::simulation::InOut *out_from, jazz::simulation::InOut *out_to) {
    auto found = m_output_dest.find(out_from);
    if (found != m_output_dest.end()) {
        std::for_each(found->second.begin(), found->second.end(), [&](auto &item) {
            if (item.port == out_to) {
                item.port = nullptr;
                item.component = nullptr;
            }
        });
    }
}
bool jazz::simulation::Component::saveAsDot(const char *filename) const {
    FILE *fp = fopen(filename, "w");
    if (fp == nullptr) return false;

    fprintf(fp, "digraph %s {\n", name());
    fprintf(fp, "rankdir=LR\n");
    fprintf(fp, "node [shape=record;style=filled;fillcolor=gray95]\n");
    fprintf(fp, "graph [label=\"%s\"]\n", name());

    unsigned int node_serial = 0;

    std::unordered_map<InOut *, unsigned int> port_serial;
    std::unordered_map<Component *, unsigned int> component_serial;

    // input ports
    for (auto &in : m_ins) {
        port_serial[in] = node_serial;
        fprintf(fp, "n_%d [label=\"%s\"; shape=circle]\n", node_serial++, in->name().c_str());
    }
    // output ports
    for (auto &out : m_outs) {
        port_serial[out] = node_serial;
        fprintf(fp, "n_%d [label=\"%s\"; shape=circle]\n", node_serial++, out->name().c_str());
    }

    // subcomponents
    for (auto &component : m_sub_components) {
        component_serial[component] = node_serial;
        fprintf(fp, "n_%d [shape=plain\n", node_serial++);
        auto name = component->name();
        auto num_input = component->numInput();
        auto num_output = component->numOutput();
        auto num_row = std::max(num_input, num_output);
        fprintf(fp, R"(label=<<table border="0" cellborder="1" cellspacing="0" cellpadding="1">)");
        fprintf(fp, "<tr><td><b>%s</b></td></tr>\n", name);
        fprintf(fp, "<tr><td>\n");
        fprintf(fp, "<table border=\"0\" cellborder=\"0\" cellspacing=\"0\">\n");
        for (int i = 0; i < num_row; i++) {
            fprintf(fp, "<tr>");
            if (i < num_input) {
                fprintf(fp, R"(<td port="%s" align="left">%s</td>)", component->in(i)->name().c_str(), component->in(i)->name().c_str());
            } else {
                fprintf(fp, "<td></td>");
            }
            fprintf(fp, "<td> &nbsp;&nbsp;&nbsp;&nbsp; </td>");
            if (i < num_output) {
                fprintf(fp, R"(<td port="%s" align="right">%s</td>)", component->out(i)->name().c_str(), component->out(i)->name().c_str());
            } else {
                fprintf(fp, "<td></td>");
            }
            fprintf(fp, "</tr>\n");
        }
        fprintf(fp, "</table>\n");
        fprintf(fp, "</td></tr>\n");
        fprintf(fp, "</table>>]\n");
    }

    // connections
    for (auto &sub : m_sub_components) {
        for (auto &in : sub->m_ins) {
            auto source = sub->inputSource(in, false);
            if (source.component != nullptr) {
                if (source.component == this) {
                    // The source is an input/out port of this component.
                    fprintf(fp, "n_%d -> n_%d:%s\n",
                            port_serial[source.port],
                            component_serial[sub],
                            in->name().c_str());
                } else {
                    fprintf(fp, "n_%d:%s -> n_%d:%s\n",
                            component_serial[source.component],
                            source.port->name().c_str(),
                            component_serial[sub],
                            in->name().c_str());
                }
            }
        }
    }

    for (auto &[out, source] : m_output_map) {
        fprintf(fp, "n_%d:%s -> n_%d\n",
                component_serial[source.component],
                source.port->name().c_str(),
                port_serial[out]);
    }

    fprintf(fp, "}\n");
    fclose(fp);
    return true;
}
bool jazz::simulation::Component::replaceWithParts(jazz::simulation::Component::Replacement &replacement) {
    if (m_parent == nullptr || m_sub_components.empty()) {
        return false;
    }

    // reconnection.,
    // For example: A -> B  => A -> B1, A -> B2
    for (auto &pair : m_input_dest) {
        auto source = inputSource(pair.first);
        // make a copy because connect() may change the value of pair.second
        auto receivers = pair.second;
        for (auto &receiver : receivers) {
            receiver.component->connect(receiver.port, source);
            JAZZ_ASSERT(receiver.component->isConnected(receiver.port, source.port));
        }
    }

    // For example: A -> B  => A1 -> B, A2 -> B
    for (auto &pair : m_output_dest) {
        auto source = m_output_map[pair.first];
        for (auto &receiver : pair.second) {
            receiver.component->connect(receiver.port, source);
        }
    }

    // move the subcomponents to the parent
    replacement.parts = m_sub_components;
    replacement.component = this;
    return true;
}
void jazz::simulation::Component::makeOutOfDate() {
    m_is_computed = false;

    // subcomponents
    for (auto &dest : m_input_dest) {
        for (auto &component_and_port : dest.second) {
            if (component_and_port.component != nullptr) {
                component_and_port.component->makeOutOfDate();
            }
        }
    }

    // other components connected to the output of this component
    for (auto &dest : m_output_dest) {
        for (auto &component_and_port : dest.second) {
            if (component_and_port.component != m_parent && component_and_port.component != nullptr) {
                component_and_port.component->makeOutOfDate();
            }
        }
    }
}
void jazz::simulation::Component::unpack(int level) {
    while (level > 0) {
        std::vector<Replacement> replacements;
        for (auto &subcomponent : m_sub_components) {
            replacements.emplace_back();
            subcomponent->replaceWithParts(replacements.back());
        }

        for (auto &replacement : replacements) {

            if (replacement.component == nullptr) {
                continue;
            }

            // erase this component from the parent's subcomponents
            for (auto it = m_sub_components.begin(); it != m_sub_components.end(); it++) {
                if (*it == replacement.component) {
                    m_sub_components.erase(it);
                    break;
                }
            }

            // delete the subcomponent
            replacement.component->m_sub_components.clear();
            delete replacement.component;

            for (auto &component : replacement.parts) {
                component->m_parent = this;
                m_sub_components.push_back(component);
            }
        }

        clearDeadConnections();
        level--;
    }
}
void jazz::simulation::Component::disconnectAll() {
    // Disconnect all the inputs
    for (auto &in : m_ins) {
        disconnect(in);
    }

    for (auto &[in, dest] : m_input_dest) {
        for (auto &receiver : dest) {
            receiver.component->disconnect(receiver.port);
        }
    }

    // Disconnect all the outputs
    for (auto &out : m_outs) {
        disconnect(out);
    }

    for (auto &[out, dest] : m_output_dest) {
        for (auto &receiver : dest) {
            receiver.component->disconnect(receiver.port);
        }
    }
}
void jazz::simulation::Component::clearDeadConnections() {
    // if a destination is dead(null), remove it.
    for (auto &dest : m_input_dest) {
        auto &receivers = dest.second;
        receivers.erase(std::remove_if(receivers.begin(), receivers.end(), [](ComponentAndPort &receiver) {
                            return receiver.component == nullptr;
                        }),
                        receivers.end());
    }

    // remove empty destinations
    for (auto first = m_input_dest.begin(), last = m_input_dest.end(); first != last;) {
        if (first->second.empty())
            first = m_input_dest.erase(first);
        else
            ++first;
    }
}
