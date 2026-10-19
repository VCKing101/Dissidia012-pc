param(
    [Parameter(Mandatory = $true)][string]$SessionRoot,
    [ValidateRange(30, 3600)][int]$HoldSeconds = 900
)
$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest
$principal = [Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()
if ($principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) { throw 'Launch the emulator from a normal-user shell; only firewall setup needs administrator privileges.' }
$projectRoot = Split-Path $PSScriptRoot -Parent
$allowedRoot = [IO.Path]::GetFullPath((Join-Path $projectRoot 'build/ULUS10566/runtime/sessions')) + [IO.Path]::DirectorySeparatorChar
$resolvedRoot = (Resolve-Path -LiteralPath $SessionRoot).Path
if (-not $resolvedRoot.StartsWith($allowedRoot, [StringComparison]::OrdinalIgnoreCase)) { throw 'Expected an owned runtime session.' }
$sessionPath = Join-Path $resolvedRoot 'session.json'
$session = Get-Content -LiteralPath $sessionPath -Raw | ConvertFrom-Json
if ($session.started -and -not $session.stopped) { throw 'This session is already running.' }
$plan = & (Join-Path $PSScriptRoot 'protect_runtime_session.ps1') -SessionRoot $resolvedRoot -WhatIf | ConvertFrom-Json
if (Get-NetFirewallProfile -PolicyStore ActiveStore | Where-Object { -not $_.Enabled -or $_.AllowLocalFirewallRules.ToString() -eq 'False' }) { throw 'Effective firewall profiles do not support isolation.' }
foreach ($service in @('BFE', 'MpsSvc')) { if ((Get-Service $service).Status -ne 'Running') { throw 'Firewall services must be running.' } }
$directions = @()
foreach ($name in $plan.rule_names) {
    $rule = Get-NetFirewallRule -PolicyStore ActiveStore -Name $name
    $application = $rule | Get-NetFirewallApplicationFilter
    $address = $rule | Get-NetFirewallAddressFilter
    $port = $rule | Get-NetFirewallPortFilter
    if ($rule.Group -ne $plan.group -or $application.Program -ne $session.exe -or $rule.Enabled.ToString() -ne 'True' -or $rule.Action.ToString() -ne 'Block' -or $rule.Profile.ToString() -ne 'Any' -or $port.Protocol -ne 'Any' -or $port.LocalPort -ne 'Any' -or $port.RemotePort -ne 'Any') { throw 'Unexpected effective firewall filter.' }
    if (Compare-Object @($plan.remote_addresses) @($address.RemoteAddress)) { throw 'Unexpected remote address filter.' }
    $directions += $rule.Direction.ToString()
}
if (Compare-Object @('Inbound', 'Outbound') $directions) { throw 'Both firewall directions are required.' }
if ((Get-FileHash -LiteralPath $session.iso -Algorithm SHA256).Hash.ToLowerInvariant() -ne $session.iso_sha256) { throw 'ISO differs from prepared session.' }
if ($session.case -ne 'retail') {
    $python = Join-Path $projectRoot '.venv/Scripts/python.exe'
    & $python -c 'import json,sys; sys.path.insert(0,sys.argv[1]); from prepare_runtime_packages import validate_builds; assert validate_builds() == json.load(open(sys.argv[2]))["build_inputs"]' $PSScriptRoot (Join-Path $projectRoot 'build/ULUS10566/runtime/packages/manifest.json')
    if ($LASTEXITCODE -ne 0) { throw 'Current build provenance is required.' }
}
$emptyShare = Join-Path $resolvedRoot 'empty-share'
New-Item -ItemType Directory -Path $emptyShare -Force | Out-Null
if (Get-ChildItem -LiteralPath $emptyShare -Force) { throw 'The network share directory must be empty.' }
$configuration = [IO.File]::ReadAllText($session.config)
$configuration = [regex]::Replace($configuration, '(?im)^RemoteDebuggerOnStartup\s*=.*$', 'RemoteDebuggerOnStartup = True')
$configuration = [regex]::Replace($configuration, '(?im)^RemoteShareOnStartup\s*=.*$', 'RemoteShareOnStartup = False')
$configuration = [regex]::Replace($configuration, '(?im)^RemoteISOShareType\s*=.*\r?\n?', '')
$configuration = [regex]::Replace($configuration, '(?im)^RemoteISOSharedDir\s*=.*\r?\n?', '')
$configuration = $configuration.Replace('[General]', "[General]`nRemoteISOShareType = 1`nRemoteISOSharedDir = $emptyShare")
[IO.File]::WriteAllText($session.config, $configuration, (New-Object System.Text.UTF8Encoding($false)))
$session.config_sha256 = (Get-FileHash -LiteralPath $session.config -Algorithm SHA256).Hash.ToLowerInvariant()
$session.debugger_enabled = $true
$session.stopped = $false
$session | Add-Member -NotePropertyName isolation_rule_names -NotePropertyValue $plan.rule_names -Force
$process = Start-Process -FilePath $session.exe -ArgumentList $session.arguments -WorkingDirectory (Split-Path $session.exe -Parent) -WindowStyle Hidden -PassThru
$session.pid = $process.Id
$session.process_start_utc = $process.StartTime.ToUniversalTime().ToString('o')
$session.started = $true
[IO.File]::WriteAllText($sessionPath, ($session | ConvertTo-Json -Depth 8), (New-Object System.Text.UTF8Encoding($false)))
[pscustomobject]@{case=$session.case;pid=$process.Id;port=$session.configured_port;session_root=$resolvedRoot;effective_isolation_checked=$true} | ConvertTo-Json
$deadline = [DateTime]::UtcNow.AddSeconds($HoldSeconds)
try {
    while ([DateTime]::UtcNow -lt $deadline) {
        $process.Refresh()
        if ($process.HasExited) { break }
        Start-Sleep -Seconds 1
    }
} finally {
    & (Join-Path $PSScriptRoot 'stop_runtime.ps1') -SessionRoot $resolvedRoot | Out-Null
}
