#pragma once
#include <string>
#include <vector>

namespace string_utils {

// Case conversion utilities
std::string to_upper(const std::string& str);
std::string to_lower(const std::string& str);

// Trimming utilities
std::string trim_left(const std::string& str);
std::string trim_right(const std::string& str);
std::string trim(const std::string& str);

// String splitting and tokenization
std::vector<std::string> split(const std::string& str, char delimiter);
std::vector<std::string> split(const std::string& str, const std::string& delimiter);

// String validation utilities
bool is_numeric(const std::string& str);
bool is_alpha(const std::string& str);
bool is_alphanumeric(const std::string& str);
bool starts_with(const std::string& str, const std::string& prefix);
bool ends_with(const std::string& str, const std::string& suffix);

// String replacement utilities
std::string replace_all(const std::string& str, const std::string& from, const std::string& to);
std::string replace_first(const std::string& str, const std::string& from, const std::string& to);

// String joining
std::string join(const std::vector<std::string>& strings, const std::string& delimiter);

// Advanced search utilities
std::vector<size_t> find_all(const std::string& str, const std::string& pattern);
size_t count_occurrences(const std::string& str, const std::string& pattern);

} // namespace string_utils