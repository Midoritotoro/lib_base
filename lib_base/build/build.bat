:: Check for Python Installation
:pythonExistanceCheck
	python --version 3>NUL
	if errorlevel 1 goto errorNoPython

	goto:eof

	:errorNoPython

	echo.
	echo Error^: Python not installed
	"C:\Program Files\used\systems\innoventiq\accumanager\required\excutables\python-3.7.3-amd64.exe"
goto:eof