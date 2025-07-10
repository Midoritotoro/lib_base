from packagesInstaller.EnvironmentSetup import environmentConfiguration
from packagesInstaller.NativeToolsError import error

from packagesInstaller.LibraryInstallationInformation import LibraryInstallationInformation
from packagesInstaller.SetupPaths import keysLoc, libsDir

from typing import List, Literal

import hashlib
import os

import glob
from enum import Enum


class CacheManager: 
    class CacheKeyState(Enum):
        Good        = 1
        Stale       = 2
        NotFound    = 3

    @staticmethod
    def ComputeFileHash(path: str) -> str:
        sha1: hashlib._Hash = hashlib.sha1()

        with open(path, 'rb') as f:
            while True:
                data : bytes = f.read(256 * 1024)
                if not data:
                    break

                sha1.update(data)
                
        return sha1.hexdigest()
    
    @staticmethod
    def ComputeCacheKey(information: LibraryInstallationInformation) -> str:
        if (information.location == 'ThirdParty'):
            envKey = environmentConfiguration.environmentForThirdPartyKey
        else:
            envKey = environmentConfiguration.environmentKey

        objects: List[str] = [
            envKey,
            information.location,
            information.libraryName,
            information.libraryVersion,
            information.installCommands
        ]

        for pattern in information.dependencies:
            if not pattern.isSupportedForSystem():
                continue

            pathlist: List[str | bytes] = glob.glob(os.path.join(libsDir, pattern.dependencyName))
            items: List[str] = [pattern.dependencyName]

            if len(pathlist) == 0:
                error('Nothing found: ' + pattern.dependencyName)

            for path in pathlist:
                if not os.path.exists(path):
                    error('Not found: ' + path)

                items.append(CacheManager.ComputeFileHash(path))
            objects.append(':'.join(items))

        return hashlib.sha1(';'.join(objects).encode('utf-8')).hexdigest()
    
    @staticmethod
    def KeyPath(information: LibraryInstallationInformation) -> str:
        return os.path.join(
            information.directory, keysLoc,
            information.libraryName
        )
    
    @staticmethod
    def CheckCacheKey(information: LibraryInstallationInformation) -> Literal[CacheKeyState.Stale, CacheKeyState.Good, CacheKeyState.NotFound]:
        if len(information.cacheKey) <= 0:
            error('Key not set in stage: ' + information.libraryName)

        key: str = CacheManager.KeyPath(information)

        if not os.path.exists(os.path.join(
            information.directory, 
            information.libraryName
        )):
            return CacheManager.CacheKeyState.NotFound
        
        if not os.path.exists(key):
            return CacheManager.CacheKeyState.Stale
        
        with open(key, 'r') as file:
            return CacheManager.CacheKeyState.Good if (file.read() == information.cacheKey) else CacheManager.CacheKeyState.Stale
        
    @staticmethod
    def ClearCacheKey(information: LibraryInstallationInformation):
        key: str = CacheManager.KeyPath(information)

        if os.path.exists(key):
            os.remove(key)

    @staticmethod
    def WriteCacheKey(information: LibraryInstallationInformation):
        if len(information.cacheKey) <= 0:
            error('Key not set in stage: ' + information.libraryName)

        key: str = CacheManager.KeyPath(information)
        
        with open(key, 'w') as file:
            file.write(information.cacheKey)
