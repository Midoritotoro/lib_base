from packagesInstaller.InstallExecutor import InstallExecutor
from packagesInstaller.LibraryInstallationInformation import LibraryInstallationInformation

from packagesInstaller.SetupPaths import libsDir
from packagesInstaller.NativeToolsError import error

from packagesInstaller.BuildInstructionsParser import BuildInstructionsParser
from packagesInstaller.EnvironmentSetup import environmentConfiguration

from packagesInstaller.SystemDetection import win
from packagesInstaller.YamlConfigLoader import YamlConfigLoader

from typing import Deque
from packagesInstaller.LibraryInstallationCommands import isLibrarySupported

import subprocess

class InstallationManager: 
    def __init__(self: 'InstallationManager') -> None:
        self.__executorsQueue: Deque[InstallExecutor] = []

    def addInstallation(
        self:       'InstallationManager',
        name:       str, 
        location:   str = 'Libraries'
    ) -> None:
        if not isLibrarySupported(name):
            return
        
        if location == 'Libraries':
            directory = libsDir
        else:
            error('Unknown location: ' + location)

        libraryInstallationInformation: LibraryInstallationInformation = YamlConfigLoader.ExtractLibraryInformationFromYaml(name)

        [commands, dependencies, version] = BuildInstructionsParser.FilterInstallationCommandsByPlatform(
            libraryInstallationInformation.installationCommands)

        if len(commands) > 0:
            self.__executorsQueue.append(
                InstallExecutor(
                    LibraryInstallationInformation(
                        libraryName=name, libraryInformation=libraryInstallationInformation.libraryInformation, libraryVersion=version, 
                        installCommands=commands, location=location,
                        directory=directory, dependencies=dependencies
                    )
                )
            )

    def executeAll(self: 'InstallationManager') -> None:
        if win:
            currentCodePage = subprocess.run('chcp',  
                capture_output=True, shell=True, text=True,
                env=environmentConfiguration.modifiedEnvironment).stdout.strip().split()[-1]
        
            subprocess.run('chcp 65001 > nul', shell=True, env=environmentConfiguration.modifiedEnvironment)
            self.__runStages()

            subprocess.run('chcp ' + currentCodePage + ' > nul', shell=True, env=environmentConfiguration.modifiedEnvironment)
        else:
            self.__runStages()

    def __runStages(self: 'InstallationManager') -> None:
        count = len(self.__executorsQueue)
        index = 0

        for executor in self.__executorsQueue:
            index = index + 1
            executor.install(queueLength=count, indexInQueue=index)
