/**
 * @file flow.h 
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

#ifndef JAZZ_BOOLEAN_ALGEBRA_FLOW_H
#define JAZZ_BOOLEAN_ALGEBRA_FLOW_H

#include <vector>

namespace jazz {

    class Component;
    class InOut;

    struct ComponentInOut {
        Component *c = nullptr;
        InOut *in_out = nullptr;
        bool up_to_date = false;
    };

    struct Node {
        ComponentInOut *component_in_out = nullptr;
        Node *input = nullptr;
    };

    class Flow {
    public:
        Flow() = default;
        ~Flow() {
            for (auto node : m_nodes) {
                delete node;
            }
        }

        Node *addNode(ComponentInOut *component_in_out, Node *input) {
            m_nodes.push_back(new Node{component_in_out, input});
            return m_nodes.back();
        }

    private:
        std::vector<Node *> m_nodes;
    };

}// namespace jazz

#endif//JAZZ_BOOLEAN_ALGEBRA_FLOW_H
