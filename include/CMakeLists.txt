add_library(cbitset-headers INTERFACE)
add_library(cbitset::cbitset-headers ALIAS cbitset-headers)
target_include_directories(cbitset-headers INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(TARGETS cbitset-headers EXPORT cbitset-targets)
install(FILES bitset.h portability.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cbitset)
