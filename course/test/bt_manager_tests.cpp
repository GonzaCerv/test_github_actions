/**
 * @file bt_manager_tests.cpp
 * @author Gonzalo Cervetti (gcervetti@ekumenlabs.com)
 * @brief Tests for BTManager
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
#include "BehaviorTree/BTManager.hpp"
#include "BehaviorTree/NodeUtils.hpp"
#include "BehaviorTree/Nodes/NegationNode.hpp"

// Testing
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

namespace behavior_tree {

namespace test {

class ChildNodeMock : public NodeInterface {
   public:
    ChildNodeMock() : NodeInterface("mocked_node") {}
    MOCK_METHOD0(tick, NodeResult());
};

class BTManagerTest : public Test {
   public:
};

// Running a BTManager with an empty node pool should throw an error.
TEST_F(BTManagerTest, RunEmptyNodePoolTest) {
    // Create the blackboard
    BTManager uut_;

    // Test
    EXPECT_THROW({ uut_.run(1); }, std::runtime_error);
}

// Running a BTManager with two nodes with the same name should throw an error.
TEST_F(BTManagerTest, DuplicatedNamesTest) {
    // Create the blackboard
    BTManager uut_;

    // Build the object and set the expectations
    uut_.makeNode<ChildNodeMock>();

    // Test
    EXPECT_THROW({ uut_.makeNode<ChildNodeMock>(); }, std::runtime_error);
}

// Populates the node pool with one node and checks if SUCCEEDED when running the whole BT.
TEST_F(BTManagerTest, RunNodePoolReturnSuccessTest) {
    // Create the blackboard
    BTManager uut_;

    // Build the object and set the expectations
    std::shared_ptr<ChildNodeMock> mock_node = uut_.makeNode<ChildNodeMock>();
    EXPECT_CALL(*mock_node, tick()).WillOnce(Return(NodeResult::SUCCESS));

    // Test
    EXPECT_EQ(uut_.run(2), NodeResult::SUCCESS);
}

// Populates the node pool with one node and checks if FAILS when running the whole BT.
TEST_F(BTManagerTest, RunNodePoolReturnFailureTest) {
    // Create the blackboard
    BTManager uut_;

    // Build the object and set the expectations
    std::shared_ptr<ChildNodeMock> mock_node = uut_.makeNode<ChildNodeMock>();
    EXPECT_CALL(*mock_node, tick()).WillOnce(Return(NodeResult::FAILURE));

    // Test
    EXPECT_EQ(uut_.run(2), NodeResult::FAILURE);
}

// Checks the result of the BT when it needs more ticks than the allowed amount.
TEST_F(BTManagerTest, MoreTicksThanAllowedTest) {
    // Create the blackboard
    BTManager uut_;

    // Build the object and set the expectations
    std::shared_ptr<ChildNodeMock> mock_node = uut_.makeNode<ChildNodeMock>();
    EXPECT_CALL(*mock_node, tick()).WillRepeatedly(Return(NodeResult::RUNNING));

    // Test
    EXPECT_EQ(uut_.run(2), NodeResult::FAILURE);
}

// Checks if it is possible to stack nodes
TEST_F(BTManagerTest, MultipleNodes) {
    // Create the blackboard
    BTManager uut_;

    // Build the object and set the expectations
    std::shared_ptr<ChildNodeMock> mock_node = uut_.makeNode<ChildNodeMock>();
    EXPECT_CALL(*mock_node, tick()).WillRepeatedly(Return(NodeResult::SUCCESS));
    // Create a negation node on top of mock_node.
    std::shared_ptr<NegationNode> negation_node = uut_.makeNode<NegationNode>("negation_1", mock_node);

    // Test
    EXPECT_EQ(uut_.run(2), NodeResult::FAILURE);
}

}  // namespace test

}  // namespace behavior_tree

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
