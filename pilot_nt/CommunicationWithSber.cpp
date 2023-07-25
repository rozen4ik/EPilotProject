// Пример простого TCP-клиента
#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include "CommunicationWithSber.h"

DWORD WINAPI openTCP(WSADATA& WSAData, SOCKET& server, SOCKADDR_IN& addr, LPCWSTR& ip, int& port)
{
	WSAStartup(MAKEWORD(2, 0), &WSAData);
	if ((server = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		std::cout << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
		return 1;
	}

	InetPton(AF_INET, ip, &addr.sin_addr.s_addr); //коннект к серверу
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port); //порт
	if (connect(server, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		std::cout << "Server connection failed with error: " << WSAGetLastError() << std::endl;
		return 1;
	}

	std::cout << "Connected to server!" << std::endl;
	std::cout << "Now you can use our live chat application. " << " Enter \"exit\" to disconnect" << std::endl;
	return 0;
}

DWORD WINAPI clientReceive(LPVOID lpParam) 
{ //Получение данных от сервера
	char buffer[1024] = { 0 };
	SOCKET server = *(SOCKET*)lpParam;
	while (true) 
	{
		if (recv(server, buffer, sizeof(buffer), 0) == SOCKET_ERROR) 
		{
			std::cout << "recv function failed with error: " << WSAGetLastError() << std::endl;
			return -1;
		}
		if (strcmp(buffer, "exit\n") == 0) 
		{
			std::cout << "Server disconnected." << std::endl;
			return 1;
		}
		std::cout << "Server: " << buffer << std::endl;
		memset(buffer, 0, sizeof(buffer));
	}
	return 1;
}

DWORD WINAPI clientSend(LPVOID lpParam) 
{ //Отправка данных на сервер
	char buffer[1024] = { 0 };
	SOCKET server = *(SOCKET*)lpParam;
	while (true) 
	{
		fgets(buffer, 1024, stdin);
		if (send(server, buffer, sizeof(buffer), 0) == SOCKET_ERROR) 
		{
			std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
			return -1;
		}
		if (strcmp(buffer, "exit") == 0) 
		{
			std::cout << "Thank you for using the application" << std::endl;
			break;
		}
	}
	return 1;
}

DWORD WINAPI closeTCP(SOCKET& server, HANDLE& t1, HANDLE& t2)
{
	WaitForSingleObject(t1, INFINITE);
	WaitForSingleObject(t2, INFINITE);
	closesocket(server);
	WSACleanup();

	return 0;
}