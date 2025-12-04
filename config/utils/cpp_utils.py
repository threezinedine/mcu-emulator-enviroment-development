import os
import re
import json
import shutil
from typing import Any

from ..log import logger
from .utils import RunCommand
from ..system_info import SYSTEM
from dataclasses import dataclass, field


@dataclass
class Dependency:
    name: str
    url: str
    commit: str | None = field(default=None)
    branch: str | None = field(default=None)
    develop: bool = field(
        default=False
    )  # indicates in the dev mode, fetch and pull latest each time run


def InstallCDependencies(
    **kwargs: Any,
) -> None:
    """
    Installs necessary dependencies for the C++ project which is loaded from the `dependecies.json` file. The
        code will be placed at `external/` folder.
    """
    dependencies: list[Dependency] = []

    with open("dependencies.json", "r") as depFile:
        dependenciesData = json.load(depFile)

        for depData in dependenciesData:
            dependency = Dependency(**depData)
            dependencies.append(dependency)

    externalDir = os.path.join(os.getcwd(), "externals")
    if not os.path.exists(externalDir):
        logger.debug(f'Creating external directory at "{externalDir}"')
        os.makedirs(externalDir)

    for dependency in dependencies:
        if dependency.develop:
            assert (
                dependency.branch is None
            ), f'Dependency "{dependency.name}" cannot have both "develop" and "branch" set.'

        dependencyDir = os.path.join(externalDir, dependency.name)
        if os.path.exists(dependencyDir):
            if dependency.develop:
                logger.info(
                    f'Updating dependency "{dependency.name}" in develop mode...'
                )
                RunCommand("git fetch", cwd=dependencyDir)
                RunCommand(
                    f"git pull {"" if dependency.branch is None else dependency.branch}",
                    cwd=dependencyDir,
                )
                RunCommand("git submodule update --init --recursive", cwd=dependencyDir)
                continue
            else:
                logger.debug(
                    f"Dependency {dependency.name} already exists at {dependencyDir}, skipping..."
                )
                continue

        logger.info(
            f'Cloning dependency "{dependency.name}" from "{dependency.url}"...'
        )
        RunCommand(f"git clone {dependency.url} {dependencyDir}")

        if dependency.branch:
            logger.info(
                f'Checking out branch "{dependency.branch}" for dependency "{dependency.name}"...'
            )
            RunCommand(f"git checkout {dependency.branch}", cwd=dependencyDir)
        else:
            logger.info(
                f'No specific branch provided for dependency "{dependency.name}", using default branch.'
            )

        if dependency.commit:
            logger.info(
                f'Checking out commit "{dependency.commit}" for dependency "{dependency.name}"...'
            )
            RunCommand(f"git checkout {dependency.commit}", cwd=dependencyDir)
        else:
            logger.info(
                f'No specific commit provided for dependency "{dependency.name}", using default branch.'
            )

        RunCommand(f"git submodule update --init --recursive", cwd=dependencyDir)


def BuildCProject(
    project: str = "",
    type: str = "debug",
    force: bool = False,
    reload: bool = False,
    **kwargs: Any,
) -> None:
    """
    Builds a C++ project using CMake. Supports different build types and platforms.

    Arguments:
        project (str): The path to the C++ project.
        web (bool): Whether to build for web platform using Emscripten. Defaults to False.
        type (str): The build type, either 'debug' or 'release'. Defaults to 'debug'.

    """
    if force or reload:
        buildDir = os.path.join(SYSTEM.BaseDir, project, "build", type)
        if os.path.exists(buildDir):
            logger.info(
                f'Removing existing build directory at "{buildDir}" for reload...'
            )

            shutil.rmtree(buildDir)

    configDir = os.path.join(SYSTEM.BaseDir, project, "config")
    additionalOptions = ReadConfigFile(f"{type}.cfg", configDir)

    if SYSTEM.IsWindowsPlatform:
        additionalOptions += ' -G "Visual Studio 17 2022"'

    if type.lower() == "release" or type.lower() == "web":
        additionalOptions += " -DCMAKE_BUILD_TYPE=Release"
    elif type.lower() == "debug":
        additionalOptions += " -DCMAKE_BUILD_TYPE=Debug"

    prefix = ""

    from ..conf import VARIABLES

    if type == "web":
        prefix = f"{VARIABLES.EMCMAKE}"

    logger.info(f'Building project "{project}" with build type "{type}"...')
    RunCommand(f"{prefix} cmake -S . -B build/{type} {additionalOptions}", cwd=project)
    RunCommand(f"cmake --build build/{type} --config {type.capitalize()}", cwd=project)


