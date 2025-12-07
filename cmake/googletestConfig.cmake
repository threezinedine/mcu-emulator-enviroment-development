if (TARGET googletest)
    return()
endif()

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

add_library(googletest INTERFACE)
target_link_libraries(
    googletest
    INTERFACE
    gtest
    gtest_main
    gmock
    gmock_main
)