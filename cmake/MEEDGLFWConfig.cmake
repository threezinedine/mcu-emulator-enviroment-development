if (TARGET MEEDGLFW)
    return()
endif()

set(MEEDGLFW_DIR "${EXTERNALS_DIR}/glfw")
add_subdirectory("${MEEDGLFW_DIR}" "${CMAKE_BINARY_DIR}/packages/glfw")

set(GLAD_DIR "${EXTERNALS_DIR}/glad")

find_package(OpenGL REQUIRED)

file(
    GLOB 
    MEEDGLFW_GLAD_SOURCES
    "${GLAD_DIR}/src/glad.c"
)

add_library(
    MEEDGlad
    STATIC
    ${MEEDGLFW_GLAD_SOURCES}
)

target_include_directories(
    MEEDGlad 
    PUBLIC
    ${GLAD_DIR}/include
)

add_library(MEEDGLFW INTERFACE)
target_link_libraries(MEEDGLFW INTERFACE glfw MEEDGlad OpenGL::GL)