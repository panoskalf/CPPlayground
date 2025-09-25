# String Manipulation & Utilities

This module demonstrates and tests various C++ string manipulation functions and utilities based on cppreference documentation.

## Testing Objectives

### Core String Operations
- **Constructors & Assignment**: Test different ways to create and assign strings
- **Element Access**: Test safe and unsafe access methods (at(), operator[], front(), back())
- **Capacity Management**: Test size(), length(), capacity(), empty(), reserve(), shrink_to_fit()
- **Modifiers**: Test insert(), erase(), push_back(), pop_back(), append(), replace()

### String Algorithms
- **Search Operations**: Test find(), rfind(), find_first_of(), find_last_of(), find_first_not_of(), find_last_not_of()
- **Comparison**: Test compare(), lexicographical comparisons with operators
- **Substring**: Test substr() with various parameters and edge cases

### String Utilities & Transformations
- **Case Conversion**: Implement and test to_upper(), to_lower() functions
- **Trimming**: Test trim_left(), trim_right(), trim() for whitespace removal
- **Splitting**: Test string tokenization and splitting by delimiters
- **Validation**: Test functions for checking numeric strings, email format, etc.

### Performance & Edge Cases
- **Memory Efficiency**: Test string operations for memory usage patterns
- **Unicode/Multibyte**: Test basic UTF-8 string handling
- **Error Conditions**: Test boundary conditions, empty strings, invalid indices
- **Performance**: Compare string vs string_view performance where applicable

## Structure
- `string_utilities.h` - Interface declarations
- `string_utilities.cpp` - Implementation of utility functions
- `string_demo.cpp` - Demonstration program showing usage
- `test_string_utilities.cpp` - Comprehensive test suite using GTest

## Usage
Run the demo to see string operations in action, then run tests to verify correctness.