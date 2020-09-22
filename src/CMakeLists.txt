add_library (cbitset bitset.c)
target_link_libraries(cbitset  PUBLIC cbitset-headers)
install(TARGETS cbitset   
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT library)