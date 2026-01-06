cmake_minimum_required(VERSION 3.16)

project(MultithreadedFileServer
        VERSION 1.0
        DESCRIPTION "Multithreaded TCP File Server in C++"
        LANGUAGES CXX)

# ---- Global C++ settings ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
)

# ---- Subdirectories ----
add_subdirectory(server)
add_subdirectory(client)
