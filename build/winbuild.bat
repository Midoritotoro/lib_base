@echo off
setlocal

set PYTHON_INSTALL_PATH="C:/Python313"

for %%d in (%~dp0.) do set Directory=%%~fd
echo Directory=%Directory%

for %%d in (%~dp0..) do set ParentDirectory=%%~fd
echo ParentDirectory=%ParentDirectory%

set CMAKE_BINARY_DIR="%ParentDirectory%\out\build"
set CMAKE_SOURCE_DIR="%ParentDirectory%"

@REM :pythonExistanceCheck
@REM   echo Checking for Python 3+ existance...
@REM   :: where python >nul 2>&1

@REM   :: if errorlevel 1 (
@REM   ::    goto errorNoPython
@REM   :: )

@REM   if not exist "%PYTHON_INSTALL_PATH%" (
@REM     goto errorNoPython
@REM   )

@REM   echo Python was found.
@REM   goto startPackagesInstallation

@REM :errorNoPython
@REM   echo.
@REM   echo Python is not installed.
  
@REM   echo Attempting to install python...
@REM   goto :tryToInstallPython

@REM :tryToInstallPython
@REM     PowerShell -NoProfile -ExecutionPolicy Bypass -File "%ParentDirectory%\build\powershell\InstallPython.ps1" -Verb RunAs
   
@REM     if exist "%PYTHON_INSTALL_PATH%" (
@REM         echo Python installation successful.
@REM         goto startPackagesInstallation
@REM         python -m "%ParentDirectory%\build\packagesInstaller" %*
@REM     )
    
@REM     echo Python installation failed. Stopping the build. 
@REM     goto eof

@REM :checkOptionsValidity
@REM     echo Checking validity of the given build options.
@REM     python ValidateBuildOptions.py

@REM :startPackagesInstallation
@REM     pushd ..
@REM     set upper_path=%CD%
@REM     popd

@REM     echo Installing Python packages...
@REM     PowerShell -NoProfile -ExecutionPolicy Bypass -File "%ParentDirectory%\build\powershell\InstallPackagesInstallerRequirements.ps1" -Verb Runas

:tryToBuildLibrary
    if not exist "%CMAKE_BINARY_DIR%" (
      echo Creating build directory: "%CMAKE_BINARY_DIR%"
      cmake -E make_directory "%CMAKE_BINARY_DIR%"
    )

    echo Configuring CMake project...
    pushd "%CMAKE_BINARY_DIR%"

      cmake -DCMAKE_BUILD_TYPE=Release -S "%CMAKE_SOURCE_DIR%" -B "%CMAKE_BINARY_DIR%"

      if errorlevel 1 (
          echo Error: CMake configuration failed.
          goto errorEnd
      )

      echo Building the project...
      echo "%BUILD_DEBUG_COMMAND%" "%CMAKE_SOURCE_DIR%"
      
      cmake --build "%CMAKE_BINARY_DIR%" --config Debug --parallel 
      cmake --build "%CMAKE_BINARY_DIR%" --config Release --parallel 

      if errorlevel 1 (
          echo Error: Project build failed.
          goto errorEnd
      )


    echo Project build successful.
    goto eof

    :errorEnd
        echo Error occured

:eof
popd
endlocal
pause
