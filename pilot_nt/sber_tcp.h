#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <stdio.h>
#include <string>
#include <iostream>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

int open_tcp(WSADATA& wsaData, SOCKET& ConnectSocket, addrinfo* result, addrinfo* ptr, addrinfo hints, PCSTR& ip, PCSTR& port);
int read_tcp(SOCKET& ConnectSocket, std::vector<char>& buffer, int sizeBuff);
int write_tcp(SOCKET& server, std::vector<char>& buffer, int sizeBuff);
int close_tcp();