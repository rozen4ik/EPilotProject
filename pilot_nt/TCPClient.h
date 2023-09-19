#pragma once

#include <iostream>
#include "pilot_nt.h"
#include "PacketProcessing.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <inaddr.h>
#include <stdio.h>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

class TCPClient
{
private:
	WSADATA wsData;
	SOCKET server;
	SOCKADDR_IN addr;
	LPCWSTR ip;
	int port;

public:
	TCPClient(LPCWSTR& ip, int& port);
	DWORD WINAPI OpenTCP(LPCWSTR& ip, int& port);
	DWORD WINAPI ReadTCP(std::vector<char>& buffer, int sizeBuff);
	DWORD WINAPI WriteTCP(std::vector<char>& buffer, int sizeBuff);
	DWORD WINAPI CloseTCP();
};

