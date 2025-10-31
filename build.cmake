# Cross-platform build script
# Usage: cmake -P build.cmake

# Detect platform and set defaults
if(WIN32)
    set(DEFAULT_GENERATOR "MinGW Makefiles")
    set(DEFAULT_CONFIG "Debug")
elseif(UNIX AND NOT APPLE)
    set(DEFAULT_GENERATOR "Unix Makefiles")
    set(DEFAULT_CONFIG "Debug")
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# Allow override via environment or command line
if(NOT DEFINED GENERATOR)
    set(GENERATOR ${DEFAULT_GENERATOR})
endif()

if(NOT DEFINED BUILD_TYPE)
    set(BUILD_TYPE ${DEFAULT_CONFIG})
endif()

message(STATUS "Building with generator: ${GENERATOR}")
message(STATUS "Build type: ${BUILD_TYPE}")

# Configure
execute_process(
    COMMAND ${CMAKE_COMMAND} -S . -B build -G "${GENERATOR}" -DCMAKE_BUILD_TYPE=${BUILD_TYPE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    RESULT_VARIABLE CONFIGURE_RESULT
)

if(NOT CONFIGURE_RESULT EQUAL 0)
    message(FATAL_ERROR "CMake configure failed")
endif()

# Build
execute_process(
    COMMAND ${CMAKE_COMMAND} --build build --config ${BUILD_TYPE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    RESULT_VARIABLE BUILD_RESULT
)

if(NOT BUILD_RESULT EQUAL 0)
    message(FATAL_ERROR "Build failed")
endif()

message(STATUS "Build completed successfully!")