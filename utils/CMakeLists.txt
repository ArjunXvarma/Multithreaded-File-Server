add_library(utils STATIC
        Logger.cpp
        SocketUtils.cpp
)

target_include_directories(utils
        PUBLIC
        ${CMAKE_SOURCE_DIR}
)
