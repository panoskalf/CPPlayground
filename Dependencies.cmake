include(FetchContent)

# GoogleTest
find_package(GTest 1.12.1 QUIET)
if (NOT GTest_FOUND)
    FetchContent_Declare(
        googletest
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/google/googletest/archive/release-1.12.1.zip
    )

    FetchContent_GetProperties(googletest)
    if (NOT googletest_POPULATED)
        # Show download progress when actually downloading
        set(FETCHCONTENT_QUIET NO)

        # For Windows: Prevent overriding the parent project's compiler/linker settings
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

        # Download, extract, and add to build system
        FetchContent_Populate(googletest)
        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
    endif()
endif()

# Organize in IDE (Visual Studio/CLion)
set_target_properties(gtest gtest_main gmock gmock_main
    PROPERTIES FOLDER "Dependencies/GoogleTest"
)