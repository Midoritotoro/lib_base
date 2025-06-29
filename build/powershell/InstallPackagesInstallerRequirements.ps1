# Папка, где лежит скрипт
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path

# Путь к папке build (поднимаемся на 2 уровня вверх)
$buildDir = (Get-Item $scriptDir).Parent.Parent.FullName

# Путь к папке packagesInstaller
$packagesInstallerDir = Join-Path $buildDir "\build\packagesInstaller\"

# Пути к виртуальному окружению и скрипту активации
$venvPath = Join-Path $packagesInstallerDir "venv"
$activateScript = Join-Path $venvPath "\Scripts\Activate.ps1"

# Путь к Python
$pythonPath = "C:\Python313\python.exe"

Function Create-PythonVenv {
    Write-Host "Creating Python venv at $venvPath..."
    & $pythonPath -m venv $venvPath
    Write-Host "Activating Python venv..."
    & $activateScript
}

Function Python-InstallRequirements {
    # $requirementsPath = Join-Path $packagesInstallerDir "requirements.txt"
    # Write-Host "Installing requirements from $requirementsPath..."
    # & $pythonPath -m pip install -r $requirementsPath
}

Create-PythonVenv
Python-InstallRequirements