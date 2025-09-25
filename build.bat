@echo off
REM Build with MSVC (default generator)
cmake -S . -B build
cmake --build build
