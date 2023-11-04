add_library(lib lib.c)
target_link_libraries(lib m)
target_include_directories(lib PUBLIC .)

add_executable(main main.c)
target_link_libraries(main lib)
