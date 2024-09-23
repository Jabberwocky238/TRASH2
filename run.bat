@echo off
setlocal enabledelayedexpansion

:: 检查是否有参数传递
if "%~1"=="" (
    echo No file name provided.
    echo Usage: %0 filename.cpp
    exit /b
)

:: 设置文件名变量
set FILENAME=%~1
set EXECUTABLE=!FILENAME:.cpp=!

:: 编译
echo Compiling !FILENAME!...
g++ -o !EXECUTABLE! !FILENAME!
if errorlevel 1 goto :error

:: 运行
echo Compilation successful.
echo Running !EXECUTABLE!...
echo ####################
!EXECUTABLE!
echo ####################
if errorlevel 1 goto :error

echo Execution finished successfully.
goto :eof

:error
echo An error occurred during compilation or execution.
endlocal
