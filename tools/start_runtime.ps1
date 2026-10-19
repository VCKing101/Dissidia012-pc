param(
    [Parameter(Mandatory = $true)][ValidateSet('retail', 'original', 'rebuilt')][string]$Case,
    [ValidatePattern('^[a-zA-Z0-9][a-zA-Z0-9_-]{0,63}$')][string]$RunId,
    [ValidateSet('directx11', 'gles', 'software', 'vulkan')][string]$Graphics = 'directx11',
    [ValidateRange(0, 65535)][int]$Port = 0,
    [switch]$CaptureVideo,
    [switch]$PrepareOnly
)
$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest
$projectRoot = Split-Path $PSScriptRoot -Parent
$runtimeRoot = Join-Path $projectRoot 'build/ULUS10566/runtime'
$toolRoot = Join-Path $projectRoot 'tools/local/ppsspp'
$sourceExe = Join-Path $toolRoot 'PPSSPPWindows64.exe'
$templatePath = Join-Path $projectRoot 'config/runtime-ppsspp.ini'
$manifestPath = Join-Path $runtimeRoot 'packages/manifest.json'
$utf8NoBom = New-Object System.Text.UTF8Encoding($false)

function Assert-Hash([string]$Path, [string]$Expected) {
    if (-not (Test-Path -LiteralPath $Path -PathType Leaf)) { throw "Missing runtime input: $Path" }
    $actual = (Get-FileHash -LiteralPath $Path -Algorithm SHA256).Hash.ToLowerInvariant()
    if ($actual -ne $Expected.ToLowerInvariant()) { throw "Runtime input SHA256 mismatch: $Path" }
    return $actual
}

if (-not $RunId) { $RunId = [DateTime]::UtcNow.ToString('yyyyMMddTHHmmssfffZ') + '-' + [Guid]::NewGuid().ToString('N').Substring(0, 8) }
if (-not (Test-Path -LiteralPath $sourceExe -PathType Leaf)) { throw 'Install the pinned PPSSPP 1.20.4 distribution first.' }
$installation = Get-Content -LiteralPath (Join-Path $toolRoot '.ppsspp-installed.json') -Raw | ConvertFrom-Json
if ($installation.version -ne '1.20.4 Windows x64' -or $installation.sha256 -ne 'fbc9cd2f5131b159a92424e5c458c35ce43ba603cdded64dfc98e4bd4f17ff93') { throw 'PPSSPP installation marker does not identify the pinned baseline.' }

if ($Case -eq 'retail') {
    $iso = Join-Path $projectRoot 'Dissidia 012 - Duodecim Final Fantasy (USA) (En,Fr,De,Es,It).iso'
    $isoHash = Assert-Hash $iso 'df94177d3e194e82800a574b50b61ee3c7be6caa877c8e82242a41af75d1ab6e'
} else {
    if (-not (Test-Path -LiteralPath $manifestPath -PathType Leaf)) { throw 'Build runtime packages before launching the original or rebuilt case.' }
    $manifest = Get-Content -LiteralPath $manifestPath -Raw | ConvertFrom-Json
    if ($manifest.schema_version -ne 1) { throw 'Unsupported runtime package manifest schema.' }
    $python = Join-Path $projectRoot '.venv/Scripts/python.exe'
    & $python -c 'import json,sys; sys.path.insert(0,sys.argv[1]); from prepare_runtime_packages import validate_builds; assert validate_builds() == json.load(open(sys.argv[2]))["build_inputs"], "Runtime package build proofs are stale"' $PSScriptRoot $manifestPath
    if ($LASTEXITCODE -ne 0) { throw 'Runtime packages require current build proofs; prepare packages again after changing recovered source.' }
    $label = if ($Case -eq 'original') { 'original-decrypted' } else { 'rebuilt' }
    $packages = @($manifest.packages | Where-Object { $_.label -eq $label })
    if ($packages.Count -ne 1) { throw "Expected one package entry for $label" }
    $iso = [IO.Path]::GetFullPath((Join-Path $projectRoot $packages[0].path))
    $packageRoot = [IO.Path]::GetFullPath((Join-Path $runtimeRoot 'packages')) + [IO.Path]::DirectorySeparatorChar
    if (-not $iso.StartsWith($packageRoot, [StringComparison]::OrdinalIgnoreCase)) { throw 'Package must remain within the workspace runtime/packages directory.' }
    $isoHash = Assert-Hash $iso $packages[0].sha256
}

