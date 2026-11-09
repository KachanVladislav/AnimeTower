@echo off
setlocal enabledelayedexpansion

REM ============================================================================
REM Array of {preset, target} pairs
REM ============================================================================
set CONFIGS=debug:AnimeTower_Tests release:AnimeTower_Tests debug:AnimeTower release:AnimeTower dist:AnimeTower

REM ============================================================================
REM Counter for progress display
REM ============================================================================
set TOTAL=5
set CURRENT=0

echo ===================================================
echo  Running all configurations (%TOTAL% total)
echo ===================================================
echo.

REM ============================================================================
REM Loop through each configuration
REM ============================================================================
for %%C in (%CONFIGS%) do (
    set /a CURRENT+=1
    
    REM Parse preset:target
    for /f "tokens=1,2 delims=:" %%A in ("%%C") do (
        set PRESET=%%A
        set TARGET=%%B
        
        REM Determine build config folder (Debug for debug preset, Release for others)
        if "%%A"=="debug" (set BUILD_CONFIG=Debug) else (set BUILD_CONFIG=Release)
        
        echo ===================================================
        echo  [!CURRENT!/%TOTAL%] Preset: %%A ^| Target: %%B
        echo ===================================================
        echo.
        
        REM -------------------------------------------------------------------
        REM Step 1: Configure
        REM -------------------------------------------------------------------
        echo [1/3] Configuring ^(Preset: %%A^)...
        cd src
        cmake --preset %%A
        if !ERRORLEVEL! NEQ 0 (
            echo [ERROR] Configuration failed for preset %%A!
            cd ..
            exit /b !ERRORLEVEL!
        )
        
        REM -------------------------------------------------------------------
        REM Step 2: Build
        REM -------------------------------------------------------------------
        echo.
        echo [2/3] Building target: %%B...
        cmake --build --preset %%A --target %%B
        if !ERRORLEVEL! NEQ 0 (
            echo [ERROR] Build failed for target %%B!
            cd ..
            exit /b !ERRORLEVEL!
        )
        cd ..
        
        REM -------------------------------------------------------------------
        REM Step 3: Run and wait for exit
        REM -------------------------------------------------------------------
        set EXE_PATH=build\%%A\!BUILD_CONFIG!\%%B.exe
        echo.
        echo [3/3] Running: !EXE_PATH!
        echo ---------------------------------------------------
        
        if not exist !EXE_PATH! (
            echo [ERROR] Executable not found: !EXE_PATH!
            exit /b 1
        )
        
        REM Launch exe and wait for it to close
        start "" /wait !EXE_PATH!
        
        echo ---------------------------------------------------
        echo [Done] %%B closed.
        echo.
    )
)

echo ===================================================
echo  [Complete] All %TOTAL% configurations processed.
echo ===================================================
echo.
exit /b 0
