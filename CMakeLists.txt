cmake_minimum_required(VERSION 3.20)
project(playground)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
include(CTest)

# Dependencies
include(Dependencies.cmake)

# Add subdirectories for each example/experiment
add_subdirectory(raii_utils) # library first
add_subdirectory(beej_play_with_sockets) # examples that use library
add_subdirectory(string_utilities)
add_subdirectory(fixed_point)
add_subdirectory(pointers)
