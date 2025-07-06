from packagesInstaller.LibraryInstallationCommands import isLibrarySupported
from packagesInstaller.LibraryInstallationInformation import LibraryInstallationInformation

from packagesInstaller.SystemDetection import win
from packagesInstaller.BuildInstructionsParser import BuildInstructionsParser

from packagesInstaller.NativeToolsError import error, finish
from packagesInstaller.CacheManager import CacheManager

from packagesInstaller.Getch import getch
from packagesInstaller.SetupPaths import removeDir

from packagesInstaller.EnvironmentSetup import environmentConfiguration
from packagesInstaller.Dependency import Dependency

from packagesInstaller.YamlConfigLoader import YamlConfigLoader

import subprocess
import re

import os 

class InstallExecutor: 
    def __init__(
        self:                       'InstallExecutor',
        installationInformation:    LibraryInstallationInformation,
        silentInstallation:         bool = False
    ) -> None:
        self.installationInformation: LibraryInstallationInformation = None
        self.isLibrarySupported:      bool = False
        self.silentInstallation:      bool = silentInstallation

        self.__initializeFromInformation(information=installationInformation)
        
    def __initializeFromInformation(
        self:           'InstallExecutor', 
        information:    LibraryInstallationInformation
    ) -> None: 
        self.installationInformation = information
        self.isLibrarySupported = isLibrarySupported(libraryName=information.libraryName)
        
        if self.isLibrarySupported == False: 
            print(f"WARNING: Library {self.installationInformation.libraryName} is not supported. ")
    
    def printInstallationCommands(
        self: 'InstallExecutor',
        installationInformation: LibraryInstallationInformation
    ) -> None:
        print('---------------------------------COMMANDS-LIST----------------------------------')
        print(installationInformation.installCommands, end='')
        print('--------------------------------------------------------------------------------')

    def __installDependencies(
        self: 'InstallExecutor',
        installationInformation: LibraryInstallationInformation
    ) -> bool:
        for dependency in installationInformation.dependencies:
            information: LibraryInstallationInformation = YamlConfigLoader.ExtractLibraryInformationFromYaml(dependency.dependencyName)
            commands: str = BuildInstructionsParser.FilterInstallationCommandsByPlatform(
                information.installCommands).replace(
                "LIB_BASE_INSTALLATION_DIRECTORY", installationInformation.directory)
            
            information.installCommands = commands 

            if not self.__installLibrary(information):
                return False
            
        return True

    def __installLibrary(
        self:                       'InstallExecutor',
        installationInformation:    LibraryInstallationInformation
    ) -> bool: 
        rebuildStale = False

        installationInformation.cacheKey = CacheManager.ComputeCacheKey(installationInformation)
        installationInformation.installCommands = removeDir(
            os.path.join(installationInformation.directory, installationInformation.libraryName
        )) + '\n' + installationInformation.installCommands
    
        checkResult = CacheManager.CheckCacheKey(installationInformation)
        if checkResult == CacheManager.CacheKeyState.Good:
            print('SKIPPING')
            return

        elif checkResult == CacheManager.CacheKeyState.NotFound:
            print('NOT FOUND, ', end='')

        elif checkResult == CacheManager.CacheKeyState.Stale:
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
                        if not self.silentInstallation:
                            self.printInstallationCommands(installationInformation.installCommands)
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
            return

        if checkResult == 'Skip':
            print('SKIPPING')
            return

        CacheManager.ClearCacheKey(installationInformation)
        print('BUILDING:')

        os.chdir(installationInformation.directory)

        if not self.runCommands():
            return False

        CacheManager.WriteCacheKey(installationInformation) 


    def install(
        self: 'InstallExecutor',
        queueLength: int,
        indexInQueue: int
    ) -> None:
        prefix = '[' + str(indexInQueue) + '/' + str(queueLength) + '](' + self.installationInformation.location + '/' + self.installationInformation.libraryName + version + ')'

        print(prefix + ': ', end = '', flush=True)
        print(self.installationInformation.libraryName)

        if not self.__installLibrary(self.installationInformation):
            print(prefix + ': FAILED')
            finish(1)

        
    def runCommands(
        self: 'InstallExecutor',
        inslallationInformation: LibraryInstallationInformation
    ) -> None | bool:
        self.__installDependencies(inslallationInformation)

        if not self.silentInstallation:
            self.printInstallationCommands()
            
        if win:
            if os.path.exists("command.bat"):
                os.remove("command.bat")

            with open("command.bat", 'w') as file:
                file.write('@echo OFF\r\n' + BuildInstructionsParser.winFailOnEach(self.installationInformation.installCommands))

            result : bool = False

            if self.silentInstallation:
                result = subprocess.run("command.bat", shell=True,
                    stdout=subprocess.PIPE,
                    env=environmentConfiguration.modifiedEnvironment).returncode == 0
            else:
                result = subprocess.run("command.bat", shell=True,
                    env=environmentConfiguration.modifiedEnvironment).returncode == 0

            if result and os.path.exists("command.bat"):
                os.remove("command.bat")
            return result
        
        elif re.search(r'\%', self.installationInformation.installCommands):
            error('Bad command: ' + self.installationInformation.installCommands)
        else:
            return subprocess.run(
                "set -e\n" + self.installationInformation.installCommands, shell=True,
                env=environmentConfiguration.modifiedEnvironment).returncode == 0