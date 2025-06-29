from packagesInstaller.InstallationManager import InstallationManager

if __name__ == "__main__":
    def main() -> None: 
        manager: InstallationManager = InstallationManager()
        manager.addInstallation("ffmpeg")

    main()