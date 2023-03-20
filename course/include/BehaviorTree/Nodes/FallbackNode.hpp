/**
 * @file FallbackNode.hpp
 * @author Gonzalo Cervetti (gcervetti@ekumenlabs.com)
 *
 * @version 0.1
 * @date 2020-06-04
 *
 * @copyright Copyright (c) 2020
 *
 */
#pragma once

// Standard libraries
#include <memory>
#include <stdexcept>
#include <vector>

// Challenge includes
#include "BehaviorTree/NodeInterface.hpp"
#include "BehaviorTree/NodeUtils.hpp"

namespace behavior_tree {

/**
 * @brief Behavior of the node:
 *          - Until it finishes its execution, it will return RUNNING.
 *          - With every call to tick, it will tick one of its children in order.
 *          - When a child returns SUCCESS it will stop and return sucess.
 *          - When a child returns FAILURE it will tick the next child.
 *          - When the last child returns FAILURE too it will return FAILURE.
 *          - When the child returns RUNNING it will tick it again.
 *
 */
class FallbackNode : public NodeInterface {
   public:
    FallbackNode(const std::string& name, std::vector<std::shared_ptr<NodeInterface>> children)
        : NodeInterface(name), children_{children} {
        for (auto child : children_) {
            if (child == nullptr) {
                throw std::invalid_argument("child cannot be nullptr");
            }
        }
        children_count_index_ = 0;
    };
    /**
     * @brief interface method to implement the execution of the node.
     *
     * @returns RUNNING if the node didn't finish, SUCESS or FAILURE otherwise.
     */
    NodeResult tick() override {
        if (children_.size() != 0) {
            const auto result = children_[children_count_index_]->tick();

            if ((result == NodeResult::RUNNING) || (result == NodeResult::SUCCESS)) {
                return result;
            }
            ++children_count_index_;
            if (children_count_index_ != children_.size()) {
                return NodeResult::RUNNING;
            }
            return NodeResult::FAILURE;
        }
        return NodeResult::SUCCESS;
    }

   private:
    std::vector<std::shared_ptr<NodeInterface>> children_;
    uint32_t children_count_index_;
};
}  // namespace behavior_tree