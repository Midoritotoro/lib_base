from packagesInstaller.InstallExecutor import InstallExecutor
from packagesInstaller.LibraryInstallationInformation import LibraryInstallationInformation

from packagesInstaller.EnvironmentSetup import *
from packagesInstaller.InstallOptionsCheck import options

from packagesInstaller.SetupPaths import keysLoc, libsDir
from packagesInstaller.NativeToolsError import error, finish

from packagesInstaller.BuildInstructionsParser import BuildInstructionsParser
from packagesInstaller.EnvironmentSetup import environmentConfiguration

from packagesInstaller.SystemDetection import win
from packagesInstaller.CacheManager import CacheManager

from packagesInstaller.Getch import getch, ch

from typing import Deque, List
from collections import deque

import re
import subprocess

import sys

class InstallationManager: 
    def __init__(self: 'InstallationManager') -> None:
        self.__executorsQueue: Deque[InstallExecutor] = []

    def addInstallation(
        self:       'InstallationManager',
        executor:   InstallExecutor
    ) -> None:
        self.__executorsQueue.append(executor)

    def stage(
        self: 'InstallationManager',
        name: str, 
        commands: str, 
        location: str = 'Libraries'
    ) -> None:
        if location == 'Libraries':
            directory = libsDir
        else:
            error('Unknown location: ' + location)

        [commands, dependencies, version] = BuildInstructionsParser.FilterInstallationCommandsByPlatform(commands)

        if len(commands) > 0:
            self.__executorsQueue.append(
                InstallExecutor(
                    LibraryInstallationInformation(
                        libraryName=name, libraryInformation="", libraryVersion=version, 
                        installCommands=commands, location=location,
                        directory=directory, dependencies=dependencies
                    )
                )
            )

    def printCommands(
        self: 'InstallationManager',
        commands: str
    ) -> None:
        print('---------------------------------COMMANDS-LIST----------------------------------')
        print(commands, end='')
        print('--------------------------------------------------------------------------------')

    def run(
        self: 'InstallationManager',
        commands: str
    ) -> None | bool:
        self.printCommands(commands)

        if win:
            if os.path.exists("command.bat"):
                os.remove("command.bat")

            with open("command.bat", 'w') as file:
                file.write('@echo OFF\r\n' + BuildInstructionsParser.winFailOnEach(commands))
            result = subprocess.run("command.bat", shell=True, env=environmentConfiguration.modifiedEnvironment).returncode == 0

            if result and os.path.exists("command.bat"):
                os.remove("command.bat")
            return result
        
        elif re.search(r'\%', commands):
            error('Bad command: ' + commands)
        else:
            return subprocess.run("set -e\n" + commands, shell=True, env=environmentConfiguration.modifiedEnvironment).returncode == 0

    def runStages(self: 'InstallationManager') -> None:
        onlyStages = []
        rebuildStale = False

        for arg in sys.argv[1:]:
            if arg in options:
                continue

            elif arg == 'silent':
                rebuildStale = True
                continue

            found = False

            for stage in self.__executorsQueue:
                if stage.installationInformation.libraryName == arg:
                    onlyStages.append(arg)
                    found = True

                    break

            if not found:
                error('Unknown argument: ' + arg)

        count = len(self.__executorsQueue)
        index = 0

        for stage in self.__executorsQueue:
            if len(onlyStages) > 0 and not stage['name'] in onlyStages:
                continue

            index = index + 1

            version = ('#' + str(stage['version'])) if (stage['version'] != '0') else '' 
            prefix = '[' + str(index) + '/' + str(count) + '](' + stage['location'] + '/' + stage['name'] + version + ')'

            print(prefix + ': ', end = '', flush=True)

            stage.installationInformation.set = CacheManager.ComputeCacheKey(stage)
            commands = packagesInstaller.SetupPaths.removeDir(stage['name']) + '\n' + stage['commands']

            checkResult = 'Forced' if len(onlyStages) > 0 else CacheManager.CheckCacheKey(stage)

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
                            packagesInstaller.NativeToolsError.finish(0)

                        elif ch == 'p':
                            self.printCommands(commands)
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

            CacheManager.ClearCacheKey(stage)
            print('BUILDING:')

            os.chdir(stage['directory'])

            if not self.run(commands):
                print(prefix + ': FAILED')
                finish(1)

            CacheManager.WriteCacheKey(stage)


    def executeAll(self: 'InstallationManager') -> None:
        if win:
            currentCodePage = subprocess.run('chcp',  
                capture_output=True, shell=True, text=True,
                env=environmentConfiguration.modifiedEnvironment).stdout.strip().split()[-1]
        
            subprocess.run('chcp 65001 > nul', shell=True, env=environmentConfiguration.modifiedEnvironment)
            self.runStages()

            subprocess.run('chcp ' + currentCodePage + ' > nul', shell=True, env=environmentConfiguration.modifiedEnvironment)
        else:
            self.runStages()
