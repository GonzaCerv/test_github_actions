/**
 * @file NodeUtils.hpp
 * @author Gonzalo Cervetti (gcervetti@ekumenlabs.com)
 * @brief Usefull objects implement the Behavior Tree
 * @version 0.1
 * @date 2020-06-01
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

namespace behavior_tree {

/**
 * @brief Possible results of the execution of a node.
 */
enum class NodeResult { SUCCESS, RUNNING, FAILURE };

}  // namespace behavior_tree
