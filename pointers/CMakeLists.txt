add_executable(pointers_demo main.cpp DummyRaw.h)
add_executable(pointers_test test_pointers.cpp DummyRaw.h DummyShared.h DummyUnique.h TrackedInt.h)

target_link_libraries(pointers_test gtest_main)

include(GoogleTest)
gtest_discover_tests(pointers_test)