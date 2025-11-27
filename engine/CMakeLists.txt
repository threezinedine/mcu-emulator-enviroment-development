cmake_minimum_required(VERSION 3.20)
project(MEEDEngine)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../cmake)
include(MEEDConfigure)

MEEDDetectPlatform()

file(
    GLOB 
    SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/*.c
    ${CMAKE_SOURCE_DIR}/src/**/*.c
)

file(
    GLOB 
    HEADER_FILES 
    ${CMAKE_SOURCE_DIR}/include/MEEDEngine/*.h
    ${CMAKE_SOURCE_DIR}/include/MEEDEngine/**/*.h
)

add_library(
    ${PROJECT_NAME}
    STATIC
    ${SRC_FILES}
    ${HEADER_FILES}
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(
    ${PROJECT_NAME}
    PUBLIC
    ${PLATFORM_DEFINITION}
)

target_precompile_headers(
    ${PROJECT_NAME}
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include/MEEDEngine/platforms/common.h
)

add_subdirectory("examples")