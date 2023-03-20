/**
 * @file NegationNode.hpp
 * @author Gonzalo Cervetti (gcervetti@ekumenlabs.com)
 * @brief Behavior of the node:
 *          - With every call to tick, it will tick its child.
 *          - When the child returns RUNNING, it returns RUNNING.
 *          - When the child returns SUCCESS returns FAILURE, otherwise will return SUCCESS.
 *
 * @version 0.1
 * @date 2020-06-01
 *
 * @copyright Copyright (c) 2020
 *
 */
#pragma once

// Standard libraries
#include <memory>
#include <stdexcept>

// Challenge includes
#include "BehaviorTree/NodeInterface.hpp"
#include "BehaviorTree/NodeUtils.hpp"

namespace behavior_tree {

class NegationNode : public NodeInterface {
   public:
    explicit NegationNode(const std::string& name, std::shared_ptr<NodeInterface> child)
        : NodeInterface(name), child_{child} {
        if (child_ == nullptr) {
            throw std::invalid_argument("Child cannot be nullptr");
        }
    };
    /**
     * @brief this method implements the functionality of the negation node.
     *
     * @returns RUNNING if the node didn't finish, SUCESS or FAILURE otherwise.
     */
    NodeResult tick() override {
        const NodeResult result = child_->tick();
        if (result == NodeResult::SUCCESS) {
            return NodeResult::FAILURE;
        } else if (result == NodeResult::FAILURE) {
            return NodeResult::SUCCESS;
        }
        return result;
    }

   private:
    std::shared_ptr<NodeInterface> child_;
};
}  // namespace behavior_tree