import sys

optionsList = [
    'qt6',
    'skip-release',
    'build-stackwalk',
]

options = []
runCommand = []

for arg in sys.argv[1:]:
    if arg in optionsList:
        options.append(arg)
