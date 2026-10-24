@echo off
rem Runs cmake inside the Visual Studio 2022 x64 environment, with the port's
rem local dependencies (tools\local\port-deps) on the search paths.
rem   tools\port\pk_cmake.cmd -S <source> -B <build> -G Ninja [options]
rem   tools\port\pk_cmake.cmd --build <build> -j2
setlocal EnableDelayedExpansion
set "ROOT=%~dp0..\.."
set "DEPS=%ROOT%\tools\local\port-deps"
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
for /f "usebackq delims=" %%i in (`"!VSWHERE!" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VS=%%i"
if not defined VS (
    echo pk_cmake: Visual Studio not found
    exit /b 1
)
set "PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer;%PATH%"
call "%VS%\VC\Auxiliary\Build\vcvars64.bat" >nul || exit /b 1
set "PATH=%DEPS%\glslang\bin;%DEPS%\sdl3\SDL3-3.4.18\lib\x64;%PATH%"
if "%~1"=="--build" (
    cmake %*
) else (
    cmake %* ^
        -DCMAKE_BUILD_TYPE=Release ^
        "-DSDL3_DIR=%DEPS%\sdl3\SDL3-3.4.18\cmake" ^
        "-DVulkan_INCLUDE_DIR=%DEPS%\vkheaders\Vulkan-Headers-vulkan-sdk-1.4.363.0\include" ^
        "-DVulkan_LIBRARY=%DEPS%\vulkan-lib\vulkan-1.lib" ^
        "-DPORTABLEKIT_GLSLANG=%DEPS%\glslang\bin\glslang.exe" ^
        "-DPORTABLEKIT_FFMPEG_DOWNLOAD_DIR=%DEPS%\dl\ffmpeg"
)
