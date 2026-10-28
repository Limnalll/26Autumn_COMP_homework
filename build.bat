```bat
@echo off
chcp 65001 >nul
setlocal

REM 切换到脚本所在的 project 根目录
cd /d "%~dp0"

REM 编译器路径
set "CXX=D:\Software\mingw64\bin\g++.exe"

REM 输出文件
set "OUTPUT=main.exe"

echo ==============================
echo       Building project
echo ==============================

"%CXX%" -std=c++17 -Wall -Wextra -g ^
    src\main.cpp ^
    src\json_io.cpp ^
    src\cli.cpp ^
    src\stimulator.cpp ^
    -o "%OUTPUT%"

if errorlevel 1 (
    echo.
    echo [ERROR] Compilation failed!
    pause
    exit /b 1
)

echo.
echo [SUCCESS] Generated main.exe in project root.
pause
exit /b 0
```