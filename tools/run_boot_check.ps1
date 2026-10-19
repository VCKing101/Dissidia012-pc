param(
    [Parameter(Mandatory = $true)][ValidateSet('retail', 'original', 'rebuilt')][string]$Case,
    [ValidatePattern('^[a-zA-Z0-9][a-zA-Z0-9_-]{0,63}$')][string]$RunId,
    [ValidateRange(15, 120)][int]$Seconds = 30
)
$ErrorActionPreference = 'Stop'
# Keep the owned child inside this invocation until capture and shutdown finish.
# A successful launch alone is not a successful boot check.
$launchOptions = @{ Case = $Case; CaptureVideo = $true }
if ($RunId) { $launchOptions.RunId = $RunId }
$record = & (Join-Path $PSScriptRoot 'start_runtime.ps1') @launchOptions | ConvertFrom-Json
$process = Get-Process -Id $record.pid -ErrorAction SilentlyContinue
$aliveAfterWait = $false
$exitCode = $null
try {
    if ($process) {
        $aliveAfterWait = -not $process.WaitForExit($Seconds * 1000)
        if (-not $aliveAfterWait) { $exitCode = $process.ExitCode }
    }
} finally {
    & (Join-Path $PSScriptRoot 'stop_runtime.ps1') -SessionRoot $record.session_root | Out-Null
}
$result = [ordered]@{
    schema_version = 1; case = $Case; session_root = $record.session_root
    requested_seconds = $Seconds; alive_after_wait = $aliveAfterWait
    early_exit_code = $exitCode; debugger_enabled = $false
    scenario = 'boot-with-no-input'; observed_screen = $null
    finished_utc = [DateTime]::UtcNow.ToString('o')
}
$json = $result | ConvertTo-Json
[IO.File]::WriteAllText((Join-Path $record.session_root 'boot-check.json'), $json, (New-Object System.Text.UTF8Encoding($false)))
Write-Output $json
if (-not $aliveAfterWait) { throw 'Emulator exited before the observation period; inspect this session before claiming a boot pass.' }
