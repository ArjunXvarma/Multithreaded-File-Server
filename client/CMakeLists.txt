add_executable(client
        ClientSession.cpp
        ClientSession.hpp
        main.cpp
)

target_link_libraries(client PRIVATE utils metrics)

set_target_properties(client PROPERTIES
        OUTPUT_NAME "client"
)
