from packagesInstaller.InstallationManager import InstallationManager

from packagesInstaller.LibraryInstallationCommands import installCommands
from packagesInstaller.YamlConfigLoader import YamlConfigLoader

if __name__ == "__main__":
    def main() -> None: 
        manager: InstallationManager = InstallationManager()
        manager.addInstallation("patches")
        manager.addInstallation("msys64")
        manager.addInstallation("gas-preprocessor")
        manager.addInstallation("ffmpeg")
        manager.executeAll()
        # for command in installCommands:
        #     print(command.libraryName)
        #     YamlConfigLoader.DumpLibraryInformationToYaml(command, f"D:/lib_base/build/packagesInstaller/build_instructions/{command.libraryName}.yaml")

    main()