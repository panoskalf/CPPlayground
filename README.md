# CPPlayground

![C++](https://img.shields.io/badge/C++-17/20-blue.svg) ![CMake](https://img.shields.io/badge/CMake-3.20+-green.svg) ![GoogleTest](https://img.shields.io/badge/Testing-GoogleTest-red.svg)

This project is my Modern C++ playground!

It contains some experiments, examples, and tests. You are free to have a look! In fact you are invited :)

## 🎯 **Highlights**

- **SimpleSocket Library**: Modern C++ RAII wrapper for cross-platform networking
- **Template Programming**: Fixed-point arithmetic with C++20 concepts
- **Memory Management**: Smart pointer patterns and custom RAII classes
- **Testing**: Comprehensive unit tests with GoogleTest

## Motivation

I am sharing this fun little project of mine for educational purpose and hopefully to receive some constructive critisism, which would be **greatly appreciated**.

## Structure

- Each folder has a CMakeLists.txt with different targets and may have a README.md
- Unit tests use Google Test (GTest).

## Building and Testing

### Prerequisites
- CMake (version 3.20 or higher)
- A C++ compiler (e.g., MSVC, GCC, Clang)
- C++ (version 17 or higher)

### Build All Examples
Use the provided cross-platform scripts:

```bash
cmake -P build.cmake    # Build project (MinGW on Windows, Unix Makefiles on Linux)
cmake -P clean.cmake    # Clean build files
cmake -P test.cmake     # Run all tests
```
or let cmake detect your systems default generator:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug		# Configure step
cmake --build build		                        	# Build step
```

### Run an Example
After building, executables are in `build/<example_folder>/`:
```
build/string_utilities/string_demo.exe
```

### Run Tests
```bash
cmake -P test.cmake     # All tests
# Or run individual test executables:
build/<example_folder>/<test_name>.exe
```
