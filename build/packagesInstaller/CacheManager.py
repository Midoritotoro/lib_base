from packagesInstaller.EnvironmentSetup import environmentConfiguration
from packagesInstaller.NativeToolsError import error

from packagesInstaller.InstallExecutor import InstallExecutor
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
    def ComputeCacheKey(installExecutor: InstallExecutor) -> str:
        if (installExecutor.installationInformation.location == 'ThirdParty'):
            envKey = environmentConfiguration.environmentForThirdPartyKey
        else:
            envKey = environmentConfiguration.environmentKey

        objects: List[str] = [
            envKey,
            installExecutor.installationInformation.location,
            installExecutor.installationInformation.libraryName,
            installExecutor.installationInformation.libraryVersion,
            installExecutor.installationInformation.installationCommands
        ]

        for pattern in installExecutor.installationInformation.dependencies:
            pathlist: List[str | bytes] = glob.glob(os.path.join(libsDir, pattern))
            items: List[str] = [pattern]

            if len(pathlist) == 0:
                error('Nothing found: ' + pattern)

            for path in pathlist:
                if not os.path.exists(path):
                    error('Not found: ' + path)

                items.append(CacheManager.ComputeFileHash(path))
            objects.append(':'.join(items))

        return hashlib.sha1(';'.join(objects).encode('utf-8')).hexdigest()
    
    @staticmethod
    def KeyPath(installExecutor: InstallExecutor) -> str:
        return os.path.join(
            installExecutor.installationInformation.directory, keysLoc,
            installExecutor.installationInformation.libraryName
        )
    
    @staticmethod
    def CheckCacheKey(installExecutor: InstallExecutor) -> Literal[CacheKeyState.Stale, CacheKeyState.Good, CacheKeyState.NotFound]:
        if len(installExecutor.installationInformation.cacheKey) <= 0: # if not 'key' in stage
            error('Key not set in stage: ' + installExecutor.installationInformation.libraryName)

        key: str = CacheManager.KeyPath(installExecutor)

        if not os.path.exists(os.path.join(
            installExecutor.installationInformation.directory, 
            installExecutor.installationInformation.libraryName
        )):
            return CacheManager.CacheKeyState.NotFound
        
        if not os.path.exists(key):
            return CacheManager.CacheKeyState.Stale
        
        with open(key, 'r') as file:
            return CacheManager.CacheKeyState.Good if (file.read() == installExecutor.installationInformation.cacheKey) else CacheManager.CacheKeyState.Stale
        
    @staticmethod
    def ClearCacheKey(installExecutor: InstallExecutor):
        key: str = CacheManager.KeyPath(installExecutor)

        if os.path.exists(key):
            os.remove(key)

    @staticmethod
    def WriteCacheKey(installExecutor: InstallExecutor):
        if len(installExecutor.installationInformation.cacheKey) <= 0: # if not 'key' in stage
            error('Key not set in stage: ' + installExecutor.installationInformation.libraryName)

        key: str = CacheManager.KeyPath(installExecutor)
        
        with open(key, 'w') as file:
            file.write(installExecutor.installationInformation.cacheKey)
