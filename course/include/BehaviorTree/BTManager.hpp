/**
 * @file BTManager.hpp
 * @author Gonzalo Cervetti (g.cervetti@ekumenlabs.com)
 * @brief Manages the creation and execution of the nodes in BT
 * @version 0.1
 * @date 2020-06-27
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

// Standard include
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// Challenge includes
#include "BehaviorTree/Blackboard.hpp"
#include "BehaviorTree/NodeInterface.hpp"

namespace behavior_tree {
class BTManager {
   public:
    /**
     * @brief Construct a new BTManager object
     *
     */
    BTManager() {
        blackboard_ = std::make_shared<Blackboard>();
        root_node_name_ = "";
    };

    /**
     * @brief Destructor
     */
    ~BTManager() = default;

    /**
     * @brief Emplaces a new node into the node_pool.
     *
     * @tparam T NodeInterface
     * @param name name of the node to be injected
     * @param children Vector with a list of children of that node.
     * @return std::shared_ptr<T>& shared pointer with the reference of the node created.
     */
    template <typename NodeType, class... U>
    std::shared_ptr<NodeType> makeNode(U&&... args) {
        auto new_node = std::make_shared<NodeType>(std::forward<U>(args)...);

        auto search = node_pool_.find(new_node->getName());
        if (search != node_pool_.end()) {
            throw std::runtime_error("Node name duplicated");
        }
        // Add the root to the pool.
        node_pool_.emplace(std::make_pair(new_node->getName(), new_node));
        // Store the name of the last node that its emplaced into the map.
        // This node will be the first called (the root node) when the run function gets called.
        root_node_name_ = new_node->getName();
        return new_node;
    }

    /**
     * @brief Runs the current Behavior Tree
     * @param max_tick_count maximum amount of ticks that the BT is going to be using.
     * @return Noderesult returns SuCESS when the execution has finished and it was SUCCESS, FAILURE otherwhise
     */
    NodeResult run(uint32_t max_tick_count) {
        NodeResult result = NodeResult::RUNNING;
        uint32_t tick_count = 0;

        if (node_pool_.empty()) {
            throw std::runtime_error("node pool is empty");
        }

        // Get the last node in the node_pool
        auto root_node = node_pool_[root_node_name_];

        while ((result == NodeResult::RUNNING) && (max_tick_count > tick_count)) {
            result = root_node->tick();
            ++tick_count;
        }
        if (tick_count == max_tick_count) {
            return NodeResult::FAILURE;
        }
        return result;
    };

   private:
    std::map<std::string, std::shared_ptr<NodeInterface>> node_pool_;
    std::shared_ptr<Blackboard> blackboard_;
    std::string root_node_name_;
};

}  // namespace behavior_tree