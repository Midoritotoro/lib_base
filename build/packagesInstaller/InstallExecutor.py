from packagesInstaller.LibraryInstallationCommands import *

class InstallExecutor: 
    def __init__(
        self: 'InstallExecutor',
        libraryName: str
    ) -> None:
        self.__libraryName: str         = libraryName
        self.__isLibrarySupported: bool = isLibrarySupported(libraryName=libraryName)
        
        if self.__isLibrarySupported == False: 
            print(f"WARNING: Library {self.__libraryName} is not supported. ")

    def install(self: 'InstallExecutor') -> None:
        if self.__isLibrarySupported == False:
            return 
