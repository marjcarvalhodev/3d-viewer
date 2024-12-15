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

# Add TinyObjLoader implementation file
list(APPEND SOURCES "${CMAKE_SOURCE_DIR}/src/tiny_obj_loader.cc")

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
target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    SDL2
    GLEW
    dl
)

# For Linux systems, link pthread if necessary
if(UNIX)
    target_link_libraries(${PROJECT_NAME} pthread)
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
