@echo off
rem Runs a command inside the Visual Studio x64 build environment, found the same
rem way as tools\port\pk_cmake.cmd finds it. Used by setup.ps1.
rem   vsenv.cmd <command> [args]   e.g. vsenv.cmd dumpbin /exports file.dll
rem   vsenv.cmd --probe            prints vs=, cl=, cmake=, ninja=, dumpbin=, lib=, sdk= lines
setlocal EnableDelayedExpansion
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
set "VS="
if exist "!VSWHERE!" (
    for /f "usebackq delims=" %%i in (`"!VSWHERE!" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VS=%%i"
)
if not defined VS (
    echo vsenv: Visual Studio with the C++ tools not found
    exit /b 1
)
call "%VS%\VC\Auxiliary\Build\vcvars64.bat" >nul 2>nul || exit /b 1
setlocal DisableDelayedExpansion
if "%~1"=="--probe" goto probe
%*
exit /b %ERRORLEVEL%

:probe
echo vs=%VS%
call :which cl.exe
call :which cmake.exe
call :which ninja.exe
call :which dumpbin.exe
call :which lib.exe
echo sdk=%WindowsSdkDir%
exit /b 0

:which
set "FOUND="
for /f "delims=" %%p in ('where %1 2^>nul') do if not defined FOUND set "FOUND=%%p"
echo %~n1=%FOUND%
exit /b 0
