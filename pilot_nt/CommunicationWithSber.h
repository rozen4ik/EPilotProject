#pragma once

#include <iostream>
#include "pilot_nt.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <inaddr.h>
#include <stdio.h>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

DWORD WINAPI openTCP(WSADATA& WSAData, SOCKET& server, SOCKADDR_IN& addr, LPCWSTR& ip, int& port);
DWORD WINAPI readTCP(SOCKET& server, std::vector<char>& buffer, int sizeBuff);
DWORD WINAPI writeTCP(SOCKET& server, std::vector<char>& buffer, int sizeBuff);
DWORD WINAPI closeTCP(SOCKET& server);