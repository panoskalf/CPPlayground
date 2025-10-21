# Platform-specific source selection
if(WIN32)
set(SIMPLESOCKET_SOURCE SimpleSocket_windows.cpp)
set(SOCKET_LIBS ws2_32)
message(STATUS "Building Panos' SimpleSocket lib for Windows")
else()
set(SIMPLESOCKET_SOURCE SimpleSocket_linux.cpp)
set(SOCKET_LIBS)
message(STATUS "Building Panos' SimpleSocket for Linux/POSIX")
endif()

# RAII Utilities Library
add_library(SimpleSocket STATIC
    ${SIMPLESOCKET_SOURCE}
)

# So others can #include "SimpleSocket.h"
target_include_directories(SimpleSocket PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link platform-specific libraries
target_link_libraries(SimpleSocket ${SOCKET_LIBS})