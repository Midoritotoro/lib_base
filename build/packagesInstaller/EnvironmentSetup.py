from packagesInstaller.SystemDetection import os, win, win32, win64, winarm, mac
from packagesInstaller.NativeToolsError import finish, error, nativeToolsError

import packagesInstaller.SetupPaths

import hashlib
from typing import Dict, List

class EnvironmentConfiguration:
    def __init__(self: "EnvironmentConfiguration") -> None:
        EnvironmentConfiguration.__checkEnvironment()

        self.__qt: str | None = os.environ.get("QT")
        self.__environment: Dict[str, str] = self.__initializeEnvironment()

        self.__ignoreInCacheForThirdParty: List[str] = [
            "USED_PREFIX",
            "LIBS_DIR",
            "SPECIAL_TARGET",
            "X8664",
            "WIN32X64",
        ]

        self.__environmentKeyString:                      str = "".join(f"{key}={value};" 
            for key, value in self.__environment.items())
        self.__environmentThirdPartyEnvironmentKeyString: str = "".join(
            f"{key}={value};" for key, value in self.__environment.items() 
            if key not in self.__ignoreInCacheForThirdParty
        )
        
        self.__environmentKey               : str       = self.__generateEnvironmentKey()
        self.__environmentForThirdPartyKey  : str       = self.__generateThirdPartyEnvironmentKey()

        self.modifiedEnvironment : Dict[str, str]       = self.__createModifiedEnvironment() 

    @property
    def qt(self: 'EnvironmentConfiguration') -> str:
        return self.__qt
    
    @property
    def environment(self: 'EnvironmentConfiguration') -> Dict[str, str]:
        return self.__environment
    
    @property
    def ignoreInCacheForThirdParty(self: 'EnvironmentConfiguration') -> List[str]:
        return self.__ignoreInCacheForThirdParty
    
    @property
    def environmentKeyString(self: 'EnvironmentConfiguration') -> str:
        return self.__environmentKeyString
    
    @property
    def environmentThirdPartyEnvironmentKeyString(self: 'EnvironmentConfiguration') -> str:
        return self.__environmentThirdPartyEnvironmentKeyString
    
    @property
    def environmentKey(self: 'EnvironmentConfiguration') -> str:
        return self.__environmentKey
    
    @property
    def environmentForThirdPartyKey(self: 'EnvironmentConfiguration') -> str:
        return self.__environmentForThirdPartyKey

    def __initializeEnvironment(self: 'EnvironmentConfiguration') -> Dict[str, str]:
        environment: Dict[str, str] = {
            "USED_PREFIX"   :   packagesInstaller.SetupPaths.usedPrefix,
            "ROOT_DIR"      :   packagesInstaller.SetupPaths.rootDir,
            "LIBS_DIR"      :   packagesInstaller.SetupPaths.libsDir,
            "PATH_PREFIX"   :   packagesInstaller.SetupPaths.pathPrefix,
        }

        if (win32):
            environment.update({
                "SPECIAL_TARGET":   "win",
                "X8664"         :   'x86',
                "WIN32X64"      :   "Win32",
            })

        elif (win64):
            environment.update({
                "SPECIAL_TARGET": "win64",
                "X8664"         : "x64",
                "WIN32X64"      : "x64",
            })

        elif (winarm):
            environment.update({
                "SPECIAL_TARGET"    : "winarm",
                "X8664"             : "ARM64",
                "WIN32X64"          : "ARM64",
            })

        elif (mac):
            environment.update({
                "SPECIAL_TARGET"            : "mac",
                "MAKE_THREADS_CNT"          : "-j" + str(os.cpu_count()),
                "MACOSX_DEPLOYMENT_TARGET"  : "10.13",
                "UNGUARDED"                 : "-Werror=unguarded-availability-new",
                "MIN_VER"                   : "-mmacosx-version-min=10.13",
            })

        return environment


    def __generateEnvironmentKey(self: 'EnvironmentConfiguration') -> str:
        return hashlib.sha1(self.__environmentKeyString.encode("utf-8")).hexdigest()
    
    def __generateThirdPartyEnvironmentKey(self: 'EnvironmentConfiguration') -> str:
        return hashlib.sha1(self.__environmentKeyString.encode("utf-8")).hexdigest()
    
    def __createModifiedEnvironment(self: 'EnvironmentConfiguration') -> Dict[str, str]:
        modifiedEnvironment = os.environ.copy()

        for key, value in self.__environment.items():
            modifiedEnvironment[key] = value

        modifiedEnvironment["PATH"] = self.__environment["PATH_PREFIX"] + modifiedEnvironment["PATH"]
        return modifiedEnvironment
    
    @staticmethod
    def __checkEnvironment() -> None:        
       pass

environmentConfiguration: EnvironmentConfiguration = EnvironmentConfiguration()

