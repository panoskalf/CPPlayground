@echo off
REM Build with MinGW
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
