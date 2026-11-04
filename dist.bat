@echo off

set EXE_NAME=AnimeTower.exe
set DIST_DIR=distribution\AnimeTower
set EXE_BUILDED_DIR=build\Release
set RESOURCE_DIR=resource

REM Build Release first (without console for distribution)
echo Building Release for distribution (no console)...
call "%~dp0build.bat" -dist
if %ERRORLEVEL% neq 0 (
    echo ERROR: Build failed
    exit /b 1
)
echo.

if not exist %DIST_DIR% mkdir %DIST_DIR%

copy /Y %EXE_BUILDED_DIR%\%EXE_NAME% %DIST_DIR%\%EXE_NAME% >nul
echo Copied %EXE_NAME%

REM Copy resource folder (with replacement)
if exist %RESOURCE_DIR% (
    echo Copying resources...
    xcopy /E /I /Y /Q %RESOURCE_DIR% %DIST_DIR%\resource >nul
    echo Resources copied
)

REM Create ZIP archive with date and time
echo Creating ZIP archive...
for /f "tokens=*" %%a in ('powershell -Command "Get-Date -Format 'yyyy-MM-dd_HH-mm'"') do set DT=%%a
set ZIP_NAME=AnimeTower_%DT%.zip
powershell -Command "Compress-Archive -Path '%DIST_DIR%\*' -DestinationPath 'distribution\%ZIP_NAME%' -Force"
echo Archive created: distribution\%ZIP_NAME%

echo.
echo Distribution complete!
exit /b 0
