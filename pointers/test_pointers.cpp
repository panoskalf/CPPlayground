#include <string>
#include <chrono>
#include "gtest/gtest.h"
// Disable logging during tests for clean output and accurate timing
#define DUMMY_DISABLE_LOGGING
#include "DummyRaw.h"
#include "DummyShared.h"
#include "DummyUnique.h"

template <typename T>
class TypedTest : public ::testing::Test {};

using DummyObjectTypes = ::testing::Types<DummyRaw, DummyShared, DummyUnique>;

TYPED_TEST_SUITE(TypedTest, DummyObjectTypes);

// Test basic constructor creates proper deep copies of string and allocates value
TYPED_TEST(TypedTest, Constructor)
{
    const char* n = "test";
    TypeParam a(n, 10);

    EXPECT_STREQ(a.getName(), "test");  // Content matches
    EXPECT_NE(a.getName(), n);          // But pointer differs (deep copy)
    EXPECT_EQ(*(a.getValue()), 10);     // Value correctly stored
}

// Test copy constructor performs deep copy (Rule of 3/5)
TYPED_TEST(TypedTest, CopyConstructor)
{
    // for DummyUnique copy is not supported
    if constexpr (TypeParam::supports_copy) {

        const char* n = "test";
        TypeParam a(n, 10);
        TypeParam b(a);  // Copy constructor called

        EXPECT_STREQ(a.getName(), b.getName());     // Content identical
        EXPECT_NE(a.getName(), b.getName());        // But pointers differ (deep copy)
        EXPECT_NE(b.getName(), n);                  // Neither points to original literal
        EXPECT_EQ(*(a.getValue()), *(b.getValue())); // Values identical
        if constexpr (a.is_copy_deep) {
            // Value pointers differ (by design)
            EXPECT_NE(a.getValue(), b.getValue());
        } else {
            // shared pointer checks
            EXPECT_EQ(a.getSharedPtr().use_count(), 2);
        }
    } else if constexpr (!TypeParam::supports_copy) {
        GTEST_SKIP() << "Skipping test for non-copyable type: "
                     << typeid(TypeParam).name() << std::endl;
    }
}

// Test copy assignment operator performs deep copy (Rule of 3/5)
TYPED_TEST(TypedTest, CopyAssignment)
{
    // for DummyUnique copy is not supported
    if constexpr (TypeParam::supports_copy) {

        TypeParam a("first", 10);
        TypeParam b("second", 20);

        a = b;  // Copy assignment operator called

        EXPECT_STREQ(a.getName(), b.getName());     // Content copied
        EXPECT_NE(a.getName(), b.getName());        // Pointers still different
        EXPECT_EQ(*(a.getValue()), *(b.getValue())); // Values copied
        if constexpr (a.is_copy_deep) {
            // Value pointers differ (by design)
            EXPECT_NE(a.getValue(), b.getValue());
        } else {
            // shared pointer checks
            EXPECT_EQ(a.getSharedPtr().use_count(), 2);
        }
    } else if constexpr (!TypeParam::supports_copy) {
        GTEST_SKIP() << "Skipping test for non-copyable type: "
                     << typeid(TypeParam).name() << std::endl;
    }
}

// Test self-assignment safety - should be no-op without crashes
TYPED_TEST(TypedTest, CopyAssignmentSelf)
{
    // for DummyUnique copy is not supported
    if constexpr (TypeParam::supports_copy) {

        TypeParam a("original", 42);

        // Store original state to verify preservation
        const char* original_name = a.getName();
        const int* original_value_ptr = a.getValue();
        const int original_value = *(a.getValue());

        a = a;  // Self-assignment - should detect and return early

        // Object state should be completely unchanged
        EXPECT_STREQ(a.getName(), "original");      // Content preserved
        EXPECT_EQ(*(a.getValue()), original_value); // Value preserved
        EXPECT_EQ(a.getName(), original_name);      // No reallocation occurred
        EXPECT_EQ(a.getValue(), original_value_ptr); // No reallocation occurred
    } else if constexpr (!TypeParam::supports_copy) {
        GTEST_SKIP() << "Skipping test for non-copyable type: "
                     << typeid(TypeParam).name() << std::endl;
    }
}

// Test move constructor transfers ownership (Rule of 5)
TYPED_TEST(TypedTest, MoveConstructor)
{
    const char* n = "test";
    TypeParam a(n, 10);
    TypeParam b(std::move(a));  // Move constructor called

    EXPECT_STRNE(a.getName(), b.getName());  // Names differ (a is null)
    EXPECT_STREQ(b.getName(), n);            // b has the content
    EXPECT_NE(b.getName(), n);               // But not the original pointer
    EXPECT_NE(a.getName(), b.getName());     // Pointers are different
    EXPECT_EQ(10, *(b.getValue()));          // Value transferred to destination
    EXPECT_EQ(a.getValue(), nullptr);        // Source value pointer nulled
    EXPECT_NE(a.getValue(), b.getValue());   // Pointers definitely different
}

