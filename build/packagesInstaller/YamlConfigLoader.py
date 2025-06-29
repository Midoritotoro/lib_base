import yaml 
from packagesInstaller.LibraryInstallationInformation import LibraryInstallationInformation

from packagesInstaller.NativeToolsError import executePath
from typing import Dict 

def represent_str_literal(dumper, data):
    if '\n' in data:
        return dumper.represent_scalar('tag:yaml.org,2002:str', data, style='|')
    return dumper.represent_scalar('tag:yaml.org,2002:str', data)

yaml.add_representer(str, represent_str_literal)

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
    def __LibraryInfoFromDict(data: dict) -> LibraryInstallationInformation:
        return LibraryInstallationInformation(
            libraryName=data.get('libraryName', ""),
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
            yaml.dump(YamlConfigLoader.__LibraryInfoToDict(libraryInformation), file, default_flow_style=False, default_style="|")

    @staticmethod
    def ExtractLibraryInformationFromYaml(libraryName: str) -> LibraryInstallationInformation:
        with open(f"{executePath}\\packagesInstaller\\build_instructions\\{libraryName}.yaml", 'r') as yaml_file:
            data: Dict = yaml.safe_load(yaml_file)
            print(data.get('installCommands', ""))
            return YamlConfigLoader.__LibraryInfoFromDict(data)
