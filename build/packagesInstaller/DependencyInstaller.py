import os
import sys
import re
import pathlib
import hashlib
import subprocess
import glob
import tempfile

import packagesInstaller.EnvironmentSetup
import packagesInstaller.Getch
import packagesInstaller.NativeToolsError

import packagesInstaller.SetupPaths

CMakeBuildOptions = []

def checkCmakeBuildOptions(txt_file: str):
    with open(txt_file, 'r', encoding='utf-8') as options_file:
        line = options_file.read()
        CMakeBuildOptions.append(line.splitlines())

checkCmakeBuildOptions("CMakeBuildOptions.txt")
# print(CMakeBuildOptions)


# if win:
#     currentCodePage = subprocess.run('chcp', capture_output=True, shell=True, text=True, env=modifiedEnv).stdout.strip().split()[-1]
#     subprocess.run('chcp 65001 > nul', shell=True, env=modifiedEnv)
#     runStages()
#     subprocess.run('chcp ' + currentCodePage + ' > nul', shell=True, env=modifiedEnv)
# else:
#     runStages()
