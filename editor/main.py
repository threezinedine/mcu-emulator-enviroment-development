import sys
import resources_rc  # type: ignore
from PyQt6.QtGui import QIcon
from views import EditorMainWindow
from PyQt6.QtWidgets import QApplication


def main():
    app = QApplication(sys.argv)
    appIcon = QIcon()
    appIcon.addFile(
        ":/assets/images/meed-logo.png",
        mode=QIcon.Mode.Normal,
        state=QIcon.State.Off,
    )
    app.setWindowIcon(appIcon)
    mainWindow = EditorMainWindow()
    mainWindow.showMaximized()
    sys.exit(app.exec())


if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        pass