# The pinned Windows build selects its memory stick from installed.txt beside
# the executable. A fresh executable directory prevents use of existing saves.
$sessionRoot = Join-Path $runtimeRoot "sessions/$Case/$RunId"
if (Test-Path -LiteralPath $sessionRoot) { throw "Runtime session already exists; choose a new RunId: $sessionRoot" }
$exeRoot = Join-Path $sessionRoot 'emulator'
$memstick = Join-Path $sessionRoot 'memstick'
$configRoot = Join-Path $memstick 'PSP/SYSTEM'
New-Item -ItemType Directory -Path $exeRoot, $configRoot | Out-Null
$exe = Join-Path $exeRoot 'PPSSPPWindows64.exe'
Copy-Item -LiteralPath $sourceExe -Destination $exe
Copy-Item -LiteralPath (Join-Path $toolRoot 'assets') -Destination $exeRoot -Recurse
[IO.File]::WriteAllText((Join-Path $exeRoot 'installed.txt'), $memstick, $utf8NoBom)

if ($Port -eq 0) {
    $listener = [Net.Sockets.TcpListener]::new([Net.IPAddress]::Loopback, 0)
    try { $listener.Start(); $Port = $listener.LocalEndpoint.Port } finally { $listener.Stop() }
}
$ini = Join-Path $configRoot 'ppsspp.ini'
$controls = Join-Path $configRoot 'controls.ini'
$log = Join-Path $sessionRoot 'ppsspp.log'
$template = [IO.File]::ReadAllText($templatePath)
if ($template -match '(?im)^Remote(?:Debugger|Share)OnStartup\s*=\s*(True|1)\s*$') {
    throw 'Remote services require verified network isolation; this baseline launcher keeps them disabled.'
}
if ([regex]::Matches($template, '(?m)^RemoteISOPort\s*=\s*0\s*$').Count -ne 1) { throw 'Expected exactly one debugger-port placeholder in runtime config.' }
$configuration = [regex]::Replace($template, '(?m)^RemoteISOPort\s*=\s*0\s*$', "RemoteISOPort = $Port")
if ($CaptureVideo) { $configuration = $configuration.Replace('DumpFrames = False', 'DumpFrames = True') }
[IO.File]::WriteAllText($ini, $configuration, $utf8NoBom)
[IO.File]::WriteAllText($controls, "[ControlMapping]`n", $utf8NoBom)

# Start-Process joins ArgumentList into one Windows command line; quote every
# path explicitly. None of these paths can contain a double quote on Windows.
$launchArgs = @("--config=`"$ini`"", "--controlconfig=`"$controls`"", "--graphics=$Graphics", "--log=`"$log`"", '--loglevel=4', "`"$iso`"")
$session = [ordered]@{
    schema_version = 1; case = $Case; run_id = $RunId; session_root = $sessionRoot
    ppsspp_version = '1.20.4'; exe = $exe; exe_sha256 = (Get-FileHash -LiteralPath $exe -Algorithm SHA256).Hash.ToLowerInvariant()
    config = $ini; config_sha256 = (Get-FileHash -LiteralPath $ini -Algorithm SHA256).Hash.ToLowerInvariant()
    control_config = $controls; memstick = $memstick; log = $log
    iso = $iso; iso_sha256 = $isoHash; debugger_enabled = $false; configured_port = $Port; capture_video = [bool]$CaptureVideo
    graphics = $Graphics; arguments = $launchArgs; pid = $null; process_start_utc = $null
    prepared_utc = [DateTime]::UtcNow.ToString('o'); started = $false; stopped = $false
}
$sessionJson = Join-Path $sessionRoot 'session.json'
if (-not $PrepareOnly) {
    $process = Start-Process -FilePath $exe -ArgumentList $launchArgs -WorkingDirectory $exeRoot -WindowStyle Hidden -PassThru
    $session.pid = $process.Id
    $session.process_start_utc = $process.StartTime.ToUniversalTime().ToString('o')
    $session.started = $true
}
$json = $session | ConvertTo-Json -Depth 6
[IO.File]::WriteAllText($sessionJson, $json, $utf8NoBom)
Write-Output $json
