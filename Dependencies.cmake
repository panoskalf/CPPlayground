include(FetchContent)

# GoogleTest - Modern FetchContent approach
find_package(GTest 1.12.1 QUIET)
if (NOT GTest_FOUND)
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_Declare(
        googletest
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )

    # This does everything: download, extract, and add_subdirectory
    FetchContent_MakeAvailable(googletest)
endif()

# Organize in IDE (Visual Studio/CLion)
set_target_properties(gtest gtest_main gmock gmock_main
    PROPERTIES FOLDER "Dependencies/GoogleTest"
)