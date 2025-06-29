import yaml 
from packagesInstaller.LibraryInstallationInformation import LibraryInstallationInformation

from typing import Dict 

class YamlConfigLoader:
    @staticmethod
    def __LibraryInfoToDict(libraryInformation: LibraryInstallationInformation) -> Dict:
        return {
            'libraryName'       :   libraryInformation.libraryName,
            'libraryInformation':   libraryInformation.libraryInformation,
            'libraryVersion'    :   libraryInformation.libraryVersion,
            'installCommands'   :   libraryInformation.installationCommands,
            'location'          :   libraryInformation.location,
            'directory'         :   libraryInformation.directory,
            'cacheKey'          :   libraryInformation.cacheKey,
            'dependencies'      :   libraryInformation.dependencies,
        }

    @staticmethod
    def __LibraryInfoFromDict(data: Dict) -> LibraryInstallationInformation:
        return LibraryInstallationInformation(
            libraryName=data['libraryName'],
            libraryInformation=data.get('libraryInformation', ""),
            libraryVersion=data.get('libraryVersion', ""),
            installCommands=data.get('installCommands', ""),
            location=data.get('location', ""),
            directory=data.get('directory', ""),
            cacheKey=data.get('cacheKey', ""),
            dependencies=data.get('dependencies', [])
        )

    @staticmethod
    def DumpLibraryInformationToYaml(
        libraryInformation: LibraryInstallationInformation, 
        output: str
    ) -> None:
        with open(output, 'w') as file:
            yaml.dump(YamlConfigLoader.__LibraryInfoToDict(libraryInformation), file)

    @classmethod
    def ExtractLibraryInformationFromYaml(libraryName: str) -> LibraryInstallationInformation:
        with open(f"build_instructions/{libraryName}.yaml", 'r') as yaml_file:
            data: Dict = yaml.safe_load(yaml_file)
            return YamlConfigLoader.__LibraryInfoFromDict(data)
