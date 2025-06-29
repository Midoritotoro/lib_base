import sys
import os

import packagesInstaller.QtVersion as QtVersion

win = (sys.platform == 'win32')
mac = (sys.platform == 'darwin')

from packagesInstaller.NativeToolsError import nativeToolsError, error

if win and not 'Platform' in os.environ:
    nativeToolsError()
    
win32 = win and (os.environ['Platform'] == 'x86')
win64 = win and (os.environ['Platform'] == 'x64')
winarm = win and (os.environ['Platform'] == 'arm64')

if win and not 'COMSPEC' in os.environ:
    error('COMSPEC environment variable is not set.')

if win and not win32 and not win64 and not winarm:
    nativeToolsError()

arch = ''
if win32:
    arch = 'x86'
elif win64:
    arch = 'x64'
elif winarm:
    arch = 'arm'

if not QtVersion.resolveQtVersion(arch):
    error('Usupported platform.')