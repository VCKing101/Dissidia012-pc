# Starts the port in a window for the owner to play, with no time limit.
#   .\tools\port\play_d012.ps1                 # recompiled build, saves in build/port/data
#   .\tools\port\play_d012.ps1 -Interpreter    # interpreter build instead
# Saves stay in <data dir>\ms0. Close the window to quit.
param(
    [switch]$Interpreter,
    [string]$DataDir = "build/port/data"
)
$ErrorActionPreference = "Stop"
$root = Resolve-Path (Join-Path $PSScriptRoot "..\..")
$build = if ($Interpreter) { "tools/local/port/build-d012" } else { "tools/local/port/build-d012-recomp" }
$exe = Join-Path $root "$build/bin/D012Native.exe"
if (-not (Test-Path $exe)) {
    # The desktop shortcut runs this hidden, so say it in a window too.
    $message = "The port is not built yet ($exe).`nRun setup.ps1 (or setup.cmd) in the repository folder first."
    try {
        Add-Type -AssemblyName System.Windows.Forms
        [System.Windows.Forms.MessageBox]::Show($message, "Dissidia 012 (PC port)") | Out-Null
    } catch { }
    throw "Not built: $exe"
}
$data = Join-Path $root $DataDir
New-Item -ItemType Directory -Force $data | Out-Null
$env:D012_GAME_DIR = Join-Path $root "build/port/game"
$env:D012_DATA_DIR = $data
$env:D012_MEMORY_STICK_DIR = Join-Path $data "ms0"
if (-not $Interpreter) { $env:PSPRECOMP_NO_INTERPRETER = "1" }
Start-Process -FilePath $exe -WorkingDirectory (Split-Path $exe)
