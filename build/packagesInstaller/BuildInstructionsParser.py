from packagesInstaller.SystemDetection import win, win32, win64, winarm, mac
from packagesInstaller.InstallOptionsCheck import options

from packagesInstaller.NativeToolsError import error

from typing import List, Any
import re

class BuildInstructionsParser:
    @staticmethod
    def FilterInstallationCommandsByPlatform(commands: str) -> list:
        commands: List[str] = commands.split('\n')

        result: str = ''
        dependencies: List[str] = []

        version: str | Any = '0'
        skip: bool = False

        for command in commands:
            match = re.match(r'(!?)([a-z0-9_]+):', command)
            if match and match.group(2) != 'depends' and match.group(2) != 'version':

                scopes: List[str] | Any = match.group(2).split('_')
                inscope : bool = 'common' in scopes

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

                skip = inscope if match.group(1) == '!' else not inscope

            elif not skip and not re.match(r'\s*#', command):
                if match and match.group(2) == 'version':
                    version = version + '.' + command[len(match.group(0)):].strip()

                elif match and match.group(2) == 'depends':
                    pattern = command[len(match.group(0)):].strip()
                    dependencies.append(pattern)

                else:
                    command = command.strip()
                    if len(command) > 0:
                        result = result + command + '\n'

        return [result, dependencies, version]
    
    @staticmethod
    def winFailOnEach(command: str) -> str:
        commands: List[str] = command.split('\n')
        result: str         = ""

        startingCommand: bool = True

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