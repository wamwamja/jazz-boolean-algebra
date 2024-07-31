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

#ifndef BOOLEAN_ALGEBRA_CLASS_HIERARCHY_H
#define BOOLEAN_ALGEBRA_CLASS_HIERARCHY_H

#include <cstring>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace jazz {
    /**
     * @brief ClassHierarchy is a template class that provides a mechanism for registering classes and their parent classes.
     *
     * @tparam Info The type of the class information. requires
     *          - const char* className() const
     *          - const char* parentName() const
     *          - const std::type_info& typeInfo() const
     */
    template<typename Info>
    class ClassHierarchy {
    public:
        explicit ClassHierarchy(const Info &info) : class_info(info), next(root), parent(nullptr) {
            root = this;
            parentsIdentified = false;
        }

        ClassHierarchy *getParent() const {
            if (!parentsIdentified) {
                identifyParents();
            }
            return parent;
        }

        static const ClassHierarchy *find(const std::string &name);

        static void dumpHierarchy(std::ostream &os, bool verbose = false);

        Info class_info;

    private:
        struct TreeNode {
            explicit TreeNode(ClassHierarchy *info) : info(info) {}
            void addChild(TreeNode *child) {
                children.push_back(child);
            }
            std::vector<TreeNode *> children;
            ClassHierarchy *info = nullptr;
        };

        static void dumpTree(std::ostream &os, TreeNode *node, const std::string &prefix, bool verbose);
        static void identifyParents();

        static ClassHierarchy *root;
        ClassHierarchy *next = nullptr;
        mutable ClassHierarchy *parent = nullptr;

        static bool parentsIdentified;
    };

    template<typename Info>
    void ClassHierarchy<Info>::identifyParents() {
        if (!parentsIdentified) {
            for (auto info = root; info; info = info->next) {
                const char *parentName = info->class_info.parentName();
                for (auto parent = root; parent; parent = parent->next) {
                    if (std::strcmp(parent->class_info.className(), parentName) == 0) {
                        info->parent = parent;
                        break;
                    }
                }
            }
            parentsIdentified = true;
        }
    }

    template<typename Info>
    void ClassHierarchy<Info>::dumpHierarchy(std::ostream &os, bool verbose) {
        std::vector<TreeNode> nodes;
        for (auto info = root; info; info = info->next) {
            nodes.emplace_back(info);
        }

        TreeNode *rootNode = nullptr;
        for (auto &node : nodes) {
            auto p = node.class_info->getParent();
            if (p) {
                for (auto &parentNode : nodes) {
                    if (parentNode.class_info == p) {
                        parentNode.addChild(&node);
                        break;
                    }
                }
            } else {
                if (rootNode) {
                    throw std::runtime_error("ClassInfo::dumpHierarchy: multiple root classes found.");
                }
                rootNode = &node;
            }
        }

        dumpTree(os, rootNode, "", verbose);
    }

    template<typename Info>
    void ClassHierarchy<Info>::dumpTree(std::ostream &os, TreeNode *node, const std::string &prefix, bool verbose) {
        auto name = node->info->class_info.className();
        os << std::setw(10) << std::left << std::setfill(' ') << name;
        if (verbose) {
            os << " [ID 0x" << std::hex << std::setw(8) << std::setfill('0') << &node->info->class_info.typeInfo() << std::dec << "]" << std::endl;
        }

        auto num_children = node->children.size();
        for (size_t i = 0; i < num_children; ++i) {
            if (verbose) {
                std::cout << prefix << " +- ";
                if (i == num_children - 1)
                    dumpTree(os, node->children[i], prefix + "    ", verbose);
                else
                    dumpTree(os, node->children[i], prefix + " |  ", verbose);
            } else {
                std::string spaces(std::strlen(name), ' ');
                if (i > 0)
                    os << prefix << spaces;
                if (num_children == 1)
                    os << " --- ";
                else if (i > 0)
                    os << "  +- ";
                else
                    os << " -+- ";
                if (i == num_children - 1)
                    dumpTree(os, node->children[i], prefix + spaces + "     ", verbose);
                else
                    dumpTree(os, node->children[i], prefix + spaces + "  |  ", verbose);
            }
        }
    }

    template<typename Info>
    const ClassHierarchy<Info> *ClassHierarchy<Info>::find(const std::string &name) {
        static std::unordered_map<std::string, const ClassHierarchy<Info> *> name2info;
        static bool initialized = false;
        if (!initialized) {
            initialized = true;
            for (auto info = root; info; info = info->next) {
                name2info[info->class_info.className()] = info;
            }
        }

        auto it = name2info.find(name);
        if (it == name2info.end()) {
            throw std::runtime_error("ClassInfo::find: class '" + name + "' not registered.");
        } else
            return it->second;
    }

    template<typename Info>
    ClassHierarchy<Info> *ClassHierarchy<Info>::root = nullptr;
    template<typename Info>
    bool ClassHierarchy<Info>::parentsIdentified = false;

}// namespace jazz

#endif//BOOLEAN_ALGEBRA_CLASS_HIERARCHY_H
