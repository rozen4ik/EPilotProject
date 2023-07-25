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
DWORD WINAPI clientReceive(LPVOID lpParam);
DWORD WINAPI clientSend(LPVOID lpParam);
DWORD WINAPI closeTCP(SOCKET& server, HANDLE& t1, HANDLE& t2);