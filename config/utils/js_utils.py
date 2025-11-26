import os
from typing import Any
from ..log import logger
from .utils import RunCommand


def SetupEditor() -> None:
    """
    Setup the JavaScript editor by installing necessary dependencies.
    """

    logger.debug("Setting up the JavaScript editor...")

    if os.path.exists("editor/node_modules"):
        logger.debug("Editor dependencies already installed, skipping...")
        return

    logger.info("Installing editor dependencies using npm...")
    RunCommand("npm install", cwd="editor")


def InstallEditorDependencies(
    dependencies: list[str],
    save_dev: bool = False,
    **kwargs: Any,
) -> None:
    """
    Install additional dependencies for the JavaScript editor.
    Arguments
    ---------
    dependencies : list[str]
        The list of dependencies to install.
    save_dev : bool
        Whether to save the installed dependencies to dev-requirements.txt.
    """
    if not dependencies:
        logger.debug("No additional editor dependencies to install, skipping...")
        return

    depsString = " ".join(dependencies)
    logger.info(f"Installing additional editor dependencies: {depsString}")
    RunCommand(
        f"npm install {depsString} {'--save-dev' if save_dev else ''}",
        cwd="editor",
    )


def RunEditor(
    **kwargs: Any,
) -> None:
    """
    Run editor executable after building it.
    """

    logger.info("Starting the editor...")
    RunCommand("npm run start", cwd="editor")
