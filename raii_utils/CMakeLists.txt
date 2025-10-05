# Platform-specific source selection
if(WIN32)
    set(SOCKET_IMPL try_sockets_windows.cpp)
    # Link Winsock library on Windows
    set(SOCKET_LIBS ws2_32 wsock32)
else()
    set(SOCKET_IMPL try_sockets_linux.cpp)
    set(SOCKET_LIBS)
endif()

add_executable(try_sockets
    main.cpp
    ${SOCKET_IMPL}
)

# Link platform-specific libraries
target_link_libraries(try_sockets ${SOCKET_LIBS})

# Print which implementation is being used
message(STATUS "Using socket implementation: ${SOCKET_IMPL}")