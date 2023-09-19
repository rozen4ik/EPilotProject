#include "pch.h"
#include "TCPClient.h"

TCPClient::TCPClient(LPCWSTR& ip, int& port)
{
	WSAStartup(MAKEWORD(2, 0), &wsData);
	server = socket(AF_INET, SOCK_STREAM, 0);
	Logger("Значение сервера: " + server);
	if (server == INVALID_SOCKET)
	{
		Logger("Ошибка при создании сокета: " + WSAGetLastError());		
		return;
	}
}

DWORD TCPClient::OpenTCP(LPCWSTR& ip, int& port)
{
	this->ip = ip;
	this->port = port;
	InetPton(AF_INET, this->ip, &addr.sin_addr.s_addr); //коннект к серверу
	addr.sin_family = AF_INET;
	addr.sin_port = htons(this->port); //порт
	int iResult = connect(server, (SOCKADDR*)&addr, sizeof(addr));

	if (iResult == SOCKET_ERROR)
	{
		Logger("Ошибка при подключении сервера: " + WSAGetLastError());
		return 1;
	}

	Logger("Успешное подключение к серверу!");
	return 0;
}

DWORD TCPClient::ReadTCP(std::vector<char>& buffer, int sizeBuff)
{
	int iResult;
	char* buff = new char[sizeBuff];

	iResult = recv(server, buff, sizeBuff, 0);
	if (iResult == SOCKET_ERROR)
	{
		Logger("Ошибка при чтении данных: " + WSAGetLastError());
		return 1;
	}
	if (iResult == 0)
	{
		Logger("Отключение от сервера.");
		return 1;
	}

	iResult = 0;

	for (int i = 0; i < sizeBuff; i++)
	{
		if (buff[i] == 0xFFFFFFCD || buff[i] == 0)
			break;
		buffer.push_back(buff[i]);
	}

	memset(buff, 0, sizeBuff);
	delete[] buff;
	buff = nullptr;

	return 0;
}

DWORD TCPClient::WriteTCP(std::vector<char>& buffer, int sizeBuff)
{
	int iResult;
	std::string str(buffer.begin(), buffer.end());
	Logger("Size -> " + str.size());
	Logger("-> " + str);

	iResult = send(server, &str[0], str.size(), 0);
	Logger("Кол-во байт -> " + iResult);
	if (iResult == SOCKET_ERROR)
	{
		Logger("Ошибка при записи: " + WSAGetLastError());
		return 1;
	}

	iResult = 0;

	return 0;
}

DWORD TCPClient::CloseTCP()
{
	shutdown(server, SD_BOTH);
	closesocket(server);
	WSACleanup();
	Logger("Отключение от сервера.");

	return 0;
}
