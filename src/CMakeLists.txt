add_library(cbitset bitset.c)
add_library(cbitset::cbitset ALIAS cbitset)
target_link_libraries(cbitset PUBLIC cbitset-headers)

install(TARGETS cbitset EXPORT cbitset-targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
