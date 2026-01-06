add_library(metrics STATIC
        metrics.cpp
)

target_include_directories(metrics
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
