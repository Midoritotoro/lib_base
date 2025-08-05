@echo off
setlocal

set PYTHON_INSTALL_PATH="C:/Python313"

for %%d in (%~dp0.) do set Directory=%%~fd
echo Directory=%Directory%

for %%d in (%~dp0..) do set ParentDirectory=%%~fd
echo ParentDirectory=%ParentDirectory%

set CMAKE_BINARY_DIR="%ParentDirectory%\out\build"
set CMAKE_SOURCE_DIR="%ParentDirectory%"

 :pythonExistanceCheck
   echo Checking for Python 3+ existance...
   :: where python >nul 2>&1

   :: if errorlevel 1 (
   ::    goto errorNoPython
   :: )

   if not exist "%PYTHON_INSTALL_PATH%" (
     goto errorNoPython
   )

   echo Python was found.
   goto startPackagesInstallation

 :errorNoPython
   echo.
   echo Python is not installed.
  
   echo Attempting to install python...
   goto :tryToInstallPython

 :tryToInstallPython
     PowerShell -NoProfile -ExecutionPolicy Bypass -File "%ParentDirectory%\build\powershell\InstallPython.ps1" -Verb RunAs
   
     if exist "%PYTHON_INSTALL_PATH%" (
         echo Python installation successful.
         goto startPackagesInstallation
         python -m "%ParentDirectory%\build\packagesInstaller" %*
     )
    
     echo Python installation failed. Stopping the build. 
     goto eof

 :checkOptionsValidity
     echo Checking validity of the given build options.
     python ValidateBuildOptions.py

 :startPackagesInstallation
     pushd ..
     set upper_path=%CD%
     popd

     echo Installing Python packages...
     PowerShell -NoProfile -ExecutionPolicy Bypass -File "%ParentDirectory%\build\powershell\InstallPackagesInstallerRequirements.ps1" -Verb Runas

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
