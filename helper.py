import os
from system_info import SYSTEM
from ntt_autogen import Autogen
from ntt_project_manager import Manager


def main():
    constants = dict(
        BASE_DIR=SYSTEM.BaseDir,
        PLATFORM_DEFINE=(
            "PLATFORM_IS_WINDOWS" if SYSTEM.IsWindowsPlatform else "PLATFORM_IS_LINUX"
        ),
        COMPILER_PATH=SYSTEM.CCompiler,
        INTELLISENSE_MODE=SYSTEM.IntelliSenseMode,
        PYTHON_INTERPRETER=SYSTEM.PythonInterpreter,
        PYTHON_COMMAND=SYSTEM.PythonCommand,
        PYTHON_INCLUDE_DIR=SYSTEM.PythonIncludeDir,
        ENGINE_EXAMPLES=SYSTEM.AllEngineExamples(),
    )

    Autogen(
        baseDir=os.getcwd(),
        tempFolder="temp",
        **constants,
    )
    Manager(baseDir=os.getcwd())


if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print(f"Error: {e}")
        exit(1)
