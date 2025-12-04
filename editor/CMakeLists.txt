cmake_minimum_required(VERSION 3.20)
project(MEEDEditor)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)
include(MEEDConfigure)

MdSetup()

file(
    GLOB 
    EDITOR_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

file(
    GLOB 
    EDITOR_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

# =============== Executable ===============
add_executable(
    ${PROJECT_NAME}
    ${EDITOR_SOURCES}
    ${EDITOR_HEADERS}
)

MEEDFindPackage(MEEDEngine REQUIRED)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE 
    MEEDEngine
)