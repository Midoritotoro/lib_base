$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path

# Path to build folder
$buildDir = (Get-Item $scriptDir).Parent.Parent.FullName

# Path to packagesInstaller folder
$packagesInstallerDir = Join-Path $buildDir "\build\packagesInstaller\"

# Paths to virtual environment and activation script
$venvPath = Join-Path $packagesInstallerDir "venv"
$activateScript = Join-Path $venvPath "\Scripts\Activate.bat"

Function Create-PythonVenv {
    Write-Host "Creating Python venv at $venvPath..."
    python -m venv $venvPath
    Write-Host "Activating Python venv..."
    & $activateScript
}

Function Python-InstallRequirements {
    $requirementsPath = Join-Path $buildDir "build\requirements.txt"
    Write-Host "Installing requirements from $requirementsPath..."
    python -m pip install -r $requirementsPath
}

Create-PythonVenv
Python-InstallRequirements