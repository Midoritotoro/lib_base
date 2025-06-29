from packagesInstaller.LibraryInstallationCommands import *
from packagesInstaller.LibraryInstallationInformation import LibraryInstallationInformation

class InstallExecutor: 
    def __init__(
        self:                       'InstallExecutor',
        installationInformation:    LibraryInstallationInformation
    ) -> None:
        self.__installationInformation: LibraryInstallationInformation = None
        self.__isLibrarySupported:      bool = False

        self.__initializeFromInformation(information=installationInformation)
        
    def __initializeFromInformation(
        self:           'InstallExecutor', 
        information:    LibraryInstallationInformation
    ) -> None: 
        self.__installationInformation = information
        self.__isLibrarySupported = isLibrarySupported(libraryName=information.libraryName)
        
        if self.__isLibrarySupported == False: 
            print(f"WARNING: Library {self.__installationInformation.libraryName} is not supported. ")

    def install(self: 'InstallExecutor') -> None:
        if self.__isLibrarySupported == False:
            return

    @property
    def installationInformation(self: 'InstallExecutor') -> LibraryInstallationInformation:
        return self.__installationInformation
    
    @installationInformation.setter
    def setInstallationInformation(
        self: 'InstallExecutor', 
        information: LibraryInstallationInformation
    ) -> None: 
        self.__installationInformation = information

