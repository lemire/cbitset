
add_executable(benchmark benchmark.c)
add_executable(lemirebenchmark benchmark.c)

target_link_libraries(benchmark PUBLIC cbitset )
target_link_libraries(lemirebenchmark PUBLIC cbitset )