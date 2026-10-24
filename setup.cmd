@echo off
rem Double-click this file (or run it in a terminal) to set up the port: it runs setup.ps1
rem without changing PowerShell's script policy. Arguments are passed on, e.g. setup.cmd -Update
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0setup.ps1" %*
set "CODE=%ERRORLEVEL%"
if "%~1"=="" pause
exit /b %CODE%
