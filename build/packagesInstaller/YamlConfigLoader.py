import yaml 
from packagesInstaller.LibraryInstallationInformation import LibraryInstallationInformation

from packagesInstaller.NativeToolsError import executePath
from typing import Dict 

def YamlMultilineStringPresenter(dumper: yaml.Dumper, data: str | bytes) -> yaml.ScalarNode:
    if len(data.splitlines()) > 1:
        data = '\n'.join([line.rstrip() for line in data.strip().splitlines()])
        return dumper.represent_scalar('tag:yaml.org,2002:str', data, style='|')
    
    return dumper.represent_scalar('tag:yaml.org,2002:str', data)

yaml.add_representer(str, YamlMultilineStringPresenter)

class YamlConfigLoader:
    @staticmethod
    def __LibraryInfoToDict(libraryInformation: LibraryInstallationInformation) -> Dict:
        return {
            'libraryName'       :   libraryInformation.libraryName,
            'libraryInformation':   libraryInformation.libraryInformation,
            'libraryVersion'    :   libraryInformation.libraryVersion,
            'installCommands'   :   libraryInformation.installCommands,
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
            return YamlConfigLoader.__LibraryInfoFromDict(data)
