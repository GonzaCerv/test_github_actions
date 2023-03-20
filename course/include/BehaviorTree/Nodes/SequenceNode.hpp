/**
 * @file SequenceNode.hpp
 * @author Gonzalo Cervetti (gcervetti@ekumenlabs.com)
 * @brief Behavior of the node:
 *          - Until it finishes its execution, it will return RUNNING.
 *          - With every call to tick, it will tick one of its children in order.
 *          - When the child returns SUCCESS it will tick the next child.
 *          - When the child returns FAILURE it will be restart the execution of all the nodes.
 *          - When the child returns RUNNING it will tick it again.
 *          - When all the children return SUCCESS, it will return SUCESS.
 *
 * @version 0.1
 * @date 2020-06-01
 *
 * @copyright Copyright (c) 2020
 *
 */
#pragma once

// Standard libraries
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

// Challenge includes
#include "BehaviorTree/NodeInterface.hpp"
#include "BehaviorTree/NodeUtils.hpp"

namespace behavior_tree {

class SequenceNode : public NodeInterface {
   public:
    explicit SequenceNode(const std::string& name, std::vector<std::shared_ptr<NodeInterface>> children)
        : NodeInterface(name), children_{children} {
        for (auto child = children_.begin(); child != children_.end(); ++child) {
            if (*child == nullptr) {
                throw std::invalid_argument("child cannot be nullptr");
            }
        }
        children_count_index_ = 0;
    };
    /**
     * @brief this method implements the functionality of the Sequence node.
     *
     * @returns RUNNING if the node didn't finish, SUCESS or FAILURE otherwise.
     */
    NodeResult tick() override {
        if (children_.size() != 0) {
            const NodeResult result = children_[children_count_index_]->tick();
            // When the child returns RUNNING it will tick it again.
            if (result == NodeResult::RUNNING) {
                return result;
            }
            // When the child returns FAILURE it will be restart the execution of all the nodes.
            else if (result == NodeResult::FAILURE) {
                children_count_index_ = 0;
                return NodeResult::RUNNING;
            }
            ++children_count_index_;
            // When all the children return SUCCESS, it will return SUCESS.
            if (children_count_index_ == (children_.size())) {
                return NodeResult::SUCCESS;
            }

            // Point to the next child to be ticked.
            return NodeResult::RUNNING;
        }
        return NodeResult::SUCCESS;
    }

   private:
    std::vector<std::shared_ptr<NodeInterface>> children_;
    uint32_t children_count_index_;
};
}  // namespace behavior_tree