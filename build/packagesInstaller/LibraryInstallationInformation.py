from typing import List
from .Dependency import Dependency

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
        dependencies:       List[Dependency]   = [],
        additionalDependencies: List[Dependency] = []
    ):
        self.libraryName:             str               = libraryName
        self.libraryInformation:      str               = libraryInformation 
        self.libraryVersion:          str               = libraryVersion
        self.installCommands:         str               = installCommands
        self.location:                str               = location
        self.directory:               str               = directory
        self.cacheKey:                str               = cacheKey
        self.dependencies:            List[Dependency]  = dependencies
        self.additionalDependencies:  List[Dependency]  = additionalDependencies

