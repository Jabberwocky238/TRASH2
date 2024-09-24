@echo off
if "%~1"=="" (
    echo No command provided.
    echo Usage: "setup [command=build|run|clean]"
    exit /b
)
goto :eof

REM 定义命令
echo "[Command]: %~1"
if "%~1"=="build" (
    echo Building the project...
    call :build
) else if "%~1"=="run" (
    echo Running the project...
    call :build
    call :run
) else if "%~1"=="clean" (
    echo Clean the project...
    call :clean
) else (
    echo Invalid command: %~1
    echo Usage: "setup [command=build|run|clean]"
    exit /b
)
goto :eof


:clean
REM 切换到脚本所在目录
cd /d "%~dp0"
REM 删除构建目录
del /f /s /q "build\*.*"
for /d %%x in ("build\*.*") do @rd /s /q "%%x"
del /f /s /q "bin\*.*"
for /d %%x in ("bin\*.*") do @rd /s /q "%%x"
echo Clean completed successfully.
goto :eof

:run
REM 切换到脚本所在目录
cd /d "%~dp0"
REM 运行项目
bin\Debug\demo.exe %~2 %~3 %~4 %~5 %~6 %~7 %~8 %~9
goto :eof

:build
REM 切换到脚本所在目录
cd /d "%~dp0"
REM 创建构建目录
if not exist build mkdir build
cd build
REM 运行cmake来生成Makefile
cmake ..
REM 检查cmake是否成功
if %ERRORLEVEL% NEQ 0 (
    echo CMake failed to generate the Makefile.
    exit /b %ERRORLEVEL%
)
REM 运行make来构建项目
cmake --build . --config Debug --target ALL_BUILD
REM 检查make是否成功
if %ERRORLEVEL% NEQ 0 (
    echo MSBuild failed to build the project.
    exit /b %ERRORLEVEL%
)
echo Build completed successfully.
cd ..
goto :eof


:eof
