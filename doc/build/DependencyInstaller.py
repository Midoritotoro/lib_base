import os
import sys
import pprint
import re
import json
import pathlib
import hashlib
import subprocess
import glob
import tempfile


executePath = os.getcwd()
sys.dont_write_bytecode = True
scriptPath = os.path.dirname(os.path.realpath(__file__))
sys.path.append(scriptPath + '/..')


CMakeBuildOptions = []


def finish(code):
    global executePath
    os.chdir(executePath)
    sys.exit(code)

def error(text):
    print('[ERROR] ' + text)
    finish(1)

def nativeToolsError():
    error('Make sure to run from Native Tools Command Prompt.')

def checkCmakeBuildOptions(txt_file: str):
    with open(txt_file, 'r', encoding='utf-8') as options_file:
        line = options_file.read()
        CMakeBuildOptions.append(line.splitlines())

checkCmakeBuildOptions("CMakeBuildOptions.txt")
# print(CMakeBuildOptions)

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

qt = os.environ.get('QT')

if win and not 'COMSPEC' in os.environ:
    error('COMSPEC environment variable is not set.')

if win and not win32 and not win64 and not winarm:
    nativeToolsError()

os.chdir(scriptPath + '/../../../..')

pathSep = ';' if win else ':'
libsLoc = 'Libraries' if not win64 else (os.path.join('Libraries', 'win64'))
keysLoc = 'cache_keys'

rootDir = os.getcwd()
libsDir = os.path.realpath(os.path.join(rootDir, libsLoc))
usedPrefix = os.path.realpath(os.path.join(libsDir, 'local'))

class _Getch:
    """Gets a single character from standard input.  Does not echo to the screen."""
    def __init__(self):
        try:
            self.impl = _GetchWindows()
        except ImportError:
            self.impl = _GetchUnix()

    def __call__(self): return self.impl()

class _GetchUnix:
    def __init__(self):
        import tty, sys

    def __call__(self):
        import sys, tty, termios
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch

class _GetchWindows:
    def __init__(self):
        import msvcrt

    def __call__(self):
        import msvcrt
        return msvcrt.getch().decode('ascii')

getch = _Getch()


print('Path to installing libraries')
print('(d)efault, (c)ustom, (q)uit?: ')

ch = getch()

while True:
    if ch == 'q':
        finish(0)
    elif ch == 'c':
        libsDir = input("Enter path: ")
        if os.path.exists(libsDir) == False:
            print("Enter the correct existing path")
            continue
        print("Libs path: ", libsDir)
        break
    elif ch == 'd':
        break

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

