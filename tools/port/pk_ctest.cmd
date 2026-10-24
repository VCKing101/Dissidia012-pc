@echo off
rem Runs PortableKit's framework tests in a build directory, without the ad hoc
rem networking tests: their server listens on every interface, so each new build
rem of them raises a Windows Firewall prompt that nobody may be there to answer.
rem   tools\port\pk_ctest.cmd [build dir]          (default tools\local\port\build-framework)
rem   set PK_CTEST_ADHOC=1 to include them (only when someone is at the PC).
setlocal
set "ROOT=%~dp0..\.."
set "BUILD=%~1"
if "%BUILD%"=="" set "BUILD=%ROOT%\tools\local\port\build-framework"
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VS=%%i"
set "CTEST=%VS%\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\ctest.exe"
set "PATH=%ROOT%\tools\local\port-deps\sdl3\SDL3-3.4.18\lib\x64;%PATH%"
if "%PK_CTEST_ADHOC%"=="1" (
    "%CTEST%" --test-dir "%BUILD%" --timeout 300
) else (
    "%CTEST%" --test-dir "%BUILD%" --timeout 300 -E adhoc
)
