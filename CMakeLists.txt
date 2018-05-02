cmake_minimum_required(VERSION 3.9)
project(2048)

#GLFW additions
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/glfw-3.2.1)
include_directories(lib/glfw-3.2.1)
include_directories(lib/glfw-3.2.1/deps)
include_directories(lib/glfw-3.2.1/include)

set(GLAD "${GLFW_SOURCE_DIR}/deps/glad/glad.h"
        "${GLFW_SOURCE_DIR}/deps/glad.c")

set(SOURCE_FILES src/main.cpp src/board.h)

add_executable(2048 ${SOURCE_FILES} ${GLAD})

target_link_libraries(2048 glfw ${GLFW_LIBRARIES})