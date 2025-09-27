#ifndef COMMON_H
#define COMMON_H

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include "typedefs.h

typedef s32 SOCKET;
#endif

extern s32 set_nonblocking(SOCKET sock); // Function to set a socket to non-blocking mode

#endif // COMMON_H