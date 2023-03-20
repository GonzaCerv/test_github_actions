/**
 * @file NodeInterface.hpp
 * @author Gonzalo Cervetti (gcervetti@ekumenlabs.com)
 * @brief Interface to create all Nodes.
 * @version 0.1
 * @date 2020-06-01
 *
 * @copyright Copyright (c) 2020
 *
 */
#pragma once

// Standard libraries
#include <cstdint>

// Challenge includes
#include "BehaviorTree/NodeUtils.hpp"

namespace behavior_tree {
class NodeInterface {
   public:
    /** @brief Constructor of the class
     *  @param[in] name name of the node.
     *  @param[in] key  order of the nodes to be executed.
     */
    explicit NodeInterface(const std::string& name) : name_{name} {};

    /** @brief Virtual destructor to ensure the proper destruction of derived clases */
    virtual ~NodeInterface() = default;

    /**
     * @brief interface method to implement the execution of the node.
     *
     * @returns RUNNING if the node didn't finish, SUCESS or FAILURE otherwise.
     */
    virtual NodeResult tick() = 0;

    /**
     * @brief returns the name of the current node.
     *
     * @returns String with the name of the node.
     */
    std::string getName() { return name_; }

   private:
    std::string name_;
};
}  // namespace behavior_tree
