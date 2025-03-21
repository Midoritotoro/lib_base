@echo off
setlocal

set CMAKE_BUILD_OPTIONS_FILE="CMakeBuildOptions.txt"
set CMAKE_BINARY_DIR="build"
set CMAKE_SOURCE_DIR="."
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
  echo Checking for Python 3.11 existance...
  python --version >nul 2>&1
  if errorlevel 1 (
    goto errorNoPython
  )

  echo Python was found.
  goto startPackagesInstallation

:errorNoPython
  echo.
  echo Error: Python not installed.
  
  echo Attempting to install python...
  goto :tryToInstallPython


:tryToInstallPython
     Invoke-WebRequest  -UseBasicParsing -Uri  \
    'https://www.python.org/ftp/python/3.11.0/python-3.11.0-amd64.exe' \
    -OutFile 'C:\ProgramFiles\python-3.11.0-amd64.exe' 

    .\python-3.11.0-amd64.exe /passive /quiet InstallAllUsers=1 PrependPath=1 Include_test=0

    setx /M path "%path%;C:\Program Files\Python311\" 
    $env:PATH =$env:PATH+";C:\Program Files\Python311\"

    if %errorlevel% equ 0 (
        echo Python installation successful.
        goto startPackagesInstallation
    ) 
    else (
        echo Python installation failed. Stopping the build. 
        goto eof
    )

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

::   echo Configuring CMake project...
::    pushd "%CMAKE_BINARY_DIR%"

::    set CMAKE_OPTIONS=""
::    for /f "delims=" %%a in (%CMAKE_BUILD_OPTIONS_FILE%) do (
::      set CMAKE_OPTIONS=%CMAKE_OPTIONS% -D%%a
::      )
  
::      echo Using CMAKE OPTIONS: %CMAKE_OPTIONS%
::      "%CMAKE_COMMAND%" %CMAKE_OPTIONS% "%CMAKE_SOURCE_DIR%"

      ::if errorlevel 1 (
    ::        echo Error: CMake configuration failed.
  ::          goto errorEnd
::      )

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
