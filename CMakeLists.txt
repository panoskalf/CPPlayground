cmake_minimum_required(VERSION 3.14)
project(playground)

# Enable testing
include(CTest)

# get Google Test so subprojects can use it
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.12.1.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add subdirectories for each example/experiment
add_subdirectory(sample)
add_subdirectory(string_utilities)
add_subdirectory(fixed_point)
add_subdirectory(pointers)
