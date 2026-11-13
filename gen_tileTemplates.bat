@echo off
echo ===================================================
echo  Building atTileTemplateGenerator (debug)
echo ===================================================
cd src
cmake --preset debug
cmake --build --preset debug --target atTileTemplateGenerator
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Build failed!
    cd ..
    exit /b %ERRORLEVEL%
)
cd ..

echo.
echo ===================================================
echo  Running TileTemplateGenerator
echo ===================================================
build\debug\Debug\atTileTemplateGenerator.exe
echo.
echo [Done] Check ./tileTemplates/
exit /b 0
