@echo off
echo Compiling Library Management System (Reorganized Structure)...

if not exist obj mkdir obj

g++ -std=c++14 -Wall -Wextra -g -Iinclude src/main.cpp src/Library.cpp src/Book.cpp src/Student.cpp src/FileStore.cpp -o library_system.exe

if %errorlevel% equ 0 (
    echo Compilation successful!
    echo To run the program, type: .\library_system.exe
) else (
    echo Compilation failed!
)

pause
