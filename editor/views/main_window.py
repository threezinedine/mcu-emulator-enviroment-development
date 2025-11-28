from PyQt6.QtCore import Qt
from PyQt6.QtWidgets import QMainWindow, QWidget
from PyQt6.QtGui import QIcon, QPixmap


class EditorMainWindow(QMainWindow):
    """
    The entry application main window.
    """

    def __init__(
        self,
        parent: QWidget | None = None,
        flags: Qt.WindowType = Qt.WindowType.Widget,
    ) -> None:
        super().__init__(parent, flags)

        self.setWindowTitle("MCU Emulator Environment Development")
        self.setWindowIcon(QIcon(QPixmap(":/assets/images/meed-logo.png")))
