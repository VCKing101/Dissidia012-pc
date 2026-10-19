param([switch]$Usa)
$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path $PSScriptRoot -Parent
$folder = Join-Path $projectRoot 'tools/local/ppsspp'
$executable = Join-Path $folder 'PPSSPPWindows64.exe'
if ($Usa) {
    $iso = Join-Path $projectRoot 'Dissidia 012 - Duodecim Final Fantasy (USA) (En,Fr,De,Es,It).iso'
    Start-Process -FilePath $executable -ArgumentList ('"' + $iso + '"') -WorkingDirectory $folder -WindowStyle Hidden | Out-Null
} else {
    Start-Process -FilePath $executable -WorkingDirectory $folder -WindowStyle Hidden | Out-Null
}
