add_executable(server
        server.cpp
)

# POSIX threads (required for std::thread)
find_package(Threads REQUIRED)
target_link_libraries(server PRIVATE Threads::Threads)

set_target_properties(server PROPERTIES
        OUTPUT_NAME "server"
)
