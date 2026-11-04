@echo off
setlocal

set EXE_NAME=raylibStart.exe
set DIST_DIR=distribution
set EXE_BUILDED_DIR=build\Release

REM Build Release first
echo Building Release...
call "%~dp0build.bat" -r
if %ERRORLEVEL% neq 0 (
    echo ERROR: Build failed
    exit /b 1
)
echo.

if not exist %DIST_DIR% mkdir %DIST_DIR%

copy /Y %EXE_BUILDED_DIR%\%EXE_NAME% %DIST_DIR%\%EXE_NAME% >nul
echo Distribution complete!
