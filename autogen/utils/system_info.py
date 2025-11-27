import os


class SystemInfo:
    @property
    def BASE_DIR(self) -> str:
        return os.path.dirname(
            os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        )


SYSTEM = SystemInfo()
TEMPLATE_DATA = dict(
    BASE_DIR=SYSTEM.BASE_DIR,
)
