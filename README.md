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
On Windows you can use the provided batch files for convenience:

- **MSVC (default):**
	Double-click or run `build.bat`
- **MinGW:**
	Double-click or run `build_mingw.bat`

Or manually:
```
cmake -S . -B build
cmake --build build
```

### Run an Example
After building, executables are located `build/<example_folder>/<target_name>`. For example:
```
build/string_utilities/string_demo.exe
```

### Run Tests
To run all tests (from the build directory):
```
cd build
ctest
```
Or run a specific test executable directly:
```
build/<example_folder>/<test_target_name>.exe
```
