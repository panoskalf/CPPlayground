# Cross-platform clean script
# Usage: cmake -P clean.cmake

message(STATUS "Cleaning build directory...")

# Remove build directory if it exists
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/build")
    file(REMOVE_RECURSE "${CMAKE_CURRENT_LIST_DIR}/build")
    message(STATUS "Removed build directory")
else()
    message(STATUS "Build directory does not exist")
endif()

message(STATUS "Clean completed!")