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
    ) -> None: ...

    @property
    def installationCommands(self: 'LibraryInstallationInformation') -> str: ...
    @installationCommands.setter
    def setInstallationCommands(
        self:               'LibraryInstallationInformation',
        installCommands:    str
    ) -> None:  ...
    

    @property
    def location(self: 'LibraryInstallationInformation') -> str: ...
    @location.setter
    def setLocation(
        self:       'LibraryInstallationInformation',
        location:   str
    ) -> None: ...


    @property
    def directory(self: 'LibraryInstallationInformation') -> str: ...
    @location.setter
    def setDirectory(
        self:       'LibraryInstallationInformation',
        directory:  str
    ) -> None: ...


    @property
    def libraryInformation(self: 'LibraryInstallationInformation') -> str: ...
    @libraryInformation.setter
    def setLibraryInformation(
        self:           'LibraryInstallationInformation',
        libraryInfo:    str
    ) -> None: ...


    @property
    def libraryName(self: 'LibraryInstallationInformation') -> str: ...
    @libraryInformation.setter
    def setLibraryName(
        self:           'LibraryInstallationInformation',
        libraryName:    str
    ) -> None: ...


    @property
    def dependencies(self: 'LibraryInstallationInformation') -> List[str]: ...
    @installationCommands.setter
    def setDependencies(
        self:           'LibraryInstallationInformation',
        dependencies:   List[str]
    ) -> None: ...


    @property
    def libraryVersion(self: 'LibraryInstallationInformation') -> str: ...
    @installationCommands.setter
    def setLibraryVersion(
        self:           'LibraryInstallationInformation',
        libraryVersion: str
    ) -> None: ...

    @property
    def cacheKey(self: 'LibraryInstallationInformation') -> str: ...
    @installationCommands.setter
    def setCacheKey(
        self:           'LibraryInstallationInformation',
        cacheKey:       str
    ) -> None: ...