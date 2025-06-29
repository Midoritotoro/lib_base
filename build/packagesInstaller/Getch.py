from typing import Any

class Getch:
    """Gets a single character from standard input.  Does not echo to the screen."""

    def __init__(self: 'Getch') -> None:
        try:
            from packagesInstaller.win.GetchWindows import GetchWindows
            self.__implementation: GetchWindows = GetchWindows()
        except ImportError:
            from packagesInstaller.unix.GetchUnix import GetchUnix
            self.__implementation: GetchUnix    = GetchUnix()

    def __call__(self: 'Getch') -> str | Any:
        return self.__implementation()
    
getch = Getch()
ch = getch()