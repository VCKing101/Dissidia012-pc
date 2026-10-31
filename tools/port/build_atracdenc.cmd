@echo off
rem Builds atracdenc, the ATRAC3 encoder the Battle BGM Maker's "Make a PSP /
rem PPSSPP ISO" runs (port/bgm_maker/psp_export.cpp), from its source at a fixed
rem commit: github.com/dcherednik/atracdenc, LGPL-2.1 (its libgha submodule:
rem BSD-2). Built with MSVC, Media Foundation for reading WAV, the C runtime
rem linked in, so the program needs only Windows' own DLLs. It stays a program
rem of its own, run by the tool; installers ship it beside BgmMaker.exe with its
rem licences.
rem   tools\port\build_atracdenc.cmd
rem Source: build\external\atracdenc-src\atracdenc (cloned the first time).
rem Program: tools\local\port\build-atracdenc\src\atracdenc.exe
setlocal
set "ROOT=%~dp0..\.."
set "SRC=%ROOT%\build\external\atracdenc-src\atracdenc"
set "BUILD=%ROOT%\tools\local\port\build-atracdenc"
set "COMMIT=cb170709f836cb43dc0312455188c791159d81dc"
if not exist "%SRC%\.git" (
    git clone https://github.com/dcherednik/atracdenc.git "%SRC%" || exit /b 1
)
git -C "%SRC%" cat-file -e %COMMIT% 2>nul || git -C "%SRC%" fetch origin %COMMIT% || exit /b 1
git -C "%SRC%" checkout --quiet %COMMIT% || exit /b 1
git -C "%SRC%" submodule update --init --recursive || exit /b 1
call "%~dp0pk_cmake.cmd" -S "%SRC%" -B "%BUILD%" -G Ninja -DATRACDENC_PCM_IO_BACKEND=mediafoundation ^
    -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded || exit /b 1
call "%~dp0pk_cmake.cmd" --build "%BUILD%" --target atracdenc || exit /b 1
echo built %BUILD%\src\atracdenc.exe (atracdenc %COMMIT%)
