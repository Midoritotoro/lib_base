@echo off
setlocal

set CMAKE_BUILD_OPTIONS_FILE="CMakeBuildOptions.txt"
set CMAKE_BINARY_DIR="..\out\build"
set CMAKE_SOURCE_DIR="..\"
set CMAKE_COMMAND="cmake"
set BUILD_COMMAND="cmake --build"


type nul > %CMAKE_BUILD_OPTIONS_FILE%

:processArgs
    if "%1"=="" (
        goto pythonExistanceCheck
    )

    echo %1 >> %CMAKE_BUILD_OPTIONS_FILE%

    shift
    goto processArgs

:pythonExistanceCheck
  echo Checking for Python 3+ existance...
  where python >nul 2>&1

  if errorlevel 1 (
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
    PowerShell -Command "& {Start-Process PowerShell -ArgumentList '-NoProfile -ExecutionPolicy Bypass -File ""powershell/pythonInstall.ps1""' -Verb RunAs}"

    where python >nul 2>&1

    if errorlevel 0 (
        echo Python installation successful.
        goto startPackagesInstallation
    ) 
    
    echo Python installation failed. Stopping the build. 
    goto eof

:checkOptionsValidity
    echo Checking validity of the given build options.
    python ValidateBuildOptions.py

:startPackagesInstallation
    echo Installing packages...
    python DependencyInstaller.py

:tryToBuildLibrary
    if not exist "%CMAKE_BUILD_OPTIONS_FILE%" (
        echo Error: CMake options file "%CMAKE_BUILD_OPTIONS_FILE%" not found.
        goto errorEnd
    )

    if not exist "%CMAKE_BINARY_DIR%" (
      echo Creating build directory: "%CMAKE_BINARY_DIR%"
      mkdir "%CMAKE_BINARY_DIR%"
    )

    echo Configuring CMake project...
    pushd "%CMAKE_BINARY_DIR%"

    set CMAKE_OPTIONS=""
    for /f "delims=" %%a in (%CMAKE_BUILD_OPTIONS_FILE%) do (
      set CMAKE_OPTIONS=%CMAKE_OPTIONS% -D%%a
    )
  
    echo Using CMAKE OPTIONS: %CMAKE_OPTIONS%
    "%CMAKE_COMMAND%" %CMAKE_OPTIONS% "%CMAKE_SOURCE_DIR%"

      if errorlevel 1 (
          echo Error: CMake configuration failed.
          goto errorEnd
      )

   :: echo Building the project...
     :: "%BUILD_COMMAND%" .

    ::  if errorlevel 1 (
  ::          echo Error: Project build failed.
::            goto errorEnd
    ::  )


    echo Project build successful.
    goto eof

    :errorEnd
        echo Error occured

:eof
popd
endlocal
pause
