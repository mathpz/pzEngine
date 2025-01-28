@echo off
setlocal

rem Set the path to the glslc compiler (same directory as the batch file)
set GLSLC="%~dp0glslc.exe"

rem Set the directory containing the shader files (same directory as the batch file)
set SHADER_DIR=%~dp0

rem Delete previous .spv files
del /q %SHADER_DIR%\*.spv

rem Compile each shader file in the shader directory
for %%f in (%SHADER_DIR%\*.frag %SHADER_DIR%\*.vert) do (
    echo Compiling %%f...
    %GLSLC% %%f -o %%f.spv
    if errorlevel 1 (
        echo Failed to compile %%f
        pause
        exit /b 1
    )
)

echo All shaders compiled successfully.
pause
endlocal