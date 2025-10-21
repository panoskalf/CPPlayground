# Example RAII utility libraries implementation

Collection of my experiments which turned out to be small "libraries"

## TODO
- implement linux support for SimpleSocket utilities
  *general task notes:*
  Aspect         | Windows Implementation	| Linux Implementation
  ------         | ------                 | -------
  Socket Type	   | SOCKET (Windows handle)| int (POSIX file descriptor)
  Headers        | #include <winsock2.h>  | #include <sys/socket.h>
  Initialization | WSAStartup() required  | No initialization needed
  Error Handling | WSAGetLastError()      | errno + strerror()
  Create Socket  | socket() → SOCKET      | socket() → int
  Close Socket   | closesocket(s)         | close(s)
  Invalid Value  | INVALID_SOCKET         | -1