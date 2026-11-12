@echo off
echo ===================================================
echo  Building AnimeTower_TileTemplateGenerator (debug)
echo ===================================================
cd src
cmake --preset debug
cmake --build --preset debug --target AnimeTower_TileTemplateGenerator
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
build\debug\Debug\AnimeTower_TileTemplateGenerator.exe
echo.
echo [Done] Check ./tileTemplates/
exit /b 0