def ReadConfigFile(name: str, folder: str) -> str:
    """
    Reads a configuration file and returns its content as a string.

    Arguments:
        name (str): The name of the configuration file.
        folder (str): The folder where the configuration file is located.

    Returns:
        str: The content of the configuration file.
    """
    configPath = os.path.join(folder, name)
    if not os.path.exists(configPath):
        logger.warning(f'Configuration file "{configPath}" does not exist.')
        return ""

    additionalOptions = ""

    with open(configPath, "r") as cfgFile:
        content = cfgFile.read()
        for index, line in enumerate(content.splitlines()):
            line = line.strip()

            if line == "" or line.startswith("//") or line.startswith("#"):
                continue

            if re.match(r"^[A-Za-z0-9_\-]+=(.*)$", line):
                additionalOptions += f" -D{line}"
                continue

            includeMatch = re.match(r"^<[A-Za-z0-9_\-]+.cfg>$", line)

            if includeMatch:
                includeFileName = line[1:-1]
                logger.debug(
                    f'Including configuration file "{includeFileName}" from line {index + 1} in "{name}".'
                )
                includedOptions = ReadConfigFile(includeFileName, folder)
                additionalOptions += f" {includedOptions}"
                continue

            logger.warning(
                f'Unrecognized line "{line}" in configuration file "{name}" at line {index + 1}.'
            )

    return additionalOptions


def RunCProjectTest(
    project: str = "engine",
    type: str = "debug",
    filter: str = "",
    **kwargs: Any,
) -> None:
    """
    Runs the test suite for the engine project.

    Arguments:
        filter (str): Optional filter to run specific tests by name.
    """

    logger.info(f'Running "{project}" tests...')

    if project == "engine":
        testExecutable = "MEEDEngineTests"
        if SYSTEM.IsWindowsPlatform:
            engineTestDir = os.path.join(
                SYSTEM.BaseDir, project, "build", type, "tests", type.capitalize()
            )
            testCommand = f"{testExecutable}.exe"
        else:
            engineTestDir = os.path.join(
                SYSTEM.BaseDir, project, "build", type, "tests"
            )
            testCommand = f"./{testExecutable}"
    elif project == "editor":
        testExecutable = "MEEDEditorTest"

        if SYSTEM.IsWindowsPlatform:
            engineTestDir = os.path.join(
                SYSTEM.BaseDir, project, "build", type, type.capitalize()
            )
            testCommand = f"{testExecutable}.exe"
        else:
            engineTestDir = os.path.join(
                SYSTEM.BaseDir,
                project,
                "build",
                type,
            )
            testCommand = f"./{testExecutable}"
    else:
        raise ValueError(f'Unknown project "{project}" for running tests.')

    if filter:
        testCommand += f' --gtest_filter="{filter}"'

    RunCommand(testCommand, cwd=engineTestDir)


def RunExample(
    examples: list[str],
    type: str = "debug",
    **kwargs: Any,
) -> None:
    """
    Runs example projects.

    Arguments:
        examples (list[str]): List of example project names to run.
    """
    if SYSTEM.IsWindowsPlatform:
        exampleDir = os.path.join(
            SYSTEM.BaseDir,
            "engine",
            "build",
            type,
            "examples",
            type.capitalize(),
        )
    elif SYSTEM.IsLinuxPlatform:
        exampleDir = os.path.join(
            SYSTEM.BaseDir,
            "engine",
            "build",
            type,
            "examples",
        )
    else:
        raise NotImplementedError(
            "Current platform is not supported for running examples."
        )

    BuildCProject("engine", type=type, **kwargs)

    logger.debug(f"Example directory resolved to: {exampleDir}")

    for example in examples:
        exampleExecutable: str = example
        if SYSTEM.IsWindowsPlatform:
            exampleExecutable += ".exe"
        else:
            exampleExecutable = f"./{exampleExecutable}"

        if not os.path.exists(os.path.join(exampleDir, exampleExecutable)):
            logger.error(
                f'Example project "{example}" does not exist at path "{os.path.join(exampleDir, exampleExecutable)}".'
            )
            continue

        logger.info(f'Running example project "{example}"...')

        RunCommand(f"{exampleExecutable}", cwd=exampleDir)


def RunCApplication(
    project: str = "app",
    type: str = "debug",
    **kwargs: Any,
) -> None:
    """
    Runs the main application.

    Arguments:
        type (str): The build type, either 'debug' or 'release'. Defaults to 'debug'.
    """
    BuildCProject(**(kwargs | dict(project=project, type=type)))

    if type == "web" and project == "app":
        RunWebApplication(**kwargs)
        return

    if SYSTEM.IsWindowsPlatform:
        appDir = os.path.join(
            SYSTEM.BaseDir,
            project,
            "build",
            type,
            type.capitalize(),
        )
    elif SYSTEM.IsLinuxPlatform:
        appDir = os.path.join(
            SYSTEM.BaseDir,
            project,
            "build",
            type,
        )
    else:
        raise NotImplementedError(
            "Current platform is not supported for running the application."
        )

    logger.debug(f"Application directory resolved to: {appDir}")

    logger.info(f"Running application...")

    applicationName = "MEEDApp"

    if project == "editor":
        applicationName = "MEEDEditor"

    if SYSTEM.IsWindowsPlatform:
        RunCommand(f"{applicationName}.exe", cwd=appDir)
    elif SYSTEM.IsLinuxPlatform:
        RunCommand(f"./{applicationName}", cwd=appDir)


def RunWebApplication(
    port: int = 8080,
    **kwargs: Any,
) -> None:
    """
    Runs the web application using Emscripten.

    Arguments:
        None
    """

    appDir = os.path.join(
        SYSTEM.BaseDir,
        "app",
        "public",
    )

    logger.info(f"Running web application on http://localhost:{port}...")

    RunCommand(f"{SYSTEM.PythonCommand} server.py", cwd=appDir)
