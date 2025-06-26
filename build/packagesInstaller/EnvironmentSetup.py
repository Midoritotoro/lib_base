from packagesInstaller.NativeToolsError import *
import packagesInstaller.SetupPaths

win = (sys.platform == 'win32')
mac = (sys.platform == 'darwin')

if win and not 'Platform' in os.environ:
    nativeToolsError()

win32 = win and (os.environ['Platform'] == 'x86')
win64 = win and (os.environ['Platform'] == 'x64')
winarm = win and (os.environ['Platform'] == 'arm64')

arch = ''
if win32:
    arch = 'x86'
elif win64:
    arch = 'x64'
elif winarm:
    arch = 'arm'

if win and not 'COMSPEC' in os.environ:
    error('COMSPEC environment variable is not set.')

if win and not win32 and not win64 and not winarm:
    nativeToolsError()

qt = os.environ.get('QT')

environment = {
    'USED_PREFIX': packagesInstaller.SetupPaths.usedPrefix,
    'ROOT_DIR': packagesInstaller.SetupPaths.rootDir,
    'LIBS_DIR': packagesInstaller.SetupPaths.libsDir,
    'PATH_PREFIX': packagesInstaller.SetupPaths.pathPrefix,
}
if (win32):
    environment.update({
        'SPECIAL_TARGET': 'win',
        'X8664': 'x86',
        'WIN32X64': 'Win32',
    })
elif (win64):
    environment.update({
        'SPECIAL_TARGET': 'win64',
        'X8664': 'x64',
        'WIN32X64': 'x64',
    })
elif (winarm):
    environment.update({
        'SPECIAL_TARGET': 'winarm',
        'X8664': 'ARM64',
        'WIN32X64': 'ARM64',
    })
elif (mac):
    environment.update({
        'SPECIAL_TARGET': 'mac',
        'MAKE_THREADS_CNT': '-j' + str(os.cpu_count()),
        'MACOSX_DEPLOYMENT_TARGET': '10.13',
        'UNGUARDED': '-Werror=unguarded-availability-new',
        'MIN_VER': '-mmacosx-version-min=10.13',
    })
