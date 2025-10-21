# Beej's Socket Programming Examples

This folder contains socket programming examples inspired by [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/).

## Implementation Approach

- **Windows Examples**: Use my custom `SimpleSocket` C++ RAII wrapper library (from `raii_utils/`)
- **Linux Examples**: Use Beej's original C implementations (planned)

## Examples

- `client_example` - TCP client connecting to a server
- `server_example` - Multi-threaded TCP server
- `listener_example` - UDP server (datagram listener)
- `talker_example` - UDP client (datagram sender)

## About Beej's Guide

Beej's Guide to Network Programming is a excellent, freely available tutorial for learning socket programming. The original guide and examples are in the public domain.

**Note**: These examples are educational implementations based on concepts from Beej's guide. Windows code has been adapted for modern C++ with RAII principles.