environment = {
    'USED_PREFIX': usedPrefix,
    'ROOT_DIR': rootDir,
    'LIBS_DIR': libsDir,
    'PATH_PREFIX': pathPrefix,
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

ignoreInCacheForThirdParty = [
    'USED_PREFIX',
    'LIBS_DIR',
    'SPECIAL_TARGET',
    'X8664',
    'WIN32X64',
]

environmentKeyString = ''
envForThirdPartyKeyString = ''
for key in environment:
    part = key + '=' + environment[key] + ';'
    environmentKeyString += part
    if not key in ignoreInCacheForThirdParty:
        envForThirdPartyKeyString += part
environmentKey = hashlib.sha1(environmentKeyString.encode('utf-8')).hexdigest()
envForThirdPartyKey = hashlib.sha1(envForThirdPartyKeyString.encode('utf-8')).hexdigest()

modifiedEnv = os.environ.copy()
for key in environment:
    modifiedEnv[key] = environment[key]

modifiedEnv['PATH'] = environment['PATH_PREFIX'] + modifiedEnv['PATH']

def computeFileHash(path):
    sha1 = hashlib.sha1()
    with open(path, 'rb') as f:
        while True:
            data = f.read(256 * 1024)
            if not data:
                break
            sha1.update(data)
    return sha1.hexdigest()

def computeCacheKey(stage):
    if (stage['location'] == 'ThirdParty'):
        envKey = envForThirdPartyKey
    else:
        envKey = environmentKey
    objects = [
        envKey,
        stage['location'],
        stage['name'],
        stage['version'],
        stage['commands']
    ]
    for pattern in stage['dependencies']:
        pathlist = glob.glob(os.path.join(libsDir, pattern))
        items = [pattern]
        if len(pathlist) == 0:
            error('Nothing found: ' + pattern)
        for path in pathlist:
            if not os.path.exists(path):
                error('Not found: ' + path)
            items.append(computeFileHash(path))
        objects.append(':'.join(items))
    return hashlib.sha1(';'.join(objects).encode('utf-8')).hexdigest()

def keyPath(stage):
    return os.path.join(stage['directory'], keysLoc, stage['name'])

def checkCacheKey(stage):
    if not 'key' in stage:
        error('Key not set in stage: ' + stage['name'])
    key = keyPath(stage)
    if not os.path.exists(os.path.join(stage['directory'], stage['name'])):
        return 'NotFound'
    if not os.path.exists(key):
        return 'Stale'
    with open(key, 'r') as file:
        return 'Good' if (file.read() == stage['key']) else 'Stale'

def clearCacheKey(stage):
    key = keyPath(stage)
    if os.path.exists(key):
        os.remove(key)

def writeCacheKey(stage):
    if not 'key' in stage:
        error('Key not set in stage: ' + stage['name'])
    key = keyPath(stage)
    with open(key, 'w') as file:
        file.write(stage['key'])

stages = []

def removeDir(folder):
    if win:
        return 'if exist ' + folder + ' rmdir /Q /S ' + folder + '\nif exist ' + folder + ' exit /b 1'
    return 'rm -rf ' + folder

def setVar(key, multilineValue):
    singlelineValue = ' '.join(multilineValue.replace('\n', '').split());
    if win:
        return 'SET "' + key + '=' + singlelineValue + '"';
    return key + '="' + singlelineValue + '"';

def filterByPlatform(commands):
    commands = commands.split('\n')
    result = ''
    dependencies = []
    version = '0'
    skip = False
    for command in commands:
        m = re.match(r'(!?)([a-z0-9_]+):', command)
        if m and m.group(2) != 'depends' and m.group(2) != 'version':
            scopes = m.group(2).split('_')
            inscope = 'common' in scopes
            if win and 'win' in scopes:
                inscope = True
            if win32 and 'win32' in scopes:
                inscope = True
            if win64 and 'win64' in scopes:
                inscope = True
            if winarm and 'winarm' in scopes:
                inscope = True
            if mac and 'mac' in scopes:
                inscope = True
            if 'release' in scopes:
                if 'skip-release' in options:
                    inscope = False
                elif len(scopes) == 1:
                    continue
            skip = inscope if m.group(1) == '!' else not inscope
        elif not skip and not re.match(r'\s*#', command):
            if m and m.group(2) == 'version':
                version = version + '.' + command[len(m.group(0)):].strip()
            elif m and m.group(2) == 'depends':
                pattern = command[len(m.group(0)):].strip()
                dependencies.append(pattern)
            else:
                command = command.strip()
                if len(command) > 0:
                    result = result + command + '\n'
    return [result, dependencies, version]

def stage(name, commands, location = 'Libraries'):
    if location == 'Libraries':
        directory = libsDir
    else:
        error('Unknown location: ' + location)
    [commands, dependencies, version] = filterByPlatform(commands)
    if len(commands) > 0:
        stages.append({
            'name': name,
            'location': location,
            'directory': directory,
            'commands': commands,
            'version': version,
            'dependencies': dependencies
        })

def winFailOnEach(command):
    commands = command.split('\n')
    result = ''
    startingCommand = True
    for command in commands:
        command = re.sub(r'\$([A-Za-z0-9_]+)', r'%\1%', command)
        if re.search(r'\$[^<]', command):
            error('Bad command: ' + command)
        appendCall = startingCommand and not re.match(r'(if|for) ', command)
        called = 'call ' + command if appendCall else command
        result = result + called
        if command.endswith('^'):
            startingCommand = False
        else:
            startingCommand = True
            result = result + '\r\nif %errorlevel% neq 0 exit /b %errorlevel%\r\n'
    return result

def printCommands(commands):
    print('---------------------------------COMMANDS-LIST----------------------------------')
    print(commands, end='')
    print('--------------------------------------------------------------------------------')

def run(commands):
    printCommands(commands)
    if win:
        if os.path.exists("command.bat"):
            os.remove("command.bat")
        with open("command.bat", 'w') as file:
            file.write('@echo OFF\r\n' + winFailOnEach(commands))
        result = subprocess.run("command.bat", shell=True, env=modifiedEnv).returncode == 0
        if result and os.path.exists("command.bat"):
            os.remove("command.bat")
        return result
    elif re.search(r'\%', commands):
        error('Bad command: ' + commands)
    else:
        return subprocess.run("set -e\n" + commands, shell=True, env=modifiedEnv).returncode == 0

def runStages():
    onlyStages = []
    rebuildStale = False
    for arg in sys.argv[1:]:
        if arg in options:
            continue
        elif arg == 'silent':
            rebuildStale = True
            continue
        found = False
        for stage in stages:
            if stage['name'] == arg:
                onlyStages.append(arg)
                found = True
                break
        if not found:
            error('Unknown argument: ' + arg)
    count = len(stages)
    index = 0
    for stage in stages:
        if len(onlyStages) > 0 and not stage['name'] in onlyStages:
            continue
        index = index + 1
        version = ('#' + str(stage['version'])) if (stage['version'] != '0') else ''
        prefix = '[' + str(index) + '/' + str(count) + '](' + stage['location'] + '/' + stage['name'] + version + ')'
        print(prefix + ': ', end = '', flush=True)
        stage['key'] = computeCacheKey(stage)
        commands = removeDir(stage['name']) + '\n' + stage['commands']
        checkResult = 'Forced' if len(onlyStages) > 0 else checkCacheKey(stage)
        if checkResult == 'Good':
            print('SKIPPING')
            continue
        elif checkResult == 'NotFound':
            print('NOT FOUND, ', end='')
        elif checkResult == 'Stale' or checkResult == 'Forced':
            if checkResult == 'Stale':
                print('CHANGED, ', end='')
            if rebuildStale:
                checkResult == 'Rebuild'
            else:
                print('(r)ebuild, rebuild (a)ll, (s)kip, (p)rint, (q)uit?: ', end='', flush=True)
                while True:
                    ch = 'r' if rebuildStale else getch()
                    if ch == 'q':
                        finish(0)
                    elif ch == 'p':
                        printCommands(commands)
                        checkResult = 'Printed'
                        break
                    elif ch == 's':
                        checkResult = 'Skip'
                        break
                    elif ch == 'r':
                        checkResult = 'Rebuild'
                        break
                    elif ch == 'a':
                        checkResult = 'Rebuild'
                        rebuildStale = True
                        break
        if checkResult == 'Printed':
            continue
        if checkResult == 'Skip':
            print('SKIPPING')
            continue
        clearCacheKey(stage)
        print('BUILDING:')
        os.chdir(stage['directory'])
        if not run(commands):
            print(prefix + ': FAILED')
            finish(1)
        writeCacheKey(stage)

if customRunCommand:
    os.chdir(executePath)
    if len(runCommand) == 1 and runCommand[0] == 'shell':
        print('Preparing interactive mode..')
        if win:
            modifiedEnv['PROMPT'] = '(prepare) $P$G'
            subprocess.run("cmd.exe", shell=True, env=modifiedEnv)
        else:
            prompt = '(prepare) %~ %# '
            with tempfile.NamedTemporaryFile(mode='w', delete=False) as tmp_zshrc:
                tmp_zshrc.write(f'export PS1="{prompt}"\n')
                tmp_zshrc_path = tmp_zshrc.name
            subprocess.run(['zsh', '--rcs', tmp_zshrc_path], env=modifiedEnv)
            os.remove(tmp_zshrc_path)
    elif not run(' '.join(runCommand) + '\n'):
        print('FAILED :(')
        finish(1)
    finish(0)


stage('vcpkg', """
win:
    git clone https://github.com/Microsoft/vcpkg.git""")

# qt = '6.7'
# branch = '6.7.0' + ('-lts-lgpl' if qt < '6.3' else '')

# stage('qt_' + qt, """
#     git clone -b """ + branch + """ https://github.com/qt/qt5.git qt_6.7
#     cd qt_6.7
#     git submodule update --init --recursive qtbase qtimageformats qtsvg
#     cd qtbase
# mac:
#     find ../../patches/qtbase_6.7 -type f -print0 | sort -z | xargs -0 git apply -v
#     cd ..
#     sed -i.bak 's/tqtc-//' {qtimageformats,qtsvg}/dependencies.yaml

#     CONFIGURATIONS=-debug
# release:
#     CONFIGURATIONS=-debug-and-release
# mac:
#     ./configure -prefix "$USED_PREFIX/Qt-6.7" \
#         $CONFIGURATIONS \
#         -force-debug-info \
#         -opensource \
#         -confirm-license \
#         -static \
#         -opengl desktop \
#         -no-openssl \
#         -securetransport \
#         -system-webp \
#         -I "$USED_PREFIX/include" \
#         -no-feature-futimens \
#         -no-feature-brotli \
#         -nomake examples \
#         -nomake tests \
#         -platform macx-clang -- \
#         -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" \
#         -DCMAKE_PREFIX_PATH="$USED_PREFIX"

#     ninja
#     ninja install
# win:
#     for /r %%i in (..\\..\\patches\\qtbase_%QT%\\*) do git apply %%i -v
#     cd ..

#     SET CONFIGURATIONS=-debug
# release:
#     SET CONFIGURATIONS=-debug-and-release
# win:
#     """ + removeDir('"%LIBS_DIR%\\Qt' + qt + '"') + """
#     SET MOZJPEG_DIR=%LIBS_DIR%\\mozjpeg
#     SET OPENSSL_DIR=%LIBS_DIR%\\openssl3
#     SET OPENSSL_LIBS_DIR=%OPENSSL_DIR%\\out
#     SET ZLIB_LIBS_DIR=%LIBS_DIR%\\zlib
#     SET WEBP_DIR=%LIBS_DIR%\\libwebp
#     configure -prefix "%LIBS_DIR%\\Qt-%QT%" ^
#         %CONFIGURATIONS% ^
#         -force-debug-info ^
#         -opensource ^
#         -confirm-license ^
#         -static ^
#         -static-runtime ^
#         -feature-c++20 ^
#         -openssl linked ^
#         -system-webp ^
#         -system-zlib ^
#         -system-libjpeg ^
#         -nomake examples ^
#         -nomake tests ^
#         -platform win32-msvc ^
#         -D ZLIB_WINAPI ^
#         -- ^
#         -D OPENSSL_FOUND=1 ^
#         -D OPENSSL_INCLUDE_DIR="%OPENSSL_DIR%\\include" ^
#         -D LIB_EAY_DEBUG="%OPENSSL_LIBS_DIR%.dbg\\libcrypto.lib" ^
#         -D SSL_EAY_DEBUG="%OPENSSL_LIBS_DIR%.dbg\\libssl.lib" ^
#         -D LIB_EAY_RELEASE="%OPENSSL_LIBS_DIR%\\libcrypto.lib" ^
#         -D SSL_EAY_RELEASE="%OPENSSL_LIBS_DIR%\\libssl.lib" ^
#         -D JPEG_FOUND=1 ^
#         -D JPEG_INCLUDE_DIR="%MOZJPEG_DIR%" ^
#         -D JPEG_LIBRARY_DEBUG="%MOZJPEG_DIR%\\Debug\\jpeg-static.lib" ^
#         -D JPEG_LIBRARY_RELEASE="%MOZJPEG_DIR%\\Release\\jpeg-static.lib" ^
#         -D ZLIB_FOUND=1 ^
#         -D ZLIB_INCLUDE_DIR="%ZLIB_LIBS_DIR%" ^
#         -D ZLIB_LIBRARY_DEBUG="%ZLIB_LIBS_DIR%\\Debug\\zlibstaticd.lib" ^
#         -D ZLIB_LIBRARY_RELEASE="%ZLIB_LIBS_DIR%\\Release\\zlibstatic.lib" ^
#         -D WebP_INCLUDE_DIR="%WEBP_DIR%\\src" ^
#         -D WebP_demux_INCLUDE_DIR="%WEBP_DIR%\\src" ^
#         -D WebP_mux_INCLUDE_DIR="%WEBP_DIR%\\src" ^
#         -D WebP_LIBRARY="%WEBP_DIR%\\out\\release-static\\$X8664\\lib\\webp.lib" ^
#         -D WebP_demux_LIBRARY="%WEBP_DIR%\\out\\release-static\\$X8664\\lib\\webpdemux.lib" ^
#         -D WebP_mux_LIBRARY="%WEBP_DIR%\\out\\release-static\\$X8664\\lib\\webpmux.lib"
        
#     cmake --build . --config Debug --parallel
#     cmake --install . --config Debug
#     cmake --build . --parallel
#     cmake --install .
# """)

# stage('jom', """
# win:
#     powershell -Command "iwr -OutFile ./jom.zip https://qt-mirror.dannhauer.de/official_releases/jom/jom_1_1_3.zip"
#     powershell -Command "Expand-Archive ./jom.zip"
#     del jom.zip
# """, 'ThirdParty')

# stage('msys64', """
# win:
#     SET PATH_BACKUP_=%PATH%
#     SET PATH=%ROOT_DIR%\\ThirdParty\\msys64\\usr\\bin;%PATH%

#     SET CHERE_INVOKING=enabled_from_arguments
#     SET MSYS2_PATH_TYPE=inherit

#     powershell -Command "iwr -OutFile ./msys64.exe https://github.com/msys2/msys2-installer/releases/download/2024-05-07/msys2-base-x86_64-20240507.sfx.exe"
#     msys64.exe
#     del msys64.exe

#     bash -c "pacman-key --init; pacman-key --populate; pacman -Syu --noconfirm"
#     pacman -Syu --noconfirm mingw-w64-x86_64-perl mingw-w64-x86_64-nasm mingw-w64-x86_64-yasm mingw-w64-x86_64-ninja msys/make diffutils pkg-config

#     SET PATH=%PATH_BACKUP_%
# """, 'ThirdParty')

# stage('zlib', """
#     git clone https://github.com/madler/zlib.git
#     cd zlib
#     git checkout 643e17b749
# win:
#     cmake . ^
#         -A %WIN32X64% ^
#         -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreaded$<$<CONFIG:Debug>:Debug>" ^
#         -DCMAKE_C_FLAGS_DEBUG="/MTd /Zi /Ob0 /Od /RTC1" ^
#         -DCMAKE_C_FLAGS_RELEASE="/MT /O2 /Ob2 /DNDEBUG" ^
#         -DCMAKE_C_FLAGS="/DZLIB_WINAPI"
#     cmake --build . --config Debug --parallel
# release:
#     cmake --build . --config Release --parallel
# mac:
#     CFLAGS="$MIN_VER $UNGUARDED" LDFLAGS="$MIN_VER" ./configure \\
#         --static \\
#         --prefix=$USED_PREFIX \\
#         --archs="-arch x86_64 -arch arm64"
#     make $MAKE_THREADS_CNT
#     make install
# """)


stage('ms-gsl', f"""
win:
    cd vcpkg
    bootstrap-vcpkg.sh
    vcpkg integrate install
    vcpkg install ms-gsl
    cd packages
    move ms-gsl {libsDir}
""")

if win:
    currentCodePage = subprocess.run('chcp', capture_output=True, shell=True, text=True, env=modifiedEnv).stdout.strip().split()[-1]
    subprocess.run('chcp 65001 > nul', shell=True, env=modifiedEnv)
    runStages()
    subprocess.run('chcp ' + currentCodePage + ' > nul', shell=True, env=modifiedEnv)
else:
    runStages()
