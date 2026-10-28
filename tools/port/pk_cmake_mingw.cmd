@echo off
rem Runs cmake with llvm-mingw (clang) instead of MSVC, for the installer build
rem (port/README.md, "Installer build"). CMake and Ninja come from Visual
rem Studio; nothing else of it does: no vcvars, so no MSVC headers or libraries
rem are on the search paths. llvm-mingw 20260922 (ucrt, x86_64) is expected,
rem unpacked, in tools\local\llvm-mingw-20260922-ucrt-x86_64.
rem   tools\port\pk_cmake_mingw.cmd -S port -B <build> [options]
rem   tools\port\pk_cmake_mingw.cmd --build <build> -j8
setlocal EnableDelayedExpansion
set "ROOT=%~dp0..\.."
set "DEPS=%ROOT%\tools\local\port-deps"
set "LLVM=%ROOT%\tools\local\llvm-mingw-20260922-ucrt-x86_64"
if not exist "%LLVM%\bin\clang++.exe" (
    echo pk_cmake_mingw: llvm-mingw not found in %LLVM%
    exit /b 1
)
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
for /f "usebackq delims=" %%i in (`"!VSWHERE!" -latest -products * -property installationPath`) do set "VS=%%i"
if not defined VS (
    echo pk_cmake_mingw: Visual Studio's CMake and Ninja not found
    exit /b 1
)
set "CMBIN=%VS%\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin"
set "NINJA=%VS%\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja"
set "INCLUDE="
set "LIB="
set "LIBPATH="
set "PATH=%LLVM%\bin;%CMBIN%;%NINJA%;%DEPS%\glslang\bin;%DEPS%\sdl3\SDL3-3.4.18\lib\x64;%PATH%"
if "%~1"=="--build" (
    cmake %*
) else (
    cmake %* -G Ninja ^
        -DCMAKE_BUILD_TYPE=Release ^
        -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_RC_COMPILER=llvm-windres ^
        "-DSDL3_DIR=%DEPS%\sdl3\SDL3-3.4.18\cmake" ^
        "-DVulkan_INCLUDE_DIR=%DEPS%\vkheaders\Vulkan-Headers-vulkan-sdk-1.4.363.0\include" ^
        "-DVulkan_LIBRARY=%DEPS%\vulkan-lib\vulkan-1.lib" ^
        "-DPORTABLEKIT_GLSLANG=%DEPS%\glslang\bin\glslang.exe" ^
        "-DPORTABLEKIT_FFMPEG_DOWNLOAD_DIR=%DEPS%\dl\ffmpeg"
)
