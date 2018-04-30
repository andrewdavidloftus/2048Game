cmake_minimum_required(VERSION 3.10)
project(Meh)

include_directories(
        include
        lib/glad/include
        lib/glfw/include
)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib/glfw)

add_executable(Meh main.cpp glad.c)
target_link_libraries(Meh glfw)