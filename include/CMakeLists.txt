
add_library(cbitset-headers INTERFACE)
target_include_directories(cbitset-headers INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCDIR}>)
include(GNUInstallDirs)
install(FILES bitset.h portability.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cbitset )