#include <iostream>
#include <string>
#include <vector>
#include "string_utilities.h"

void demonstrate_basic_operations() {
    std::cout << "=== Basic String Operations ===\n";

    std::string text = "Hello, World!";
    std::cout << "Original: '" << text << "'\n";
    std::cout << "Length: " << text.length() << "\n";
    std::cout << "Size: " << text.size() << "\n";
    std::cout << "Capacity: " << text.capacity() << "\n";
    std::cout << "Empty: " << std::boolalpha << text.empty() << "\n";

    // Element access
    std::cout << "First char: '" << text.front() << "'\n";
    std::cout << "Last char: '" << text.back() << "'\n";
    std::cout << "Char at index 1: '" << text.at(1) << "'\n";

    std::cout << "\n";
}

void demonstrate_case_conversion() {
    std::cout << "=== Case Conversion ===\n";

    std::string text = "Hello, World! 123";
    std::cout << "Original: '" << text << "'\n";
    std::cout << "Upper: '" << string_utils::to_upper(text) << "'\n";
    std::cout << "Lower: '" << string_utils::to_lower(text) << "'\n";

    std::cout << "\n";
}

void demonstrate_trimming() {
    std::cout << "=== Trimming Operations ===\n";

    std::string text = "  \t  Hello, World!  \n  ";
    std::cout << "Original: '" << text << "' (length: " << text.length() << ")\n";
    std::cout << "Trim left: '" << string_utils::trim_left(text) << "'\n";
    std::cout << "Trim right: '" << string_utils::trim_right(text) << "'\n";
    std::cout << "Trim both: '" << string_utils::trim(text) << "'\n";

    std::cout << "\n";
}

void demonstrate_splitting_joining() {
    std::cout << "=== Splitting and Joining ===\n";

    std::string csv = "apple,banana,cherry,date";
    std::cout << "CSV string: '" << csv << "'\n";

    auto tokens = string_utils::split(csv, ',');
    std::cout << "Split by comma:\n";
    for (size_t i = 0; i < tokens.size(); ++i) {
        std::cout << "  [" << i << "]: '" << tokens[i] << "'\n";
    }

    std::string rejoined = string_utils::join(tokens, " | ");
    std::cout << "Rejoined with ' | ': '" << rejoined << "'\n";

    std::cout << "\n";
}

void demonstrate_validation() {
    std::cout << "=== String Validation ===\n";

    std::vector<std::string> test_strings = {
        "123", "12.34", "-45.67", "abc", "abc123", "123abc", "", "  "
    };

    for (const auto& str : test_strings) {
        std::cout << "'" << str << "' -> ";
        std::cout << "numeric: " << std::boolalpha << string_utils::is_numeric(str);
        std::cout << ", alpha: " << string_utils::is_alpha(str);
        std::cout << ", alphanum: " << string_utils::is_alphanumeric(str) << "\n";
    }

    std::cout << "\n";
}

void demonstrate_search_replace() {
    std::cout << "=== Search and Replace ===\n";

    std::string text = "The quick brown fox jumps over the lazy dog. The fox is quick.";
    std::cout << "Original: '" << text << "'\n";

    // Find operations
    size_t pos = text.find("fox");
    std::cout << "First 'fox' at position: " << pos << "\n";

    auto all_fox = string_utils::find_all(text, "fox");
    std::cout << "All 'fox' positions: ";
    for (size_t p : all_fox) {
        std::cout << p << " ";
    }
    std::cout << "\n";

    std::cout << "Count of 'the' (case sensitive): "
              << string_utils::count_occurrences(text, "the") << "\n";
    std::cout << "Count of 'The' (case sensitive): "
              << string_utils::count_occurrences(text, "The") << "\n";

    // Replace operations
    std::string replaced_first = string_utils::replace_first(text, "fox", "cat");
    std::cout << "Replace first 'fox': '" << replaced_first << "'\n";

    std::string replaced_all = string_utils::replace_all(text, "fox", "cat");
    std::cout << "Replace all 'fox': '" << replaced_all << "'\n";

    std::cout << "\n";
}

int main() {
    std::cout << "C++ String Manipulation & Utilities Demo\n";
    std::cout << "========================================\n\n";

    demonstrate_basic_operations();
    demonstrate_case_conversion();
    demonstrate_trimming();
    demonstrate_splitting_joining();
    demonstrate_validation();
    demonstrate_search_replace();

    std::cout << "Demo completed. Run the test suite for comprehensive validation.\n";

    return 0;
}