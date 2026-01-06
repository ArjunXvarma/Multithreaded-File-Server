add_executable(server
        TCPServer.cpp
        TCPServer.hpp
        FileService.hpp
        FileService.cpp
        main.cpp
        ServerSesssion.cpp
        ServerSesssion.hpp
)

# POSIX threads (required for std::thread)
find_package(Threads REQUIRED)
target_link_libraries(server PRIVATE utils metrics Threads::Threads)

set_target_properties(server PROPERTIES
        OUTPUT_NAME "server"
)
