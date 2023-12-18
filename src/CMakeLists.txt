add_library(settings settings.c)
target_include_directories(settings PUBLIC .)

add_library(surface_map surface_map.c)
target_include_directories(surface_map PUBLIC .)

add_library(cost_map cost_map.c)
target_include_directories(cost_map PUBLIC .)

add_library(path_algorithm path_algorithm.c)
target_include_directories(path_algorithm PUBLIC .)

add_library(visualizers visualizers.c)
target_include_directories(visualizers PUBLIC .)

add_executable(main main.c)
target_link_libraries(main settings surface_map cost_map path_algorithm visualizers)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/map.txt
        ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)