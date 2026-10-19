param(
    [ValidateSet('main','menu','battle','extra')][string]$Module,
    [ValidateRange(1,3600)][int]$AnalysisTimeoutSeconds = 600,
    [ValidatePattern('^[a-z0-9-]+$')][string]$ProjectName,
    [switch]$AuditReferences,
    [switch]$SeedOverlayHooks,
    [switch]$SeedReviewedEntries,
    [ValidatePattern('^[a-z0-9-]+$')][string]$SnapshotName
)
$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path $PSScriptRoot -Parent
$scope = Get-Content -LiteralPath (Join-Path $projectRoot 'config/progress.json') -Raw | ConvertFrom-Json
$entry = $scope.modules | Where-Object id -eq $Module
if (-not $entry) { throw 'Specify a configured module with -Module' }
$inputPath = Join-Path $projectRoot $entry.path
if ((Get-FileHash -LiteralPath $inputPath -Algorithm SHA256).Hash.ToLowerInvariant() -ne $entry.sha256) {
    throw "Reference hash mismatch for $Module"
}
$settingsRoot = Join-Path $projectRoot "analysis/ghidra-inventory-home-$Module"
$projectFolder = Join-Path $projectRoot 'analysis/ghidra-projects'
$outputFolder = Join-Path $projectRoot "analysis/ULUS10566/$Module"
if ($SnapshotName) { $outputFolder = Join-Path $outputFolder $SnapshotName }
New-Item -ItemType Directory -Force -Path $settingsRoot,$projectFolder,$outputFolder | Out-Null
$java = Join-Path $projectRoot 'tools/local/jdk-21.0.12.1+1/bin/java.exe'
$classpath = Join-Path $projectRoot 'tools/local/ghidra_12.1.3_PUBLIC/Ghidra/Framework/Utility/lib/Utility.jar'
if (-not $ProjectName) { $ProjectName = "dissidia-$Module" }
$projectFile = Join-Path $projectFolder "$projectName.gpr"
# Existing programs are processed read-only without rerunning analysis.
# New imports go into separate projects; never overwrite manual annotations.
if (Test-Path -LiteralPath $projectFile) {
    if ($SeedOverlayHooks -or $SeedReviewedEntries) { throw 'Entry seeding requires a new project name to preserve existing annotations' }
    $operation = @('-process', (Split-Path $inputPath -Leaf), '-noanalysis', '-readOnly')
    $analysisMode = 'existing'
} else {
    if ($AuditReferences) { throw 'Import/analyze the module before exporting audit references' }
    if ($Module -eq 'extra') { throw 'The analyzed extra-overlay project is missing; use tools/run_ghidra.ps1 first' }
    $operation = @('-import', $inputPath, '-processor', 'Allegrex:LE:32:default',
                   '-analysisTimeoutPerFile', "$AnalysisTimeoutSeconds", '-max-cpu', '2')
    $analysisMode = 'fresh'
}
$arguments = @('-Xmx2G', '-Djava.system.class.loader=ghidra.GhidraClassLoader', '-Dfile.encoding=UTF8',
    "-Duser.home=$settingsRoot", "-Dapplication.settingsdir=$settingsRoot/settings",
    "-Dapplication.cachedir=$settingsRoot/cache", "-Djava.util.prefs.userRoot=$settingsRoot/prefs",
    '-cp', $classpath, 'ghidra.Ghidra', 'ghidra.app.util.headless.AnalyzeHeadless', $projectFolder, $projectName)
$arguments += $operation
if ($SeedOverlayHooks) {
    if ($Module -notin @('menu','battle')) { throw 'Hook seeding applies to menu and battle overlays' }
    $arguments += @('-scriptPath', (Join-Path $PSScriptRoot 'ghidra'), '-postScript',
        'SeedOverlayHooks.java', $outputFolder, $Module)
}
if ($SeedReviewedEntries) {
    $arguments += @('-scriptPath', (Join-Path $PSScriptRoot 'ghidra'), '-postScript',
        'SeedReviewedEntries.java', (Join-Path $projectRoot 'config/overlay-dispatch.json'), $Module)
}
$scriptName = if ($AuditReferences) { 'ExportAuditReferences.java' } else { 'ExportInventory.java' }
$outputName = if ($AuditReferences) { 'audit-references.json' } else { 'inventory.json' }
$arguments += @('-scriptPath', (Join-Path $PSScriptRoot 'ghidra'), '-postScript', $scriptName,
    $outputFolder, $Module, $analysisMode, $projectName, '-log', (Join-Path $outputFolder 'inventory-headless.log'))
$runStarted = [DateTime]::UtcNow
& $java @arguments
if ($LASTEXITCODE -ne 0) { throw "Ghidra inventory failed with exit code $LASTEXITCODE" }
if (-not (Test-Path -LiteralPath (Join-Path $outputFolder $outputName))) { throw "Ghidra did not export $outputName" }
if ((Get-Item -LiteralPath (Join-Path $outputFolder $outputName)).LastWriteTimeUtc -lt $runStarted) {
    throw "Ghidra did not refresh $outputName; inspect the headless log for script errors"
}
