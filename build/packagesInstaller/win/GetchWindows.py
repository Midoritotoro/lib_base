class GetchWindows:
    def __init__(self):
        import msvcrt

    def __call__(self):
        import msvcrt
        try:
            c = msvcrt.getch().decode('ascii')
            return c
        except UnicodeDecodeError:
            return 's'