# String utilities CMake configuration

# Demo executable
add_executable(string_demo string_demo.cpp string_utilities.cpp)

# Test executable with GoogleTest
add_executable(string_test test_string_utilities.cpp string_utilities.cpp)
target_link_libraries(string_test gtest_main)

include(GoogleTest)
gtest_discover_tests(string_test)