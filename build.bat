@echo off
setlocal

REM Parse arguments
set BUILD_CONFIG=Release

:ARG_LOOP
if "%1"=="" goto :BUILD
if /i "%1"=="-d" (
    set BUILD_CONFIG=Debug
    shift
    goto :ARG_LOOP
)
if /i "%1"=="-r" (
    set BUILD_CONFIG=Release
    shift
    goto :ARG_LOOP
)
shift
goto :ARG_LOOP

:BUILD
REM Find Visual Studio
for /f "tokens=*" %%i in ('"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath 2^>nul') do set VS_PATH=%%i

if not defined VS_PATH (
    echo ERROR: Visual Studio not found
    exit /b 1
)

REM Activate MSVC environment
set VC_INIT="%VS_PATH%\VC\Auxiliary\Build\vcvarsall.bat"
call %VC_INIT% x64 >nul 2>&1

REM Create build directory
if not exist build mkdir build
cd build

REM Configure CMake (use VS 18 2026 which is default)
echo Configuring %BUILD_CONFIG% build...
cmake ../src -G "Visual Studio 18 2026" -A x64

if %ERRORLEVEL% neq 0 (
    echo ERROR: CMake configuration failed
    exit /b 1
)

REM Build
echo Building %BUILD_CONFIG%...
cmake --build . --config %BUILD_CONFIG%

if %ERRORLEVEL% neq 0 (
    echo ERROR: Build failed
    exit /b 1
)

cd ..
echo.
echo Build complete! %BUILD_CONFIG%
