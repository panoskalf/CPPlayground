# Platform-specific source selection
if(WIN32)
  set(SIMPLESOCKET_SOURCE SimpleSocket_windows.cpp)
  set(SOCKET_LIBS ws2_32)
  message(STATUS "Building Panos' SimpleSocket lib for Windows")

  # RAII Utilities Library
  add_library(SimpleSocket STATIC ${SIMPLESOCKET_SOURCE})
  # So others can #include "SimpleSocket.h"
  target_include_directories(SimpleSocket PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
  # Link platform-specific libraries
  target_link_libraries(SimpleSocket ${SOCKET_LIBS})
else()
  set(SOCKET_LIBS)
  message(
    STATUS
      "SimpleSocket Linux implementation not yet available - creating header-only target"
  )

  # Create header-only library for Linux (no implementation yet)
  add_library(SimpleSocket INTERFACE)
  target_include_directories(SimpleSocket INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
endif()
