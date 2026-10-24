# Prerequisite checks for setup.ps1 (dot-sourced). Nothing is installed here: a missing
# prerequisite is reported with what to install, a link and a winget command the user may run.

function New-Problem([string]$What, [string]$Fix, [string]$Link, [string]$Winget) {
    [pscustomobject]@{ What = $What; Fix = $Fix; Link = $Link; Winget = $Winget }
}

function Find-VisualStudio {
    $vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
    if (-not (Test-Path $vswhere)) { return $null }
    $found = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
        -format json | ConvertFrom-Json
    if ($found) { return @($found)[0] }
    $any = & $vswhere -latest -products * -format json | ConvertFrom-Json
    if ($any) { return [pscustomobject]@{ installationPath = $null; displayName = @($any)[0].displayName } }
    return $null
}

function Find-Python {
    # The py launcher first, then python on PATH (the Microsoft Store stub prints nothing useful).
    $candidates = @()
    if (Get-Command py -ErrorAction SilentlyContinue) { $candidates += , @("py", "-3") }
    if (Get-Command python -ErrorAction SilentlyContinue) { $candidates += , @("python") }
    foreach ($candidate in $candidates) {
        $exe = $candidate[0]
        $rest = @($candidate | Select-Object -Skip 1)
        try {
            $out = & $exe @rest -c "import sys; print('%d %d %s' % (sys.version_info[0], sys.version_info[1], sys.executable))" 2>$null
        } catch { continue }
        if ($LASTEXITCODE -ne 0 -or -not $out) { continue }
        $parts = "$out".Trim() -split " ", 3
        if ($parts.Count -lt 3) { continue }
        $version = [int]$parts[0] * 100 + [int]$parts[1]
        if ($version -ge 310) {
            return [pscustomobject]@{ Path = $parts[2]; Version = "$($parts[0]).$($parts[1])" }
        }
    }
    return $null
}

function Get-VsProbe([string]$Root) {
    $probe = @{}
    $lines = @()
    try { $lines = & (Join-Path $Root "tools\port\setup\vsenv.cmd") --probe 2>$null } catch { }
    foreach ($line in $lines) {
        $name, $value = "$line" -split "=", 2
        if ($name) { $probe[$name.Trim()] = "$value".Trim() }
    }
    return $probe
}

function Test-Prerequisites([string]$Root, [bool]$FirstRun) {
    $problems = @()
    $result = [pscustomobject]@{ Problems = $null; Python = $null }

    # Visual Studio with the C++ workload, and its CMake, Ninja and Windows SDK
    $vs = Find-VisualStudio
    if (-not $vs) {
        $problems += New-Problem "Visual Studio 2022 (or its Build Tools) with the C++ tools" `
            "Install Visual Studio 2022 Community (free) with the workload 'Desktop development with C++'." `
            "https://visualstudio.microsoft.com/downloads/" `
            'winget install --id Microsoft.VisualStudio.2022.Community --override "--wait --passive --add Microsoft.VisualStudio.Workload.NativeDesktop --includeRecommended"'
    } elseif (-not $vs.installationPath) {
        $problems += New-Problem "The C++ tools in $($vs.displayName)" `
            "Open 'Visual Studio Installer', click Modify on $($vs.displayName), tick 'Desktop development with C++' and click Modify." `
            "https://learn.microsoft.com/cpp/build/vscpp-step-0-installation" ""
    } else {
        Write-Host "    Visual Studio: $($vs.displayName) ($($vs.installationVersion))"
        if ([int]("$($vs.installationVersion)".Split(".")[0]) -gt 17) {
            Write-Host "    NOTE: the port is tested with Visual Studio 2022; newer versions should work but are untested."
        }
        $probe = Get-VsProbe $Root
        $missing = @()
        if (-not $probe["cl"]) { $missing += "the MSVC compiler" }
        if (-not $probe["cmake"]) { $missing += "C++ CMake tools for Windows (CMake)" }
        if (-not $probe["ninja"]) { $missing += "C++ CMake tools for Windows (Ninja)" }
        if (-not $probe["sdk"]) { $missing += "a Windows 11 (or 10) SDK" }
        if ($missing) {
            $problems += New-Problem "Parts of the C++ workload: $(($missing | Select-Object -Unique) -join ', ')" `
                "Open 'Visual Studio Installer', click Modify on $($vs.displayName), and under 'Desktop development with C++' tick the missing items (they are ticked by default)." `
                "https://learn.microsoft.com/cpp/build/vscpp-step-0-installation" ""
        }
    }

    # Python 3.10 or newer
    $python = Find-Python
    if ($python) {
        Write-Host "    Python: $($python.Version) ($($python.Path))"
        $result.Python = $python.Path
    } else {
        $problems += New-Problem "Python 3.10 or newer" `
            "Install Python from python.org (tick 'Add python.exe to PATH' in the installer), then open a new PowerShell window." `
            "https://www.python.org/downloads/windows/" "winget install --id Python.Python.3.12"
    }

    # Git
    $git = Get-Command git -ErrorAction SilentlyContinue
    if ($git) {
        Write-Host "    Git: $((& git --version) -replace '^git version ', '')"
    } else {
        $problems += New-Problem "Git" "Install Git for Windows (the default options are fine), then open a new PowerShell window." `
            "https://git-scm.com/download/win" "winget install --id Git.Git"
    }

    # A Vulkan driver
    $vulkan = Join-Path $env:SystemRoot "System32\vulkan-1.dll"
    if (Test-Path $vulkan) {
        Write-Host "    Vulkan: $vulkan"
    } else {
        $problems += New-Problem "A graphics driver with Vulkan support (vulkan-1.dll)" `
            "Install the latest driver for your graphics card from AMD, NVIDIA or Intel. Any GPU from about 2016 on supports Vulkan." `
            "AMD: https://www.amd.com/en/support  NVIDIA: https://www.nvidia.com/drivers  Intel: https://www.intel.com/content/www/us/en/support/detect.html" ""
    }

    # Disk space
    $need = if ($FirstRun) { 5 } else { 1 }
    try {
        $drive = [System.IO.DriveInfo]::new([System.IO.Path]::GetPathRoot($Root))
        $free = [math]::Round($drive.AvailableFreeSpace / 1GB, 1)
        Write-Host "    Disk: $free GB free on $($drive.Name)"
        if ($free -lt $need) {
            $problems += New-Problem "About $need GB of free disk space on $($drive.Name) (has $free GB)" `
                "Free some space, or clone the repository onto a drive with more room." "" ""
        }
    } catch {
        Write-Host "    Disk: could not check the free space ($($_.Exception.Message))"
    }

    $result.Problems = $problems
    return $result
}

function Show-Problems($Problems) {
    Write-Host ""
    Write-Host "Setup cannot continue yet. Please install:" -ForegroundColor Yellow
    $index = 1
    foreach ($problem in $Problems) {
        Write-Host ""
        Write-Host "  $index. $($problem.What)" -ForegroundColor Yellow
        Write-Host "     $($problem.Fix)"
        if ($problem.Link) { Write-Host "     $($problem.Link)" }
        if ($problem.Winget) {
            Write-Host "     Or, in PowerShell:"
            Write-Host "       $($problem.Winget)"
        }
        $index++
    }
    Write-Host ""
    Write-Host "Then run setup again. Nothing was installed or changed. Details: docs/port/setup.md"
}
