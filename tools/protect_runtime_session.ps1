[CmdletBinding(SupportsShouldProcess = $true)]
param(
    [Parameter(Mandatory = $true)][string]$SessionRoot,
    [switch]$Remove
)
$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest
$projectRoot = Split-Path $PSScriptRoot -Parent
$allowedRoot = [IO.Path]::GetFullPath((Join-Path $projectRoot 'build/ULUS10566/runtime/sessions')) + [IO.Path]::DirectorySeparatorChar
$resolvedRoot = (Resolve-Path -LiteralPath $SessionRoot).Path
if (-not $resolvedRoot.StartsWith($allowedRoot, [StringComparison]::OrdinalIgnoreCase)) { throw 'Only workspace runtime sessions may be protected.' }
$session = Get-Content -LiteralPath (Join-Path $resolvedRoot 'session.json') -Raw | ConvertFrom-Json
$exe = [IO.Path]::GetFullPath((Join-Path $resolvedRoot 'emulator/PPSSPPWindows64.exe'))
if ($session.schema_version -ne 1 -or [IO.Path]::GetFullPath($session.exe) -ne $exe) { throw 'Invalid runtime executable record.' }
if ((Get-FileHash -LiteralPath $exe -Algorithm SHA256).Hash.ToLowerInvariant() -ne $session.exe_sha256) { throw 'Runtime executable has changed.' }
$pathHash = [Security.Cryptography.SHA256]::Create()
try { $tag = ([BitConverter]::ToString($pathHash.ComputeHash([Text.Encoding]::UTF8.GetBytes($exe.ToLowerInvariant())))).Replace('-', '').Substring(0, 16) } finally { $pathHash.Dispose() }
$group = 'Dissidia decomp runtime isolation'
# Exclude IPv4 loopback 127/8 and IPv6 loopback ::1. No allow rule is added.
$remoteAddresses = @('0.0.0.0-126.255.255.255', '128.0.0.0-255.255.255.255', '::', '::2-ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff')
$ruleNames = @("DissidiaRuntime-$tag-Inbound", "DissidiaRuntime-$tag-Outbound")
$plan = [ordered]@{
    session_root = $resolvedRoot; program = $exe; action = $(if ($Remove) { 'remove' } else { 'apply' })
    group = $group; rule_names = $ruleNames; directions = @('Inbound', 'Outbound')
    remote_addresses = $remoteAddresses; profiles = 'All'; protocol = 'Any'; rule_action = 'Block'
    remote_services_enabled_by_this_script = $false
}
if ($WhatIfPreference) {
    # Review requires no administrator access, firewall access or changes.
    $plan | ConvertTo-Json -Depth 5
    return
}
$principal = [Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()
if (-not $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) { throw 'Apply/remove requires an administrator PowerShell session. Use -WhatIf to review the exact plan without changing anything.' }
if (Get-NetFirewallProfile | Where-Object { -not $_.Enabled }) { throw 'All firewall profiles must be enabled before relying on these rules.' }
if (Get-Process PPSSPPWindows64 -ErrorAction SilentlyContinue | Where-Object { $_.Path -eq $exe }) { throw 'Stop this owned emulator before changing its protection.' }
$existing = @()
foreach ($name in $ruleNames) {
    $rule = Get-NetFirewallRule -Name $name -ErrorAction SilentlyContinue
    if ($rule) {
        $program = ($rule | Get-NetFirewallApplicationFilter).Program
        if ($rule.Group -ne $group -or $program -ne $exe) { throw 'A rule-name collision belongs to another program; refusing to alter it.' }
        $existing += $rule
    }
}
if ($Remove) {
    foreach ($rule in $existing) {
        if ($PSCmdlet.ShouldProcess($rule.Name, 'Remove this session firewall rule')) { $rule | Remove-NetFirewallRule }
    }
} else {
    if ($existing.Count) { throw 'Session rules already exist; inspect or remove them before applying a fresh pair.' }
    $created = @()
    try {
        for ($index = 0; $index -lt 2; $index++) {
            $name = $ruleNames[$index]
            $direction = @('Inbound', 'Outbound')[$index]
            if ($PSCmdlet.ShouldProcess($exe, "Block non-loopback $direction network traffic")) {
                $created += New-NetFirewallRule -Name $name -DisplayName $name -Group $group -Program $exe -Direction $direction -Action Block -Enabled True -Profile Any -Protocol Any -RemoteAddress $remoteAddresses
            }
        }
    } catch {
        foreach ($rule in $created) { $rule | Remove-NetFirewallRule }
        throw
    }
}
$plan | ConvertTo-Json -Depth 5
# This is not a runtime isolation certificate. A later debugger launcher must
# verify live effective filters and loopback/external behavior before starting.
