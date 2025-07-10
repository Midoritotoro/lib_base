from packagesInstaller.SystemDetection import win, mac, winarm, win32, win64

class Dependency:
    def __init__(
        self:           'Dependency',
        platformName:   str,
        dependencyName: str
    ) -> None:
        self.platformName:      str = platformName
        self.dependencyName:    str = dependencyName

    def toDict(self: 'Dependency') -> dict[str, str]:
        return {
            "platformName": self.platformName,
            "dependencyName": self.dependencyName
        }

    @staticmethod
    def fromDict(dependencyDict: dict[str, str]) -> 'Dependency':
        return Dependency(
            platformName=dependencyDict["platformName"],
            dependencyName=dependencyDict["dependencyName"]
        )

    def isSupportedForSystem(self: 'Dependency') -> bool: 
        if win and self.platformName == "win":
            return True
        elif win32 and self.platformName == "win32":
            return True
        elif win64 and self.platformName == "win64":
            return True
        elif winarm and self.platformName == "winarm": 
            return True
        elif mac and self.platformName == "mac":
            return True

        return False


    

