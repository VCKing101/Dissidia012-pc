param([switch]$ExtraProject, [ValidateSet('extra','main','menu','battle')][string]$Module,
      [ValidatePattern('^[a-z0-9-]+$')][string]$ProjectName)
$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path $PSScriptRoot -Parent
$java = Join-Path $projectRoot 'tools/local/jdk-21.0.12.1+1/bin/javaw.exe'
$settingsRoot = Join-Path $projectRoot 'analysis/ghidra-home'
$classpath = Join-Path $projectRoot 'tools/local/ghidra_12.1.3_PUBLIC/Ghidra/Framework/Utility/lib/Utility.jar'
$launchArgs = @('-Xmx2G', '-Djava.system.class.loader=ghidra.GhidraClassLoader', '-Dfile.encoding=UTF8', "-Duser.home=$settingsRoot", "-Dapplication.settingsdir=$settingsRoot/settings", "-Dapplication.cachedir=$settingsRoot/cache", "-Djava.util.prefs.userRoot=$settingsRoot/prefs", '-cp', $classpath, 'ghidra.Ghidra', 'ghidra.GhidraRun')
if ($ExtraProject) { $Module = 'extra' }
if ($Module -or $ProjectName) {
    if (-not $ProjectName) { $ProjectName = "dissidia-$Module" }
    $projectFile = Join-Path $projectRoot "analysis/ghidra-projects/$ProjectName.gpr"
    if (-not (Test-Path -LiteralPath $projectFile)) { throw "The $Module project has not been imported yet" }
    $launchArgs += $projectFile
}
Start-Process -FilePath $java -ArgumentList $launchArgs -WorkingDirectory $projectRoot -WindowStyle Hidden | Out-Null
