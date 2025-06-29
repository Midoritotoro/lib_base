from typing import List

class LibraryInstallationInformation: 
    def __init__(
        self:               'LibraryInstallationInformation',
        libraryName:        str, 
        libraryInformation: str         = "", 
        libraryVersion:     str         = "",
        installCommands:    str         = "",
        location:           str         = "",
        directory:          str         = "",
        cacheKey:           str         = "",
        dependencies:       List[str]   = []
    ):
        self.__libraryName:         str         = libraryName
        self.__libraryInformation:  str         = libraryInformation 
        self.__libraryVersion:      str         = libraryVersion
        self.__installCommands:     str         = installCommands
        self.__location:            str         = location
        self.__directory:           str         = directory
        self.__cacheKey:            str         = cacheKey
        self.__dependencies:        List[str]   = dependencies

    @property
    def installationCommands(self: 'LibraryInstallationInformation') -> str: 
        return self.__installCommands
    
    @installationCommands.setter
    def setInstallationCommands(
        self:               'LibraryInstallationInformation',
        installCommands:    str
    ) -> None:
        self.__installCommands = installCommands

    @property
    def location(self: 'LibraryInstallationInformation') -> str: 
        return self.__location
    
    @location.setter
    def setLocation(
        self:       'LibraryInstallationInformation',
        location:   str
    ) -> None:
        self.__location = location

    @property
    def directory(self: 'LibraryInstallationInformation') -> str: 
        return self.__directory
    
    @directory.setter
    def setDirectory(
        self:       'LibraryInstallationInformation',
        directory:  str
    ) -> None:
        self.__directory = directory


    @property
    def libraryInformation(self: 'LibraryInstallationInformation') -> str: 
        return self.__libraryInformation
    
    @libraryInformation.setter
    def setLibraryInformation(
        self:           'LibraryInstallationInformation',
        libraryInfo:    str
    ) -> None:
        self.__libraryInformation = libraryInfo


    @property
    def libraryName(self: 'LibraryInstallationInformation') -> str: 
        return self.__libraryName
    
    @libraryName.setter
    def setLibraryName(
        self:           'LibraryInstallationInformation',
        libraryName:    str
    ) -> None:
        self.__libraryName = libraryName

    @property
    def dependencies(self: 'LibraryInstallationInformation') -> List[str]: 
        return self.__dependencies
    
    @dependencies.setter
    def setDependencies(
        self:           'LibraryInstallationInformation',
        dependencies:   List[str]
    ) -> None:
        self.__dependencies = dependencies

    @property
    def libraryVersion(self: 'LibraryInstallationInformation') -> str: 
        return self.__libraryVersion
    
    @libraryVersion.setter
    def setLibraryVersion(
        self:           'LibraryInstallationInformation',
        libraryVersion: str
    ) -> None:
        self.__libraryVersion = libraryVersion

    @property
    def cacheKey(self: 'LibraryInstallationInformation') -> str: 
        return self.__cacheKey
    
    @cacheKey.setter
    def setCacheKey(
        self:           'LibraryInstallationInformation',
        cacheKey:       str
    ) -> None:
        self.__cacheKey = cacheKey