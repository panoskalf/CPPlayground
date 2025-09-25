#include "string_utilities.h"
#include <algorithm>
#include <cctype>
#include <sstream>

namespace string_utils {

std::string to_upper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string to_lower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string trim_left(const std::string& str) {
    auto start = str.find_first_not_of(" \t\n\r");
    return (start == std::string::npos) ? "" : str.substr(start);
}

std::string trim_right(const std::string& str) {
    auto end = str.find_last_not_of(" \t\n\r");
    return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}

std::string trim(const std::string& str) {
    return trim_right(trim_left(str));
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    if (str == "")
    {
        tokens.push_back(str);
    }

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    tokens.push_back(str.substr(start));
    return tokens;
}

bool is_numeric(const std::string& str) {
    if (str.empty()) return false;

    size_t start = 0;
    if (str[0] == '+' || str[0] == '-') start = 1;
    if (start >= str.length()) return false;

    bool has_decimal = false;
    for (size_t i = start; i < str.length(); ++i) {
        if (str[i] == '.') {
            if (has_decimal) return false;
            has_decimal = true;
        } else if (!std::isdigit(str[i])) {
            return false;
        }
    }

    return true;
}

bool is_alpha(const std::string& str) {
    if (str.empty()) return false;
    return std::all_of(str.begin(), str.end(),
                      [](char c) { return std::isalpha(c); });
}

bool is_alphanumeric(const std::string& str) {
    if (str.empty()) return false;
    return std::all_of(str.begin(), str.end(),
                      [](char c) { return std::isalnum(c); });
}

bool starts_with(const std::string& str, const std::string& prefix) {
    if (prefix.length() > str.length()) return false;
    return str.substr(0, prefix.length()) == prefix;
}

bool ends_with(const std::string& str, const std::string& suffix) {
    if (suffix.length() > str.length()) return false;
    return str.substr(str.length() - suffix.length()) == suffix;
}

std::string replace_all(const std::string& str, const std::string& from, const std::string& to) {
    if (from.empty()) return str;

    std::string result = str;
    size_t pos = 0;

    while ((pos = result.find(from, pos)) != std::string::npos) {
        result.replace(pos, from.length(), to);
        pos += to.length();
    }

    return result;
}

std::string replace_first(const std::string& str, const std::string& from, const std::string& to) {
    if (from.empty()) return str;

    size_t pos = str.find(from);
    if (pos == std::string::npos) return str;

    std::string result = str;
    result.replace(pos, from.length(), to);
    return result;
}

std::string join(const std::vector<std::string>& strings, const std::string& delimiter) {
    if (strings.empty()) return "";

    std::string result = strings[0];
    for (size_t i = 1; i < strings.size(); ++i) {
        result += delimiter + strings[i];
    }

    return result;
}

std::vector<size_t> find_all(const std::string& str, const std::string& pattern) {
    std::vector<size_t> positions;
    size_t pos = str.find(pattern, 0);

    while (pos != std::string::npos) {
        positions.push_back(pos);
        pos = str.find(pattern, pos + 1);
    }

    return positions;
}

size_t count_occurrences(const std::string& str, const std::string& pattern) {
    if (pattern.empty()) return 0;

    size_t count = 0;
    size_t pos = 0;

    while ((pos = str.find(pattern, pos)) != std::string::npos) {
        ++count;
        pos += pattern.length();
    }

    return count;
}

} // namespace string_utils