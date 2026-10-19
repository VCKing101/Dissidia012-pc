param(
    [ValidateSet('retail', 'original', 'rebuilt')][string]$Case = 'rebuilt',
    [ValidatePattern('^[a-zA-Z0-9][a-zA-Z0-9_-]{0,63}$')][string]$RunId = 'human-combat-v1',
    [ValidateRange(60, 3600)][int]$HoldSeconds = 3600,
    [switch]$PrepareOnly
)
$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest
$principal = [Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()
if ($principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) { throw 'Run the human test as the normal user.' }
$projectRoot = Split-Path $PSScriptRoot -Parent
$sessionRoot = Join-Path $projectRoot "build/ULUS10566/runtime/sessions/$Case/$RunId"
$sessionPath = Join-Path $sessionRoot 'session.json'
if (-not (Test-Path -LiteralPath $sessionPath)) {
    & (Join-Path $PSScriptRoot 'start_runtime.ps1') -Case $Case -RunId $RunId -PrepareOnly | Out-Null
    $baseline = Join-Path $projectRoot "build/ULUS10566/runtime/sessions/$Case/debugger-baseline-v1/memstick/PSP/SAVEDATA"
    $destination = Join-Path $sessionRoot 'memstick/PSP/SAVEDATA'
    # Only the disposable profile produced by our own prior test is seeded.
    if (Test-Path -LiteralPath $baseline) { Copy-Item -LiteralPath $baseline -Destination $destination -Recurse }
    $session = Get-Content -LiteralPath $sessionPath -Raw | ConvertFrom-Json
    $configuration = [IO.File]::ReadAllText($session.config).Replace('Enable = False', 'Enable = True')
    [IO.File]::WriteAllText($session.config, $configuration, (New-Object System.Text.UTF8Encoding($false)))
    # In 1.20.4 a missing ControlMapping section retains standard keyboard and
    # gamepad defaults; an empty section deliberately clears those mappings.
    [IO.File]::WriteAllText($session.control_config, "; Use PPSSPP 1.20.4 standard input mappings.`n", (New-Object System.Text.UTF8Encoding($false)))
}
$session = Get-Content -LiteralPath $sessionPath -Raw | ConvertFrom-Json
if ($session.started -and -not $session.stopped) { throw 'This human test session is already running.' }
$expectedExe = [IO.Path]::GetFullPath((Join-Path $sessionRoot 'emulator/PPSSPPWindows64.exe'))
if ([IO.Path]::GetFullPath($session.exe) -ne $expectedExe) { throw 'Unexpected test executable path.' }
if ((Get-FileHash -LiteralPath $session.exe -Algorithm SHA256).Hash.ToLowerInvariant() -ne $session.exe_sha256) { throw 'Test executable changed.' }
if ((Get-FileHash -LiteralPath $session.iso -Algorithm SHA256).Hash.ToLowerInvariant() -ne $session.iso_sha256) { throw 'Rebuilt test ISO changed.' }
# The retail case runs the untouched ISO, so package proofs do not apply to it.
if ($session.case -ne 'retail') {
    $python = Join-Path $projectRoot '.venv/Scripts/python.exe'
    & $python -c 'import json,sys; sys.path.insert(0,sys.argv[1]); from prepare_runtime_packages import validate_builds; assert validate_builds() == json.load(open(sys.argv[2]))["build_inputs"]' $PSScriptRoot (Join-Path $projectRoot 'build/ULUS10566/runtime/packages/manifest.json')
    if ($LASTEXITCODE -ne 0) { throw 'Runtime package build proofs are stale.' }
}
$configuration = [IO.File]::ReadAllText($session.config)
if ($configuration -match '(?im)^(RemoteDebuggerOnStartup|RemoteShareOnStartup|DumpFrames|EnableWlan|EnableAdhocServer|CheckForNewVersion)\s*=\s*(True|1)\s*$') { throw 'Human tests require remote services, WLAN, updater checks and continuous recording disabled.' }
$session.config_sha256 = (Get-FileHash -LiteralPath $session.config -Algorithm SHA256).Hash.ToLowerInvariant()
$session | Add-Member -NotePropertyName human_test -NotePropertyValue $true -Force
$session.debugger_enabled = $false
$session.capture_video = $false
if ($PrepareOnly) {
    [IO.File]::WriteAllText($sessionPath, ($session | ConvertTo-Json -Depth 8), (New-Object System.Text.UTF8Encoding($false)))
    [pscustomobject]@{ready=$true;session_root=$sessionRoot;started=$false;remote_services=$false;continuous_recording=$false} | ConvertTo-Json
    return
}
# Visible only for this explicitly selected human interaction workflow.
$process = Start-Process -FilePath $session.exe -ArgumentList $session.arguments -WorkingDirectory (Split-Path $session.exe -Parent) -WindowStyle Normal -PassThru
$session.pid = $process.Id
$session.process_start_utc = $process.StartTime.ToUniversalTime().ToString('o')
$session.started = $true
$session.stopped = $false
[IO.File]::WriteAllText($sessionPath, ($session | ConvertTo-Json -Depth 8), (New-Object System.Text.UTF8Encoding($false)))
[pscustomobject]@{pid=$process.Id;session_root=$sessionRoot;visible=$true;debugger=$false;continuous_recording=$false} | ConvertTo-Json
$deadline = [DateTime]::UtcNow.AddSeconds($HoldSeconds)
try {
    while ([DateTime]::UtcNow -lt $deadline) {
        $process.Refresh()
        if ($process.HasExited) { break }
        Start-Sleep -Seconds 1
    }
} finally {
    & (Join-Path $PSScriptRoot 'stop_runtime.ps1') -SessionRoot $sessionRoot | Out-Null
}
