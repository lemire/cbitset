add_library (cbitset bitset.c)
target_link_libraries(cbitset  PUBLIC cbitset-headers)
install(TARGETS cbitset)