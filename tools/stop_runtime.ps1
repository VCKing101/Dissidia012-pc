param([Parameter(Mandatory = $true)][string]$SessionRoot)
$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest
$projectRoot = Split-Path $PSScriptRoot -Parent
$allowedRoot = [IO.Path]::GetFullPath((Join-Path $projectRoot 'build/ULUS10566/runtime/sessions')) + [IO.Path]::DirectorySeparatorChar
$resolvedRoot = (Resolve-Path -LiteralPath $SessionRoot).Path
if (-not $resolvedRoot.StartsWith($allowedRoot, [StringComparison]::OrdinalIgnoreCase)) { throw 'Only workspace runtime sessions may be stopped.' }
$sessionPath = Join-Path $resolvedRoot 'session.json'
$session = Get-Content -LiteralPath $sessionPath -Raw | ConvertFrom-Json
$expectedExe = [IO.Path]::GetFullPath((Join-Path $resolvedRoot 'emulator/PPSSPPWindows64.exe'))
if ($session.schema_version -ne 1 -or [IO.Path]::GetFullPath($session.exe) -ne $expectedExe) { throw 'Invalid runtime process record.' }
if (-not $session.started -or -not $session.pid) { throw 'This session has no recorded process.' }
$process = Get-Process -Id $session.pid -ErrorAction SilentlyContinue
$alreadyExited = $null -eq $process
if ($process) {
    if ($process.Path -ne $expectedExe) { throw 'PID belongs to a different executable; refusing to stop it.' }
    $recordedStart = ([datetime]$session.process_start_utc).ToUniversalTime()
    if ($process.StartTime.ToUniversalTime().Ticks -ne $recordedStart.Ticks) { throw 'PID has been reused; refusing to stop it.' }
    if ($process.CloseMainWindow()) { $process.WaitForExit(10000) | Out-Null }
    if (-not $process.HasExited) {
        Stop-Process -Id $process.Id
        if (-not $process.WaitForExit(5000)) { throw 'Owned emulator has not finished stopping; startup settings were not reset.' }
    }
}
# Reset startup settings after the owned process has finished. Future debugger
# launches must go through the live firewall checks again.
$ownedConfig = Join-Path $resolvedRoot 'memstick/PSP/SYSTEM/ppsspp.ini'
if (Test-Path -LiteralPath $ownedConfig -PathType Leaf) {
    $configuration = [IO.File]::ReadAllText($ownedConfig)
    $configuration = [regex]::Replace($configuration, '(?im)^Remote(?:Debugger|Share)OnStartup\s*=.*$', { param($match) ($match.Value -split '=')[0] + '= False' })
    [IO.File]::WriteAllText($ownedConfig, $configuration, (New-Object System.Text.UTF8Encoding($false)))
}
$session | Add-Member -NotePropertyName remote_startup_disabled_after_stop -NotePropertyValue $true -Force
$session.stopped = $true
$session | Add-Member -MemberType NoteProperty -Name stopped_utc -Value ([DateTime]::UtcNow.ToString('o')) -Force
$session | Add-Member -MemberType NoteProperty -Name already_exited -Value $alreadyExited -Force
$json = $session | ConvertTo-Json -Depth 6
[IO.File]::WriteAllText($sessionPath, $json, (New-Object System.Text.UTF8Encoding($false)))
Write-Output $json
