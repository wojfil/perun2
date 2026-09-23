@echo off
setlocal

set SCRIPT_DIR=%~dp0
set ROOT_DIR=%SCRIPT_DIR%\..

set BUILD_DIR=%ROOT_DIR%\build

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

if defined VCPKG_ROOT (
    echo Using vcpkg from %VCPKG_ROOT%
    set TOOLCHAIN=-DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake"
) else (
    set TOOLCHAIN=
    echo No VCPKG_ROOT defined. Using default CMake search paths.
)

cmake -S "%ROOT_DIR%" -B "%BUILD_DIR%" ^
  -G "Visual Studio 17 2022" -A x64 ^
  %TOOLCHAIN%

if ERRORLEVEL 1 (
    echo CMake configuration failed. Please ensure ICU is installed or VCPKG_ROOT is set.
    exit /b 1
)

cmake --build "%BUILD_DIR%" --config Release

endlocal