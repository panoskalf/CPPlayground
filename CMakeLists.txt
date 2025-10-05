cmake_minimum_required(VERSION 3.14)
project(playground)

# Enable testing
include(CTest)

# Dependencies
include(Dependencies.cmake)

# Add subdirectories for each example/experiment
add_subdirectory(sample)
add_subdirectory(string_utilities)
add_subdirectory(fixed_point)
add_subdirectory(pointers)
add_subdirectory(raii_utils)
add_subdirectory(beej_play_with_sockets)
