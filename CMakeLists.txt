# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
set(PROJECT_NAME "3D-Viewer")
project(${PROJECT_NAME})

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Source files
file(GLOB SOURCES "src/*.cpp")

if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    # WebAssembly build
    message(STATUS "Building for WebAssembly with Emscripten")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

    # Executable
    add_executable(${PROJECT_NAME} ${SOURCES})

    # Fetch glm using FetchContent
    include(FetchContent)
    FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG bf71a834948186f4097caa076cd2663c69a10e1e
    )
    FetchContent_MakeAvailable(glm)

    # Add WebAssembly-specific linker flags
    add_library(webassembly_flags INTERFACE)
    target_compile_options(webassembly_flags INTERFACE
        "-sUSE_SDL=2"
        "-sUSE_WEBGL2=1"
        "-sFULL_ES3=1"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sMAX_WEBGL_VERSION=2"
        "-sMIN_WEBGL_VERSION=2"
    )
    target_link_options(webassembly_flags INTERFACE "--preload-file=assets")
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2 glm::glm webassembly_flags)

    # Copy assets directory to the build directory before linking
    add_custom_command(
        TARGET ${PROJECT_NAME} PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
    )

else()
    # Executable
    add_executable(${PROJECT_NAME} ${SOURCES})

    # Copy assets directory to the build directory
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    )

    # Find packages
    find_package(OpenGL REQUIRED)
    find_package(SDL2 REQUIRED)
    find_package(GLEW REQUIRED)

    # Link libraries
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${OPENGL_LIBRARIES}
        SDL2
        GLEW
        dl
    )
endif()

# For Linux systems, link pthread if necessary
if(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()

# Enforce build type and default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose build type" FORCE)
endif()
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

# Output messages
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Source Directory: ${CMAKE_SOURCE_DIR}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")