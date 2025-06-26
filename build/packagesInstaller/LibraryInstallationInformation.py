class LibraryInstallationInformation: 
    def __init__(
        self:               'LibraryInstallationInformation',
        libraryName:        str, 
        libraryInformation: str = "", 
        installCommands:    str = ""
    ):
        self.__libraryName:         str     = libraryName
        self.__libraryInformation:  str     = libraryInformation 
        self.__installCommands:     str     = installCommands

    @property
    def installationCommands(self: 'LibraryInstallationInformation') -> str: 
        return self.__installCommands
    
    @installationCommands.setter
    def setInstallationCommands(
        self: 'LibraryInstallationInformation',
        installCommands: str
    ) -> None:
        self.__installCommands = installCommands


    @property
    def libraryInformation(self: 'LibraryInstallationInformation') -> str: 
        return self.__libraryInformation
    
    @libraryInformation.setter
    def setLibraryInformation(
        self: 'LibraryInstallationInformation',
        libraryInfo: str
    ) -> None:
        self.__libraryInformation = libraryInfo


    @property
    def libraryName(self: 'LibraryInstallationInformation') -> str: 
        return self.__libraryName
    
    @libraryInformation.setter
    def setLibraryName(
        self: 'LibraryInstallationInformation',
        libraryName: str
    ) -> None:
        self.__libraryName = libraryName
