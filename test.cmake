# Cross-platform test script
# Usage: cmake -P test.cmake

message(STATUS "Running tests...")

# Check if build directory exists
if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/build")
    message(FATAL_ERROR "Build directory not found. Run build script first.")
endif()

# Run tests
execute_process(
    COMMAND ${CMAKE_COMMAND} --build build --target test
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    RESULT_VARIABLE TEST_RESULT
)

if(NOT TEST_RESULT EQUAL 0)
    message(STATUS "Some tests failed - running with verbose output...")
    execute_process(
        COMMAND ctest --test-dir build --output-on-failure
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    )
else()
    message(STATUS "All tests passed!")
endif()