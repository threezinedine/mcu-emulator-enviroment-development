import os
from config import (
    logger,
    SYSTEM,
    RunAutogen,
    CreateEnvironment,
    ValidateCommandExist,
    InstallCDependencies,
)

from ntt_project_manager import Manager


def main():

    logger.debug("Start checking the system health...")
    ValidateCommandExist(SYSTEM.PythonCommand)
    ValidateCommandExist("cmake")
    ValidateCommandExist("clang")
    ValidateCommandExist("git")
    logger.info("System health check passed.")

    Manager(baseDir=os.getcwd())

    CreateEnvironment(dir="autogen")
    InstallCDependencies()
    RunAutogen()

    # if args.IsBuild:
    #     if args.IsCProject:
    #         BuildCProject(**args.Args)
    #     elif args.IsPythonProject:
    #         pass
    # elif args.IsPackage:
    #     if args.IsPythonProject:
    #         InstallPythonDependencies(**args.Args)
    #     elif args.IsCProject:
    #         # InstallCDependencies(**args.Args)
    #         pass
    #     else:
    #         raise NotImplementedError(
    #             f'Packaging for project "{args.args.project}" is not implemented.'
    #         )
    # elif args.IsRunTest:
    #     if args.IsPythonProject:
    #         RunPythonTest(**args.Args)
    #     elif args.IsCProject:
    #         BuildCProject(**args.Args)
    #         RunCProjectTest(**args.Args)
    #     else:
    #         raise NotImplementedError(
    #             f'Testing for project "{args.args.project}" is not implemented.'
    #         )
    # elif args.IsRunDesigner:
    #     OpenDesigner(**args.Args)
    # elif args.IsRun:
    #     if args.IsPythonProject:
    #         BuildCProject(**(args.Args | dict(project="engine")))
    #     elif args.IsCProject:
    #         RunCApplication(**args.Args)
    # elif args.IsRunExample:
    #     RunExample(**args.Args)
    # elif args.IsRunAutogen:
    #     pass
    # else:
    #     raise NotImplementedError(
    #         f"The command {args.args.command} is missing or not implemented."
    #     )


if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        logger.error(f"An error occurred: {e}")
        exit(1)
