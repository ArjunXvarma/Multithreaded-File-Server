add_executable(server
        ClientSession.cpp
        ClientSession.hpp
        TCPServer.cpp
        TCPServer.hpp
        FileService.cpp
        FileService.hpp
        main.cpp
)

# POSIX threads (required for std::thread)
find_package(Threads REQUIRED)
target_link_libraries(server PRIVATE utils metrics Threads::Threads)

set_target_properties(server PROPERTIES
        OUTPUT_NAME "server"
)