// Test move assignment operator transfers ownership (Rule of 5)
TYPED_TEST(TypedTest, MoveAssignment)
{
    const char* n = "second";
    const int v = 20;
    TypeParam a("first", 10);
    TypeParam b(n, v);

    a = std::move(b);  // Move assignment operator called

    EXPECT_STREQ(a.getName(), n);         // a now has b's content
    if constexpr (std::is_same_v<TypeParam, DummyRaw>) {
        EXPECT_EQ(b.getName(), nullptr);      // moved from ptr gets nulled (by design)
    } else if constexpr (std::is_same_v<TypeParam, DummyShared>) {
        EXPECT_NE(a.getName(), nullptr);      // std::string raw ptr points to valid data
    }
    EXPECT_NE(a.getName(), b.getName());  // Pointers different (one is null)
    EXPECT_EQ(*(a.getValue()), v);        // a has b's value
    EXPECT_EQ(b.getValue(), nullptr);     // b's value pointer nulled
    EXPECT_NE(a.getValue(), b.getValue()); // Pointers different (one is null)
}

// Test self-move assignment - implementation-defined behavior but shouldn't crash
TYPED_TEST(TypedTest, MoveAssignmentSelf)
{
    TypeParam a("original", 42);

    // Store original state - behavior after self-move is implementation-defined
    const char* original_name = a.getName();
    const int* original_value_ptr = a.getValue();
    const int original_value = *(a.getValue());

    a = std::move(a);  // Self-move - undefined but should be safe

    // Implementation may preserve state OR move to itself (both valid)
    // Main requirement: no crashes or memory corruption
    if(a.getValue()) {
        EXPECT_EQ(*(a.getValue()), original_value);  // If preserved
        EXPECT_EQ(a.getValue(), original_value_ptr);
    } else {
        EXPECT_EQ(a.getValue(), nullptr);           // If moved-from state
    }

    if constexpr (std::is_same_v<TypeParam, DummyRaw>) {
        EXPECT_STREQ(a.getName(), "original");      // state preserved
        EXPECT_EQ(a.getName(), original_name);      // ptr state preserved
    } else if constexpr (std::is_same_v<TypeParam, DummyShared>) {
        EXPECT_STREQ(a.getName(), "");              // implementation specific
    }
}

TYPED_TEST(TypedTest, ExceptionSafety)
{
    if constexpr (TypeParam::supports_copy)
    {
        // Test what happens when allocation fails
        // Mock allocation failure scenarios

        // Test with insufficient memory
        TypeParam a("short", 1);
        const size_t BIG_SIZE = 1024;
        std::string long_str(BIG_SIZE, 'x');
        TypeParam b(long_str.c_str(), 2);

        // This should either succeed or throw, but not corrupt memory
        EXPECT_NO_THROW(a = b);
    }
}

TYPED_TEST(TypedTest, EdgeCases)
{
    // Test with very long strings
    const size_t BIG_SIZE = 10000;
    std::string long_str(BIG_SIZE, 'a');

    EXPECT_NO_THROW(TypeParam a(long_str.c_str(), INT_MAX));

    // Test with special characters
    EXPECT_NO_THROW(TypeParam b("test\n\t\r\0embedded", 42));

    // Test with negative values
    EXPECT_NO_THROW(TypeParam c("negative", -999));
}

TYPED_TEST(TypedTest, MoveIsActuallyFast)
{
    // Large string for performance test
    const size_t BIG_SIZE = 50000;  // Larger to make timing more reliable
    std::string long_str(BIG_SIZE, 'b');

    // Warmup to avoid cold cache effects
    for(int i = 0; i < 10; ++i) {
        TypeParam warmup(long_str.c_str(), i);
    }

    // Measure constructor time (should involve allocation)
    auto start_ctor = std::chrono::high_resolution_clock::now();
    TypeParam source(long_str.c_str(), 42);
    auto end_ctor = std::chrono::high_resolution_clock::now();

    // Measure move time (should be just pointer swaps)
    auto start_move = std::chrono::high_resolution_clock::now();
    TypeParam dest = std::move(source);
    auto end_move = std::chrono::high_resolution_clock::now();

    auto duration_ctor = std::chrono::duration_cast<std::chrono::nanoseconds>(end_ctor - start_ctor);
    auto duration_move = std::chrono::duration_cast<std::chrono::nanoseconds>(end_move - start_move);

    // Move should be significantly faster than construction
    // Allow for measurement noise but expect move to be at least 10x faster
    EXPECT_LT(duration_move.count() * 10, duration_ctor.count())
        << "Move took " << duration_move.count() << "ns, "
        << "Constructor took " << duration_ctor.count() << "ns. "
        << "Move should be much faster!";

    // Also verify move actually happened
    if constexpr (std::is_same_v<TypeParam, DummyRaw>) {
        EXPECT_EQ(source.getName(), nullptr);  // Raw pointer explicitly nulled
    } else {
        // std::string moved-from state is valid but unspecified
        // Just verify **dest** got the content (move succeeded)
        EXPECT_NE(dest.getName(), nullptr);
    }
    EXPECT_STREQ(dest.getName(), long_str.c_str());
}