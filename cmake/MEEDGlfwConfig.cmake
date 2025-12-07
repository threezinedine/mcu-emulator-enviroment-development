if (TARGET MEEDGlfw)
    return()
endif()

add_library(MEEDGlfw INTERFACE)

if (NOT PLATFORM_IS_WEB)
    include(FetchContent)

    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG latest
    )

    FetchContent_MakeAvailable(glfw)

    FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/threezinedine/glad.git
        GIT_TAG latest
    )

    FetchContent_MakeAvailable(glad)

    find_package(OpenGL REQUIRED)

    target_link_libraries(MEEDGlfw INTERFACE glfw glad OpenGL::GL)
else()
    target_link_libraries(MEEDGlfw INTERFACE)
endif()