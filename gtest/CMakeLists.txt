# Sample example CMake configuration
add_executable(experiment1 main.cpp add.cpp)

add_executable(sample_test test.cpp add.cpp)
target_link_libraries(sample_test gtest_main)

include(GoogleTest)
gtest_discover_tests(sample_test)
