cmake_minimum_required (VERSION 2.8.8)
project(cpp_course)

# The version number.
set (app_VERSION_MAJOR 1)
set (app_VERSION_MINOR 0)

# GCC flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17")
set(GTEST_INCLUDE_DIRS /usr/src/gtest)
set(GMOCK_INCLUDE_DIRS /usr/src/gmock)

# Include paths.
include_directories(
	include
)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)

# Application sources.
set (APP_SOURCES
	src/main.cc
)

# Application creation and linkeage to libraries.
add_executable(cpp_course ${APP_SOURCES})
target_link_libraries (cpp_course 
)

################################
# Testing
################################
# Enable Testing
enable_testing()


  enable_testing()

  ##############
  # Unit Tests
  ##############
  # Tests for Behavior Tree nodes
  add_executable(runBTNodesTests test/bt_nodes_test.cpp)
  target_link_libraries(runBTNodesTests gtest gtest_main gmock gmock_main)
  add_test(NAME bt_nodes_test COMMAND runBTNodesTests)

  # Tests for object any
  add_executable(any_tests test/any_test.cpp)
  target_link_libraries(any_tests gtest gtest_main gmock gmock_main)
  add_test(NAME any_test COMMAND any_tests)

  # Tests for blackboard
  add_executable(blackboard_tests test/blackboard_test.cpp)
  target_link_libraries(blackboard_tests gtest gtest_main gmock gmock_main)
  add_test(NAME blackboard_test COMMAND blackboard_tests)

  # Tests for BtManager
  add_executable(bt_manager_tests test/bt_manager_tests.cpp)
  target_link_libraries(bt_manager_tests gtest gtest_main gmock gmock_main)
  add_test(NAME bt_manager_test COMMAND bt_manager_tests)

#endif()
