#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "string_utilities.h"

// Test fixture for string utilities
class StringUtilitiesTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Common test data
        empty_str = "";
        whitespace_str = "  \t\n\r  ";
        mixed_case_str = "HeLLo WoRLd";
        numeric_str = "12345";
        alpha_str = "abcdef";
        alphanum_str = "abc123";
        csv_data = "apple,banana,cherry";
    }

    std::string empty_str;
    std::string whitespace_str;
    std::string mixed_case_str;
    std::string numeric_str;
    std::string alpha_str;
    std::string alphanum_str;
    std::string csv_data;
};

// Test case conversion functions
TEST_F(StringUtilitiesTest, CaseConversion) {
    EXPECT_EQ(string_utils::to_upper("hello"), "HELLO");
    EXPECT_EQ(string_utils::to_upper("Hello World!"), "HELLO WORLD!");
    EXPECT_EQ(string_utils::to_upper("123abc"), "123ABC");
    EXPECT_EQ(string_utils::to_upper(""), "");

    EXPECT_EQ(string_utils::to_lower("HELLO"), "hello");
    EXPECT_EQ(string_utils::to_lower("Hello World!"), "hello world!");
    EXPECT_EQ(string_utils::to_lower("123ABC"), "123abc");
    EXPECT_EQ(string_utils::to_lower(""), "");
}

// Test trimming functions
TEST_F(StringUtilitiesTest, TrimmingOperations) {
    EXPECT_EQ(string_utils::trim_left("  hello  "), "hello  ");
    EXPECT_EQ(string_utils::trim_right("  hello  "), "  hello");
    EXPECT_EQ(string_utils::trim("  hello  "), "hello");

    EXPECT_EQ(string_utils::trim("\t\n hello \r\n"), "hello");
    EXPECT_EQ(string_utils::trim(""), "");
    EXPECT_EQ(string_utils::trim("   "), "");
    EXPECT_EQ(string_utils::trim("nospaces"), "nospaces");
}

// Test string splitting
TEST_F(StringUtilitiesTest, StringSplitting) {
    auto result = string_utils::split("a,b,c", ',');
    EXPECT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[1], "b");
    EXPECT_EQ(result[2], "c");

    auto result2 = string_utils::split("hello::world::test", "::");
    EXPECT_EQ(result2.size(), 3);
    EXPECT_EQ(result2[0], "hello");
    EXPECT_EQ(result2[1], "world");
    EXPECT_EQ(result2[2], "test");

    auto empty_result = string_utils::split("", ',');
    EXPECT_EQ(empty_result.size(), 1);
    EXPECT_EQ(empty_result[0], "");
}

// Test string validation functions
TEST_F(StringUtilitiesTest, StringValidation) {
    // Numeric validation
    EXPECT_TRUE(string_utils::is_numeric("123"));
    EXPECT_TRUE(string_utils::is_numeric("-123"));
    EXPECT_TRUE(string_utils::is_numeric("+123"));
    EXPECT_TRUE(string_utils::is_numeric("123.45"));
    EXPECT_TRUE(string_utils::is_numeric("-123.45"));
    EXPECT_FALSE(string_utils::is_numeric(""));
    EXPECT_FALSE(string_utils::is_numeric("abc"));
    EXPECT_FALSE(string_utils::is_numeric("12.34.56"));
    EXPECT_FALSE(string_utils::is_numeric("12a34"));

    // Alpha validation
    EXPECT_TRUE(string_utils::is_alpha("abc"));
    EXPECT_TRUE(string_utils::is_alpha("ABC"));
    EXPECT_TRUE(string_utils::is_alpha("aBc"));
    EXPECT_FALSE(string_utils::is_alpha(""));
    EXPECT_FALSE(string_utils::is_alpha("abc123"));
    EXPECT_FALSE(string_utils::is_alpha("123"));

    // Alphanumeric validation
    EXPECT_TRUE(string_utils::is_alphanumeric("abc123"));
    EXPECT_TRUE(string_utils::is_alphanumeric("123"));
    EXPECT_TRUE(string_utils::is_alphanumeric("abc"));
    EXPECT_FALSE(string_utils::is_alphanumeric(""));
    EXPECT_FALSE(string_utils::is_alphanumeric("abc 123"));
    EXPECT_FALSE(string_utils::is_alphanumeric("abc-123"));
}

