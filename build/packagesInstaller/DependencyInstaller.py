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

print('Path to installing libraries')
print('(d)efault, (c)ustom, (q)uit?: ')

while True:
    if packagesInstaller.Getch.ch == 'q':
        packagesInstaller.NativeToolsError.finish(0)
    elif packagesInstaller.Getch.ch == 'c':
        libsDir = input("Enter path: ")

        if os.path.exists(libsDir) == False:
            print("Enter the correct existing path")
            continue

        print("Libs path: ", libsDir)

        break
    elif packagesInstaller.Getch.ch == 'd':
        break

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
            packagesInstaller.NativeToolsError.error('Nothing found: ' + pattern)
        for path in pathlist:
            if not os.path.exists(path):
                packagesInstaller.NativeToolsError.error('Not found: ' + path)
            items.append(computeFileHash(path))
        objects.append(':'.join(items))
    return hashlib.sha1(';'.join(objects).encode('utf-8')).hexdigest()

def keyPath(stage):
    return os.path.join(stage['directory'], keysLoc, stage['name'])

def checkCacheKey(stage):
    if not 'key' in stage:
        packagesInstaller.NativeToolsError.error('Key not set in stage: ' + stage['name'])
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
        packagesInstaller.NativeToolsError.error('Key not set in stage: ' + stage['name'])
    key = keyPath(stage)
    with open(key, 'w') as file:
        file.write(stage['key'])

stages = []


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
        packagesInstaller.NativeToolsError.error('Unknown location: ' + location)
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
            packagesInstaller.NativeToolsError.error('Bad command: ' + command)
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
        packagesInstaller.NativeToolsError.error('Bad command: ' + commands)
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
            packagesInstaller.NativeToolsError.error('Unknown argument: ' + arg)

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
        commands = packagesInstaller.SetupPaths.removeDir(stage['name']) + '\n' + stage['commands']

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
                    packagesInstaller.Getch.ch = 'r' if rebuildStale else packagesInstaller.Getch.getch()

                    if packagesInstaller.Getch.ch == 'q':
                        packagesInstaller.NativeToolsError.finish(0)

                    elif packagesInstaller.Getch.ch == 'p':
                        printCommands(commands)
                        checkResult = 'Printed'

                        break

                    elif packagesInstaller.Getch.ch == 's':
                        checkResult = 'Skip'
                        break

                    elif packagesInstaller.Getch.ch == 'r':
                        checkResult = 'Rebuild'
                        break

                    elif packagesInstaller.Getch.ch == 'a':
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
        packagesInstaller.NativeToolsError.finish(1)

    packagesInstaller.NativeToolsError.finish(0)


if win:
    currentCodePage = subprocess.run('chcp', capture_output=True, shell=True, text=True, env=modifiedEnv).stdout.strip().split()[-1]
    subprocess.run('chcp 65001 > nul', shell=True, env=modifiedEnv)
    runStages()
    subprocess.run('chcp ' + currentCodePage + ' > nul', shell=True, env=modifiedEnv)
else:
    runStages()
