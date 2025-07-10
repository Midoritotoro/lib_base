from packagesInstaller.LibraryInstallationCommands import isLibrarySupported
from packagesInstaller.LibraryInstallationInformation import LibraryInstallationInformation

from packagesInstaller.SystemDetection import win
from packagesInstaller.BuildInstructionsParser import BuildInstructionsParser

from packagesInstaller.NativeToolsError import error, finish
from packagesInstaller.CacheManager import CacheManager

from packagesInstaller.Getch import getch
from packagesInstaller.SetupPaths import removeDir

from packagesInstaller.EnvironmentSetup import environmentConfiguration

import subprocess
import re

import os 

class InstallExecutor: 
    def __init__(
        self:                       'InstallExecutor',
        installationInformation:    LibraryInstallationInformation,
        silentInstallation:         bool = False
    ) -> None:
        self.__installationInformation: LibraryInstallationInformation = None
        self.__isLibrarySupported:      bool = False
        self.__silentInstallation:      bool = silentInstallation

        self.__initializeFromInformation(information=installationInformation)
        
    def __initializeFromInformation(
        self:           'InstallExecutor', 
        information:    LibraryInstallationInformation
    ) -> None: 
        self.__installationInformation = information
        self.__isLibrarySupported = isLibrarySupported(libraryName=information.libraryName)
        
        if self.__isLibrarySupported == False: 
            print(f"WARNING: Library {self.__installationInformation.libraryName} is not supported. ")
    
    def printInstallationCommands(self: 'InstallExecutor') -> None:
        print('---------------------------------COMMANDS-LIST----------------------------------')
        print(self.__installationInformation.installCommands, end='')
        print('--------------------------------------------------------------------------------')

    def install(
        self: 'InstallExecutor',
        queueLength: int,
        indexInQueue: int
    ) -> None:
        rebuildStale = False

        version = ('#' + str(self.__installationInformation.libraryVersion)) if (self.__installationInformation.libraryVersion != '0') else '' 
        prefix = '[' + str(indexInQueue) + '/' + str(queueLength) + '](' + self.__installationInformation.location + '/' + self.__installationInformation.libraryName + version + ')'

        print(prefix + ': ', end = '', flush=True)
        print(self.__installationInformation.libraryName)
        
        self.__installationInformation.cacheKey = CacheManager.ComputeCacheKey(self.__installationInformation)
        self.__installationInformation.installCommands = removeDir(
            os.path.join(self.__installationInformation.directory, self.__installationInformation.libraryName
            )) + '\n' + self.__installationInformation.installCommands
    
        checkResult = CacheManager.CheckCacheKey(self.__installationInformation)
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
                        if not self.__silentInstallation:
                            self.printInstallationCommands(self.__installationInformation.installCommands)
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

        CacheManager.ClearCacheKey(self.__installationInformation)
        print('BUILDING:')

        os.chdir(self.__installationInformation.directory)

        if not self.runCommands():
            print(prefix + ': FAILED')
            finish(1)

        CacheManager.WriteCacheKey(self.__installationInformation) 

    def runCommands(self: 'InstallExecutor') -> None | bool:
        if not self.__silentInstallation:
            self.printInstallationCommands()
            
            
        if win:
            if os.path.exists("command.bat"):
                os.remove("command.bat")

            with open("command.bat", 'w') as file:
                file.write('@echo OFF\r\n' + BuildInstructionsParser.winFailOnEach(self.__installationInformation.installCommands))

            result : bool = False

            if self.__silentInstallation:
                result = subprocess.run("command.bat", shell=True,
                    stdout=subprocess.PIPE,
                    env=environmentConfiguration.modifiedEnvironment).returncode == 0
            else:
                result = subprocess.run("command.bat", shell=True,
                    env=environmentConfiguration.modifiedEnvironment).returncode == 0

            if result and os.path.exists("command.bat"):
                os.remove("command.bat")
            return result
        
        elif re.search(r'\%', self.__installationInformation.installCommands):
            error('Bad command: ' + self.__installationInformation.installCommands)
        else:
            return subprocess.run(
                "set -e\n" + self.__installationInformation.installCommands, shell=True,
                env=environmentConfiguration.modifiedEnvironment).returncode == 0


    @property
    def installationInformation(self: 'InstallExecutor') -> LibraryInstallationInformation:
        return self.__installationInformation
    
    @installationInformation.setter
    def setInstallationInformation(
        self: 'InstallExecutor', 
        information: LibraryInstallationInformation
    ) -> None: 
        self.__installationInformation = information
