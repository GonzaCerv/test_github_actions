/**
 * @file bt_nodes_test.cc
 * @author Gonzalo Cervetti (gcervetti@ekumenlabs.com)
 * @brief Test for all types of possible nodes of the BT.
 * @version 0.1
 * @date 2020-06-01
 *
 * @copyright Copyright (c) 2020
 *
 */

// Standard includes
#include <memory>
#include <vector>

// Challenge includes
#include "BehaviorTree/NodeInterface.hpp"
#include "BehaviorTree/NodeUtils.hpp"
#include "BehaviorTree/Nodes/FallbackNode.hpp"
#include "BehaviorTree/Nodes/NegationNode.hpp"
#include "BehaviorTree/Nodes/SequenceNode.hpp"

// Testing
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

namespace behavior_tree {

namespace test {

class ChildNodeMock : public NodeInterface {
   public:
    ChildNodeMock() : NodeInterface("mocked_child") {}
    MOCK_METHOD0(tick, NodeResult());
};

class GtestGmockTests : public Test {
};

// Test for checking the case when an nullptr child is passed as parameter.
TEST_F(GtestGmockTests, NegationNodeEmptyChildTest) {
    EXPECT_THROW({ std::unique_ptr<NegationNode> uut_(new NegationNode("node", nullptr)); }, std::invalid_argument);
}

// Test for checking all the possible return values of tick.
TEST_F(GtestGmockTests, NegationNodeTickTest) {
    std::shared_ptr<ChildNodeMock> child_node_mock = std::make_shared<ChildNodeMock>();
    std::unique_ptr<NegationNode> uut_ = std::make_unique<NegationNode>("node", child_node_mock);

    // Set Expectations
    EXPECT_CALL(*child_node_mock, tick()).WillOnce(Return(NodeResult::RUNNING));
    // Test
    EXPECT_EQ(NodeResult::RUNNING, uut_->tick());

    // Set Expectations
    EXPECT_CALL(*child_node_mock, tick()).WillOnce(Return(NodeResult::SUCCESS));
    // Test
    EXPECT_EQ(NodeResult::FAILURE, uut_->tick());

    // Set Expectations
    EXPECT_CALL(*child_node_mock, tick()).WillOnce(Return(NodeResult::FAILURE));
    // Test
    EXPECT_EQ(NodeResult::SUCCESS, uut_->tick());
}

// Test for checking the case when an nullptr child is passed as parameter.
TEST_F(GtestGmockTests, SequenceNodeEmptyChildTest) {
    std::shared_ptr<ChildNodeMock> empty_child_node_mock;
    std::vector<std::shared_ptr<NodeInterface>> empty_children_nodes;
    empty_children_nodes.emplace_back(empty_child_node_mock);

    EXPECT_THROW({ std::unique_ptr<SequenceNode> uut_(new SequenceNode("node", empty_children_nodes)); },
                 std::invalid_argument);
}

// Testcase in which the child is running. In this case
// it should keep ticking the same node.
TEST_F(GtestGmockTests, SequenceNodeRunningResult) {
    // Object to check sequence of expectations.
    Sequence seq;

    // Children creation.
    auto child_node_mock_1 = std::make_shared<ChildNodeMock>();
    auto child_node_mock_2 = std::make_shared<ChildNodeMock>();

    // Crete the vector and populates it.
    std::vector<std::shared_ptr<NodeInterface>> children_nodes;
    children_nodes.emplace_back(child_node_mock_1);
    children_nodes.emplace_back(child_node_mock_2);

    // uut
    std::unique_ptr<SequenceNode> uut_(new SequenceNode("node", children_nodes));
    EXPECT_CALL(*child_node_mock_1, tick())
        .InSequence(seq)
        .WillOnce(Return(NodeResult::RUNNING))
        .WillOnce(Return(NodeResult::RUNNING));
    EXPECT_CALL(*child_node_mock_2, tick()).Times(0).InSequence(seq);
    // Test
    EXPECT_EQ(NodeResult::RUNNING, uut_->tick());
    EXPECT_EQ(NodeResult::RUNNING, uut_->tick());
}

// Testcase in which the first child return success. It should
// call the next child until returns sucess or failure
TEST_F(GtestGmockTests, SequenceNodeTickNextAfterSuccess) {
    // Object to check sequence of expectations.
    Sequence seq;

    // Children creation.
    auto child_node_mock_1 = std::make_shared<ChildNodeMock>();
    auto child_node_mock_2 = std::make_shared<ChildNodeMock>();

    // Crete the vector and populates it.
    std::vector<std::shared_ptr<NodeInterface>> children_nodes;
    children_nodes.emplace_back(child_node_mock_1);
    children_nodes.emplace_back(child_node_mock_2);

    // uut
    std::unique_ptr<SequenceNode> uut_(new SequenceNode("node", children_nodes));

    EXPECT_CALL(*child_node_mock_1, tick()).InSequence(seq).WillOnce(Return(NodeResult::SUCCESS));

    EXPECT_CALL(*child_node_mock_2, tick())
        .Times(2)
        .InSequence(seq)
        .WillOnce(Return(NodeResult::RUNNING))
        .WillOnce(Return(NodeResult::RUNNING));
    // Test
    EXPECT_EQ(NodeResult::RUNNING, uut_->tick());
    EXPECT_EQ(NodeResult::RUNNING, uut_->tick());
    EXPECT_EQ(NodeResult::RUNNING, uut_->tick());
}

// Testcase in which the first child return success. It should
// call the next child until returns sucess or failure
TEST_F(GtestGmockTests, SequenceNodeRestartSequenceAfterFailure) {
    // Object to check sequence of expectations.
    Sequence seq;

    // Children creation.
    auto child_node_mock_1 = std::make_shared<ChildNodeMock>();
    auto child_node_mock_2 = std::make_shared<ChildNodeMock>();

    // Crete the vector and populates it.
    std::vector<std::shared_ptr<NodeInterface>> children_nodes;
    children_nodes.emplace_back(child_node_mock_1);
    children_nodes.emplace_back(child_node_mock_2);

    // uut
    std::unique_ptr<SequenceNode> uut_(new SequenceNode("node", children_nodes));

    EXPECT_CALL(*child_node_mock_1, tick()).InSequence(seq).WillOnce(Return(NodeResult::SUCCESS));
    EXPECT_CALL(*child_node_mock_2, tick()).InSequence(seq).WillOnce(Return(NodeResult::FAILURE));
    EXPECT_CALL(*child_node_mock_1, tick()).InSequence(seq).WillOnce(Return(NodeResult::RUNNING));
    // Test
    EXPECT_EQ(NodeResult::RUNNING, uut_->tick());
    EXPECT_EQ(NodeResult::RUNNING, uut_->tick());
    EXPECT_EQ(NodeResult::RUNNING, uut_->tick());
}

// Testcase in which return success after all the children returned success.
TEST_F(GtestGmockTests, SequenceNodeFinishesSequence) {
    // Object to check sequence of expectations.
    Sequence seq;

    // Children creation.
    auto child_node_mock_1 = std::make_shared<ChildNodeMock>();
    auto child_node_mock_2 = std::make_shared<ChildNodeMock>();

    // Crete the vector and populates it.
    std::vector<std::shared_ptr<NodeInterface>> children_nodes;
    children_nodes.emplace_back(child_node_mock_1);
    children_nodes.emplace_back(child_node_mock_2);

    // uut
    std::unique_ptr<SequenceNode> uut_(new SequenceNode("node", children_nodes));

    EXPECT_CALL(*child_node_mock_1, tick()).InSequence(seq).WillOnce(Return(NodeResult::SUCCESS));
    EXPECT_CALL(*child_node_mock_2, tick()).InSequence(seq).WillOnce(Return(NodeResult::SUCCESS));

    // Test
    EXPECT_EQ(NodeResult::RUNNING, uut_->tick());
    EXPECT_EQ(NodeResult::SUCCESS, uut_->tick());
}

// Test for checking the case when an nullptr child is passed as parameter.
TEST_F(GtestGmockTests, FallbackNodeEmptyChildTest) {
    std::shared_ptr<ChildNodeMock> empty_child_node_mock;
    std::vector<std::shared_ptr<NodeInterface>> empty_children_nodes;
    empty_children_nodes.emplace_back(empty_child_node_mock);

    EXPECT_THROW({ std::make_shared<FallbackNode>("node", empty_children_nodes); }, std::invalid_argument);
}

// Testcase in which the child is running. In this case
// it should keep ticking the same node.
TEST_F(GtestGmockTests, FallbackNodeRunningResult) {
    // Object to check sequence of expectations.
    Sequence seq;

    // Children creation.
    auto child_node_mock_1 = std::make_shared<ChildNodeMock>();
    auto child_node_mock_2 = std::make_shared<ChildNodeMock>();

    EXPECT_CALL(*child_node_mock_1, tick())
        .InSequence(seq)
        .WillOnce(Return(NodeResult::RUNNING))
        .WillOnce(Return(NodeResult::RUNNING));
    EXPECT_CALL(*child_node_mock_2, tick()).Times(0).InSequence(seq);

    // Crete the vector and populates it.
    std::vector<std::shared_ptr<NodeInterface>> children_nodes;
    children_nodes.emplace_back(child_node_mock_1);
    children_nodes.emplace_back(child_node_mock_2);

    // uut
    auto uut_ = std::make_shared<FallbackNode>("node", children_nodes);

    // Test
    EXPECT_EQ(NodeResult::RUNNING, uut_->tick());
    EXPECT_EQ(NodeResult::RUNNING, uut_->tick());
}

// Testcase in which the first child returns success. It shouldn't
// call the next child.
TEST_F(GtestGmockTests, FallbackNodeReturnSuccessWhenChildSucceeded) {
    // Object to check sequence of expectations.
    Sequence seq;

    // Children creation.
    auto child_node_mock_1 = std::make_shared<ChildNodeMock>();
    auto child_node_mock_2 = std::make_shared<ChildNodeMock>();

    EXPECT_CALL(*child_node_mock_1, tick())
        .InSequence(seq)
        .WillOnce(Return(NodeResult::RUNNING))
        .WillOnce(Return(NodeResult::SUCCESS));

    EXPECT_CALL(*child_node_mock_2, tick()).Times(0).InSequence(seq);

    // Crete the vector and populates it.
    std::vector<std::shared_ptr<NodeInterface>> children_nodes;
    children_nodes.emplace_back(child_node_mock_1);
    children_nodes.emplace_back(child_node_mock_2);

    // uut
    auto uut_ = std::make_shared<FallbackNode>("node", children_nodes);

    // Test
    EXPECT_EQ(NodeResult::RUNNING, uut_->tick());
    EXPECT_EQ(NodeResult::SUCCESS, uut_->tick());
}

// Testcase in which the first child returns failure. It should
// call the next child up to the end of the children list.
// If one of them returns SUCCESS, stops and returns
TEST_F(GtestGmockTests, FallbackNodeTickNextWhenFailing) {
    // Object to check sequence of expectations.
    Sequence seq;

    // Children creation.
    auto child_node_mock_1 = std::make_shared<ChildNodeMock>();
    auto child_node_mock_2 = std::make_shared<ChildNodeMock>();

    EXPECT_CALL(*child_node_mock_1, tick()).InSequence(seq).WillOnce(Return(NodeResult::FAILURE));
    EXPECT_CALL(*child_node_mock_2, tick()).InSequence(seq).WillOnce(Return(NodeResult::SUCCESS));

    // Crete the vector and populates it.
    std::vector<std::shared_ptr<NodeInterface>> children_nodes;
    children_nodes.emplace_back(child_node_mock_1);
    children_nodes.emplace_back(child_node_mock_2);

    // uut
    auto uut_ = std::make_shared<FallbackNode>("node", children_nodes);

    // Test
    EXPECT_EQ(NodeResult::RUNNING, uut_->tick());
    EXPECT_EQ(NodeResult::SUCCESS, uut_->tick());
}

// Testcase in which checks what happends at the end of the execution.
// If the last child returns failure, it returns failure too.
TEST_F(GtestGmockTests, FallbackNodeReturningFailureWhenAllFailure) {
    // Object to check sequence of expectations.
    Sequence seq;

    // Children creation.
    auto child_node_mock_1 = std::make_shared<ChildNodeMock>();
    auto child_node_mock_2 = std::make_shared<ChildNodeMock>();

    EXPECT_CALL(*child_node_mock_1, tick()).InSequence(seq).WillOnce(Return(NodeResult::FAILURE));
    EXPECT_CALL(*child_node_mock_2, tick()).InSequence(seq).WillOnce(Return(NodeResult::FAILURE));

    // Crete the vector and populates it.
    std::vector<std::shared_ptr<NodeInterface>> children_nodes;
    children_nodes.emplace_back(child_node_mock_1);
    children_nodes.emplace_back(child_node_mock_2);

    // uut
    auto uut_ = std::make_shared<FallbackNode>("node", children_nodes);

    // Test
    EXPECT_EQ(NodeResult::RUNNING, uut_->tick());
    EXPECT_EQ(NodeResult::FAILURE, uut_->tick());
}

}  // namespace test

}  // namespace behavior_tree

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
