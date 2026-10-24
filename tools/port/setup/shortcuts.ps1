# Makes the desktop shortcut and the Start Menu entry that start the port with
# tools\port\play_d012.ps1 in a hidden PowerShell window. Called by setup.ps1.
#   shortcuts.ps1 -Root <repo> [-Icon <ico>] [-Name <name>] [-Folder <dir>]
# -Folder puts both under <dir>\Desktop and <dir>\Start Menu instead (for tests).
param(
    [Parameter(Mandatory = $true)][string]$Root,
    [string]$Icon,
    [string]$Name = "Dissidia 012 (PC port)",
    [string]$Folder
)
$ErrorActionPreference = "Stop"
if ($Folder) {
    $places = @((Join-Path $Folder "Desktop"), (Join-Path $Folder "Start Menu"))
} else {
    $places = @([Environment]::GetFolderPath("Desktop"), [Environment]::GetFolderPath("Programs"))
}
$powershell = Join-Path $env:SystemRoot "System32\WindowsPowerShell\v1.0\powershell.exe"
$play = Join-Path $Root "tools\port\play_d012.ps1"
$shell = New-Object -ComObject WScript.Shell
foreach ($place in $places) {
    New-Item -ItemType Directory -Force $place | Out-Null
    $path = Join-Path $place "$Name.lnk"
    $link = $shell.CreateShortcut($path)
    $link.TargetPath = $powershell
    $link.Arguments = "-NoProfile -WindowStyle Hidden -ExecutionPolicy Bypass -File `"$play`""
    $link.WorkingDirectory = $Root
    $link.WindowStyle = 7
    $link.Description = "Dissidia 012 Duodecim Final Fantasy, native PC port"
    if ($Icon -and (Test-Path $Icon)) { $link.IconLocation = "$Icon,0" }
    $link.Save()
    Write-Output $path
}
