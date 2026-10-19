param([ValidatePattern('^[a-zA-Z0-9][a-zA-Z0-9_-]{0,63}$')][string]$RunId = 'debugger-baseline-v1')
$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest
$projectRoot = Split-Path $PSScriptRoot -Parent
$runtimeRoot = Join-Path $projectRoot 'build/ULUS10566/runtime'
$resultPath = Join-Path $runtimeRoot "isolation-$RunId.json"
$result = [ordered]@{ schema_version = 1; run_id = $RunId; success = $false; sessions = @(); error = $null }
try {
    foreach ($case in @('retail', 'original', 'rebuilt')) {
        $sessionRoot = Join-Path $runtimeRoot "sessions/$case/$RunId"
        $plan = & (Join-Path $PSScriptRoot 'protect_runtime_session.ps1') -SessionRoot $sessionRoot -WhatIf | ConvertFrom-Json
        $existing = @(Get-NetFirewallRule -Name $plan.rule_names -ErrorAction SilentlyContinue)
        if (-not $existing.Count) {
            & (Join-Path $PSScriptRoot 'protect_runtime_session.ps1') -SessionRoot $sessionRoot | Out-Null
        }
        $profiles = @(Get-NetFirewallProfile -PolicyStore ActiveStore)
        if ($profiles | Where-Object { -not $_.Enabled -or $_.AllowLocalFirewallRules.ToString() -eq 'False' }) { throw 'Effective firewall profiles do not permit relying on local rules.' }
        foreach ($service in @('BFE', 'MpsSvc')) {
            if ((Get-Service $service).Status -ne 'Running') { throw "Firewall service $service is not running." }
        }
        $checked = @()
        foreach ($name in $plan.rule_names) {
            $rule = Get-NetFirewallRule -PolicyStore ActiveStore -Name $name
            $application = $rule | Get-NetFirewallApplicationFilter
            $address = $rule | Get-NetFirewallAddressFilter
            $port = $rule | Get-NetFirewallPortFilter
            if ($rule.Group -ne $plan.group -or $application.Program -ne $plan.program -or $rule.Enabled.ToString() -ne 'True' -or $rule.Action.ToString() -ne 'Block' -or $rule.Profile.ToString() -ne 'Any' -or $port.Protocol -ne 'Any') { throw "Unexpected effective filter for $name" }
            if (Compare-Object @($plan.remote_addresses) @($address.RemoteAddress)) { throw "Unexpected remote address filter for $name" }
            $checked += [ordered]@{ name = $name; program = $application.Program; enabled = $rule.Enabled.ToString(); action = $rule.Action.ToString(); direction = $rule.Direction.ToString(); profile = $rule.Profile.ToString(); remote_addresses = @($address.RemoteAddress) }
        }
        if (@($checked | Where-Object { $_.direction -eq 'Inbound' }).Count -ne 1 -or @($checked | Where-Object { $_.direction -eq 'Outbound' }).Count -ne 1) { throw 'Expected one rule in each direction.' }
        $result.sessions += [ordered]@{ case = $case; session_root = $sessionRoot; rules = $checked }
    }
    $result.success = $true
} catch {
    $result.error = $_.Exception.Message
} finally {
    $result['finished_utc'] = [DateTime]::UtcNow.ToString('o')
    [IO.File]::WriteAllText($resultPath, ($result | ConvertTo-Json -Depth 8), (New-Object System.Text.UTF8Encoding($false)))
}
if (-not $result.success) { Write-Error $result.error; exit 1 }
