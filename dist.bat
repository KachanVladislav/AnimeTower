@echo off
setlocal enabledelayedexpansion

set PRESET=dist
set TARGET=AnimeTower

echo ===================================================
echo [1/5] Configuring project (Preset: %PRESET%)...
echo ===================================================
cd src
cmake --preset %PRESET%
if %ERRORLEVEL% NEQ 0 (
    echo "[ERROR] Configuration failed!"
    exit /b %ERRORLEVEL%
)

echo.
echo ===================================================
echo [2/5] Building target: %TARGET%...
echo ===================================================
cmake --build --preset %PRESET% --target %TARGET%
if %ERRORLEVEL% NEQ 0 (
    echo "[ERROR] Build failed!"
    exit /b %ERRORLEVEL%
)
cd ..

set EXE_NAME=AnimeTower.exe
set EXE_BUILDED_DIR=build\dist\Release
set STAGE_DIR=distribution\AnimeTower
echo.
echo ===================================================
echo [3/5] Copy: %EXE_BUILDED_DIR%\%EXE_NAME% to %STAGE_DIR%
echo ===================================================
if not exist %STAGE_DIR% mkdir %STAGE_DIR%
copy /Y %EXE_BUILDED_DIR%\%EXE_NAME% %STAGE_DIR%\%EXE_NAME% 
if %ERRORLEVEL% NEQ 0 (
    echo "[ERROR] copy %EXE_BUILDED_DIR%\%EXE_NAME% to %STAGE_DIR%"
    exit /b %ERRORLEVEL%
)

set RESOURCE_DIR=resource
echo.
echo ===================================================
echo [4/5] Copy resources: %RESOURCE_DIR% to %STAGE_DIR%
echo ===================================================
REM Copy resource folder (with replacement)
if not exist %RESOURCE_DIR% (
    echo "[ERROR] resource at %RESOURCE_DIR% not exist!"
    exit /b %ERRORLEVEL%
)
xcopy /E /I /Y /Q %RESOURCE_DIR% %STAGE_DIR%\resource
if %ERRORLEVEL% NEQ 0 (
    echo "[ERROR] copy resources: %RESOURCE_DIR% to %STAGE_DIR%"
    exit /b %ERRORLEVEL%
)

for /f "tokens=*" %%a in ('powershell -Command "Get-Date -Format 'yyyy-MM-dd_HH-mm'"') do set DT=%%a
set ZIP_NAME=AnimeTower_%DT%.zip
set DIST_SIR=distribution
echo.
echo ===================================================
echo [5/5] Creating ZIP archive: %ZIP_NAME%
echo ===================================================
powershell -Command "Compress-Archive -Path '%STAGE_DIR%' -DestinationPath '%DIST_SIR%\%ZIP_NAME%' -Force"
echo Archive created: %DIST_SIR%\%ZIP_NAME%

echo.
echo ===================================================
echo [Complete] Creating ZIP archive: %ZIP_NAME%
echo ===================================================
echo.
exit /b 0
