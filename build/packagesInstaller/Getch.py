class Getch:
    """Gets a single character from standard input.  Does not echo to the screen."""

    def __init__(self):
        try:
            import packagesInstaller.win.GetchWindows as GetchWindows
            self.impl = GetchWindows()
        except ImportError:
            import packagesInstaller.unix.GetchUnix as GetchUnix
            self.impl = GetchUnix()

    def __call__(self):
        return self.impl()
    
getch = Getch()
ch = getch()