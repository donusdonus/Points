@echo off
REM Generate Doxygen documentation for Points project

echo Generating Doxygen documentation...

REM Check if doxygen is available
where doxygen >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Error: Doxygen not found in PATH
    echo Please install Doxygen from http://www.doxygen.nl/
    pause
    exit /b 1
)

REM Create docs directory if it doesn't exist
if not exist "docs" mkdir docs

REM Generate documentation
doxygen Doxyfile

if %ERRORLEVEL% EQU 0 (
    echo Documentation generated successfully!
    echo Open docs\html\index.html to view the documentation
    
    REM Ask if user wants to open the documentation
    set /p OPEN="Do you want to open the documentation now? (y/n): "
    if /i "%OPEN%"=="y" (
        start docs\html\index.html
    )
) else (
    echo Error generating documentation
    pause
    exit /b 1
)

pause
