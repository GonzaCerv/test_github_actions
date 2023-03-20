/**
 * @file Blackboard.hpp
 * @author Gonzalo Cervetti (g.cervetti@ekumenlabs.com)
 * @brief This class allows to store values for BT
 * @version 0.1
 * @date 2020-06-22
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

// Standard include
#include <map>
#include <memory>
#include <string>

// Challenge includes
#include <BehaviorTree/Any.hpp>

namespace behavior_tree {
class Blackboard {
   public:
    /**
     * @brief Construct a new Blackboard object
     *
     */
    Blackboard() = default;

    /**
     * @brief Destroy the Blackboard object
     *
     */
    ~Blackboard() = default;

    /**
     * @brief Set an object into the blackboard
     *
     * @param key key string where the object should be stored
     * @param object object to be stored.
     */
    void set(const std::string& key, const Any& object) {
        database_[key] = object;
    }

    /**
     * @brief Get an object from blackboard
     *
     * @param key string with the object to be retrieved
     */
    const Any& get(const std::string& key) const { return database_.at(key); }

   private:
    std::map<std::string, Any> database_;
};

}  // namespace behavior_tree