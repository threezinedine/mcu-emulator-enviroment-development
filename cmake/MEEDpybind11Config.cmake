if (TARGET MEEDpybind11)
    return()
endif()

include(FetchContent)

FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
)

FetchContent_MakeAvailable(pybind11)

find_package(Python3 REQUIRED)

add_library(MEEDpybind11 INTERFACE)

target_link_libraries(
    MEEDpybind11
    INTERFACE
    pybind11::pybind11
)