// Test prefix/suffix checking
TEST_F(StringUtilitiesTest, PrefixSuffixChecking) {
    std::string text = "hello world";

    EXPECT_TRUE(string_utils::starts_with(text, "hello"));
    EXPECT_TRUE(string_utils::starts_with(text, ""));
    EXPECT_FALSE(string_utils::starts_with(text, "world"));
    EXPECT_FALSE(string_utils::starts_with(text, "hello world extra"));

    EXPECT_TRUE(string_utils::ends_with(text, "world"));
    EXPECT_TRUE(string_utils::ends_with(text, ""));
    EXPECT_FALSE(string_utils::ends_with(text, "hello"));
    EXPECT_FALSE(string_utils::ends_with(text, "extra hello world"));
}

// Test string replacement
TEST_F(StringUtilitiesTest, StringReplacement) {
    std::string text = "hello world hello universe";

    EXPECT_EQ(string_utils::replace_first(text, "hello", "hi"),
              "hi world hello universe");
    EXPECT_EQ(string_utils::replace_all(text, "hello", "hi"),
              "hi world hi universe");

    EXPECT_EQ(string_utils::replace_all(text, "xyz", "abc"), text);
    EXPECT_EQ(string_utils::replace_all(text, "", "abc"), text);
}

// Test string joining
TEST_F(StringUtilitiesTest, StringJoining) {
    std::vector<std::string> words = {"hello", "world", "test"};

    EXPECT_EQ(string_utils::join(words, " "), "hello world test");
    EXPECT_EQ(string_utils::join(words, ", "), "hello, world, test");
    EXPECT_EQ(string_utils::join(words, ""), "helloworldtest");

    std::vector<std::string> empty_vec;
    EXPECT_EQ(string_utils::join(empty_vec, " "), "");

    std::vector<std::string> single = {"alone"};
    EXPECT_EQ(string_utils::join(single, " "), "alone");
}

// Test search functions
TEST_F(StringUtilitiesTest, SearchOperations) {
    std::string text = "the quick brown fox jumps over the lazy fox";

    auto positions = string_utils::find_all(text, "fox");
    EXPECT_EQ(positions.size(), 2);
    EXPECT_EQ(positions[0], 16);
    EXPECT_EQ(positions[1], 40);

    EXPECT_EQ(string_utils::count_occurrences(text, "the"), 2);
    EXPECT_EQ(string_utils::count_occurrences(text, "fox"), 2);
    EXPECT_EQ(string_utils::count_occurrences(text, "cat"), 0);
    EXPECT_EQ(string_utils::count_occurrences(text, ""), 0);
}

// Test edge cases and boundary conditions
TEST_F(StringUtilitiesTest, EdgeCases) {
    // Empty strings
    EXPECT_EQ(string_utils::to_upper(""), "");
    EXPECT_EQ(string_utils::trim(""), "");
    EXPECT_FALSE(string_utils::is_numeric(""));
    EXPECT_EQ(string_utils::replace_all("", "a", "b"), "");

    // Single character strings
    EXPECT_EQ(string_utils::to_upper("a"), "A");
    EXPECT_EQ(string_utils::trim(" "), "");
    EXPECT_TRUE(string_utils::is_alpha("a"));

    // Very long strings (basic test for performance)
    std::string long_str(10000, 'a');
    EXPECT_EQ(string_utils::to_upper(long_str), std::string(10000, 'A'));
}

// Performance-oriented tests
TEST_F(StringUtilitiesTest, BasicPerformanceChecks) {
    // Test that operations don't cause excessive copying
    std::string large_text(1000, 'x');
    large_text += "find_me";
    large_text += std::string(1000, 'y');

    // These operations should complete in reasonable time
    auto result = string_utils::find_all(large_text, "find_me");
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], 1000);

    size_t count = string_utils::count_occurrences(large_text, "x");
    EXPECT_EQ(count, 1000);
}