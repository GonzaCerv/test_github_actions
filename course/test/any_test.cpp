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

// Testing
#include <gtest/gtest.h>

using namespace ::testing;

namespace behavior_tree {

namespace test {

class AnyTests : public Test {
   public:
    AnyTests() {}
};

TEST_F(AnyTests, AnyTestCorrectCast) {
    Any uut_string_(std::string("Hello!"));
    Any uut_uint32_(uint32_t(10));
    Any uut_double_(10.0);

    EXPECT_EQ("Hello!", uut_string_.get<std::string>());
    EXPECT_EQ(uint32_t(10), uut_uint32_.get<uint32_t>());
    EXPECT_EQ(10.0, uut_double_.get<double>());
}

TEST_F(AnyTests, AnyTestIncorrectCast) {
    Any uut_string_(std::string("Hello!"));
    Any uut_uint32_(uint32_t(10));
    Any uut_double_(10.0);

    EXPECT_THROW({ uut_string_.get<int>(); }, std::runtime_error);
    EXPECT_THROW({ uut_uint32_.get<double>(); }, std::runtime_error);
    EXPECT_THROW({ uut_double_.get<uint32_t>(); }, std::runtime_error);
}

}  // namespace test

}  // namespace behavior_tree

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
