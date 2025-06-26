import sys

optionsList = [
    'qt6',
    'skip-release',
    'build-stackwalk',
]

options = []
runCommand = []

customRunCommand = False

for arg in sys.argv[1:]:
    if customRunCommand:
        runCommand.append(arg)
    if arg in optionsList:
        options.append(arg)
    elif arg == 'run':
        customRunCommand = True
    elif arg == 'shell':
        customRunCommand = True
        runCommand.append('shell')
