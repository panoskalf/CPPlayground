# Playground

This folder contains C++ experiments, examples, and tests. Each subfolder is an independent target (executable or test suite) with its own CMakeLists.txt and documentation.

## Structure
- Each topic/example is in its own folder.
- Each folder has a README.md and CMakeLists.txt.
- Unit tests use Google Test (GTest).

## Getting Started

## Building and Testing

### Prerequisites
- CMake (version 3.14 or higher)
- A C++ compiler (e.g., MSVC, GCC, Clang)


### Build All Examples
You can use the provided batch files for convenience:

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
After building, executables are in the `build/<example_folder>/` directory. For example:
```
build/sample/sample_main.exe
```


### Run Tests
To run all tests (from the build directory):
```
cd build
ctest
```
Or run a specific test executable directly:
```
build/sample/sample_test.exe
```
You can also use a batch file (add this if you create one):
```
test.bat
```
### Clean Build Files
To clean all build files and start fresh, use the provided batch file:
```
clean.bat
```
Or manually:
```
rmdir /s /q build
```


### Adding New Examples
- Create a new folder (e.g., `my_example`) with its own `CMakeLists.txt`, source files, and README.md.
- Add `add_subdirectory(my_example)` to the main `CMakeLists.txt`.
- If your example needs GoogleTest, just link to `gtest` or `gtest_main` (already fetched at the top level).

## Best Practices
- Keep code modular and well-documented.
- Add tests for new features or experiments.
- Use clear folder and file names.
