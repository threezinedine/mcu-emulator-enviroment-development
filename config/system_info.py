import os
import platform


class SystemInfo:
    """
    This class is used to tracking the whole application constants which are
    used throughout the application.
    """

    def __init__(self) -> None:
        self._pythonCommand = "python3" if self.IsLinuxPlatform else "python"
        self._baseDir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

    @property
    def IsWindowsPlatform(self) -> bool:
        return platform.system() == "Windows"

    @property
    def IsLinuxPlatform(self) -> bool:
        return platform.system() == "Linux"

    @property
    def PythonCommand(self) -> str:
        return self._pythonCommand

    @property
    def BaseDir(self) -> str:
        return self._baseDir

    @property
    def PythonProjects(self) -> list[str]:
        return ["autogen"]

    @property
    def CProjects(self) -> list[str]:
        return ["engine"]

    @property
    def JSProjects(self) -> list[str]:
        return ["editor"]


SYSTEM = SystemInfo()  # Global constant instance
