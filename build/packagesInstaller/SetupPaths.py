from packagesInstaller.SystemDetection import win, win64

from packagesInstaller.NativeToolsError import finish, executePath
from packagesInstaller.Getch import getch

import pathlib

import os
import sys


sys.dont_write_bytecode = True
scriptPath = os.path.dirname(os.path.realpath(__file__))

sys.path.append(scriptPath + '/..')
os.chdir(scriptPath + '/../../../..')

pathSep = ';' if win else ':'
libsLoc = 'Libraries' if not win64 else (os.path.join('Libraries', 'win64'))
keysLoc = 'cache_keys'

rootDir = os.getcwd()
libsDir = os.path.realpath(os.path.join(rootDir, libsLoc))

usedPrefix = os.path.realpath(os.path.join(libsDir, 'local'))

if not os.path.isdir(os.path.join(libsDir, keysLoc)):
    pathlib.Path(os.path.join(libsDir, keysLoc)).mkdir(parents=True, exist_ok=True)

pathPrefixes = [
    'ThirdParty\\msys64\\mingw64\\bin',
    'ThirdParty\\jom',
    'ThirdParty\\cmake\\bin',
    'ThirdParty\\gyp',
] if win else [
    'ThirdParty/gyp',
    'ThirdParty/yasm',
    'ThirdParty/depot_tools',
]
pathPrefix = ''
for singlePrefix in pathPrefixes:
    pathPrefix = pathPrefix + os.path.join(rootDir, singlePrefix) + pathSep

def removeDir(folder: str):
    if win:
        return 'if exist ' + folder + ' rmdir /Q /S ' + folder + '\nif exist ' + folder + ' exit /b 1'
    return 'rm -rf ' + folder

# def setupInstallingPath():
#     print('Path to installing libraries')
#     print('(d)efault, (c)ustom, (q)uit?: ')

#     while True:
#         if getch() == 'q':
#             finish(0)
#         elif getch() == 'c':
#             libsDir = input("Enter path: ")

#             if os.path.exists(libsDir) == False:
#                 print("Enter the correct existing path")
#                 continue

#             print("Libs path: ", libsDir)

#             break
#         elif getch() == 'd':
#             break
        
# setupInstallingPath()
