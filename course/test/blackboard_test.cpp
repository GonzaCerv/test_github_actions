/**
 * @file any_test.cpp
 * @author Gonzalo Cervetti (gcervetti@ekumenlabs.com)
 * @brief Provides tests for the Any class.
 * @version 0.1
 * @date 2020-06-10
 *
 * @copyright Copyright (c) 2020
 *
 */

// Standard includes
#include <cstdint>
#include <memory>

// Challenge includes
#include "BehaviorTree/Any.hpp"
#include "BehaviorTree/Blackboard.hpp"

// Testing
#include <gtest/gtest.h>

using namespace ::testing;

namespace behavior_tree {

namespace test {

class BlackboardTest : public Test {
   public:
    BlackboardTest() {}
};

TEST_F(BlackboardTest, RetrieveUnexistentValue) {
    // Create the blackboard
    Blackboard uut_;

    // Search for something that doesn't exists.
    EXPECT_THROW({ uut_.get("any_string"); }, std::out_of_range);
}

TEST_F(BlackboardTest, BlackboardSetAndGet) {
    Any any_string_(std::string("Hello!"));
    Any any_uint32_(uint32_t(10));
    Any any_double_(10.0);

    // Create the blackboard
    Blackboard uut_;

    // Set values in the blackboard
    uut_.set("any_string", any_string_);
    uut_.set("any_int32", any_uint32_);
    uut_.set("any_double", any_double_);

    // Get
    Any result_string = uut_.get("any_string");
    Any result_int32 = uut_.get("any_int32");
    Any result_double = uut_.get("any_double");

    EXPECT_EQ(any_string_.get<std::string>(), result_string.get<std::string>());
    EXPECT_EQ(any_uint32_.get<uint32_t>(), result_int32.get<uint32_t>());
    EXPECT_EQ(any_double_.get<double>(), result_double.get<double>());
}

}  // namespace test

}  // namespace behavior_tree

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
