param(
    [string]$InputFile = 'orig/ULUS10566/PSP_GAME/USRDIR/DATA/OVL_EXTRA_APP.ELF',
    [string]$ProjectName = 'dissidia-extra',
    [string]$OutputFolder = 'analysis/ULUS10566/extra'
)
$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path $PSScriptRoot -Parent
$env:JAVA_HOME = Join-Path $projectRoot 'tools/local/jdk-21.0.12.1+1'
$settingsRoot = Join-Path $projectRoot 'analysis/ghidra-home'
$projectFolder = Join-Path $projectRoot 'analysis/ghidra-projects'
New-Item -ItemType Directory -Force -Path $projectFolder,$settingsRoot | Out-Null
$java = Join-Path $env:JAVA_HOME 'bin/java.exe'
$classpath = Join-Path $projectRoot 'tools/local/ghidra_12.1.3_PUBLIC/Ghidra/Framework/Utility/lib/Utility.jar'
# Use the bundled Java directly, keeping settings inside the project and avoiding
# LaunchSupport's user-profile JDK registration step.
& $java '-Xmx2G' '-Djava.system.class.loader=ghidra.GhidraClassLoader' '-Dfile.encoding=UTF8' "-Duser.home=$settingsRoot" "-Dapplication.settingsdir=$settingsRoot/settings" "-Dapplication.cachedir=$settingsRoot/cache" "-Djava.util.prefs.userRoot=$settingsRoot/prefs" -cp $classpath ghidra.Ghidra ghidra.app.util.headless.AnalyzeHeadless $projectFolder $ProjectName -import (Join-Path $projectRoot $InputFile) -overwrite -processor 'Allegrex:LE:32:default' -scriptPath (Join-Path $PSScriptRoot 'ghidra') -postScript ExportFunctions.java (Join-Path $projectRoot $OutputFolder) -analysisTimeoutPerFile 180 -log (Join-Path $projectRoot 'analysis/ghidra-headless.log')
if ($LASTEXITCODE -ne 0) { throw "Ghidra failed with exit code $LASTEXITCODE" }
