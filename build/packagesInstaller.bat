@echo off
setlocal

for %%d in (%~dp0.) do set Directory=%%~fd
echo Directory=%Directory%

for %%d in (%~dp0..) do set ParentDirectory=%%~fd
echo ParentDirectory=%ParentDirectory%

:callInstaller
    PowerShell -NoProfile -ExecutionPolicy Bypass -File "%ParentDirectory%\build\powershell\InstallPackagesInstallerRequirements.ps1" -Verb Runas
    python -m packagesInstaller %*

:eof
popd
endlocal
pause