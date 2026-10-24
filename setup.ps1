<#
.SYNOPSIS
    Sets up the Dissidia 012 PC port from this repository: checks what must be installed, downloads
    and verifies the build dependencies and PortableKit, checks your game files, builds the port and
    makes a desktop shortcut.

.DESCRIPTION
    Run it from the repository folder:
        powershell -ExecutionPolicy Bypass -File .\setup.ps1
    (or double-click setup.cmd). It asks for your ISO (USA, ULUS10566) and your decrypted EBOOT the
    first time, and remembers them. Running it again redoes only what changed, so after `git pull` just
    run it again. Full guide and troubleshooting: docs/port/setup.md.

    It never installs system software, never needs administrator rights, never includes or downloads
    the key that decrypts the game, and never touches your saves (build\port\data\ms0).

.PARAMETER Update
    Re-apply the PortableKit patches and rebuild, asking nothing (uses the files given before).
.PARAMETER Deck
    Also build the Steam Deck package (build\port\deck-package\Dissidia012); see docs/port/steam-deck.md.
.PARAMETER NonInteractive
    Never ask: fail with a message instead. Give -Iso and -Eboot on the first run.
.PARAMETER Iso
    Path of your Dissidia 012 Duodecim Final Fantasy (USA, ULUS10566) ISO.
.PARAMETER Eboot
    Path of the decrypted EBOOT (pspdecrypt's EBOOT.ELF, or PPSSPP's dump).
.PARAMETER NoShortcuts
    Make no desktop shortcut or Start Menu entry.
.PARAMETER ShortcutDir
    Put the two shortcuts under this folder instead of the desktop and Start Menu (for tests).
.PARAMETER PortableKitSource
    Clone PortableKit from this local clone or URL instead of GitHub (it must contain commit 33800c4).
.PARAMETER DepsCache
    A folder holding the dependency zips (as in tools\local\port-deps\dl), used before downloading.
    Every file is still checked against its SHA-256.
.PARAMETER SkipIsoHash
    Accept an ISO whose SHA-1 differs from the known-good dump (its game files are still checked).

.EXAMPLE
    .\setup.ps1
.EXAMPLE
    .\setup.ps1 -NonInteractive -Iso "D:\Games\Dissidia 012 (USA).iso" -Eboot "D:\Games\EBOOT.ELF"
.EXAMPLE
    git pull; .\setup.ps1 -Update
#>
[CmdletBinding()]
param(
    [switch]$Update,
    [switch]$Deck,
    [switch]$NonInteractive,
    [string]$Iso,
    [string]$Eboot,
    [switch]$NoShortcuts,
    [string]$ShortcutDir,
    [string]$PortableKitSource,
    [string]$DepsCache,
    [switch]$SkipIsoHash
)
$ErrorActionPreference = "Stop"
$root = $PSScriptRoot
. (Join-Path $root "tools\port\setup\prereqs.ps1")

Write-Host "Dissidia 012 Duodecim Final Fantasy: PC port setup"
Write-Host "(You need your own copy of the game: the USA disc ULUS10566 as an ISO file.)"
$firstRun = -not (Test-Path (Join-Path $root "build\port\setup\state.json"))
$total = if ($Deck) { 7 } else { 6 }

Write-Host ""
Write-Host "[1/$total] Checking what is installed"
$check = Test-Prerequisites $root $firstRun
if ($check.Problems.Count -gt 0) {
    Show-Problems $check.Problems
    exit 1
}

# The port's Python environment: .venv with requirements-port.txt only.
$venvPython = Join-Path $root ".venv\Scripts\python.exe"
$requirements = Join-Path $root "requirements-port.txt"
if (-not (Test-Path $venvPython)) {
    Write-Host "    Making the Python environment .venv"
    & $check.Python -m venv (Join-Path $root ".venv")
    if ($LASTEXITCODE -ne 0) { Write-Host "Setup stopped: could not make .venv with $($check.Python)"; exit 1 }
}
$stamp = Join-Path $root ".venv\d012-requirements-port.sha256"
$wanted = (Get-FileHash $requirements -Algorithm SHA256).Hash
$have = if (Test-Path $stamp) { (Get-Content $stamp -Raw).Trim() } else { "" }
if ($have -ne $wanted) {
    Write-Host "    Installing the Python packages of requirements-port.txt into .venv"
    & $venvPython -m pip install --disable-pip-version-check --quiet -r $requirements
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Setup stopped: pip could not install requirements-port.txt (see the messages above)."
        exit 1
    }
    Set-Content -Path $stamp -Value $wanted -Encoding ascii
} else {
    Write-Host "    Python environment: .venv is ready"
}

$arguments = @((Join-Path $root "tools\port\setup\d012_setup.py"))
if ($Update) { $arguments += "--update" }
if ($Deck) { $arguments += "--deck" }
if ($NonInteractive) { $arguments += "--non-interactive" }
if ($Iso) { $arguments += @("--iso", $Iso) }
if ($Eboot) { $arguments += @("--eboot", $Eboot) }
if ($NoShortcuts) { $arguments += "--no-shortcuts" }
if ($ShortcutDir) { $arguments += @("--shortcut-dir", $ShortcutDir) }
if ($PortableKitSource) { $arguments += @("--portablekit-source", $PortableKitSource) }
if ($DepsCache) { $arguments += @("--deps-cache", $DepsCache) }
if ($SkipIsoHash) { $arguments += "--skip-iso-hash" }
$env:PYTHONUNBUFFERED = "1"  # progress shows as it happens, also when the output goes to a file
& $venvPython @arguments
exit $LASTEXITCODE
