// ������ �������� TCP-�������
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

	InetPton(AF_INET, ip, &addr.sin_addr.s_addr); //������� � �������
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port); //����
	if (connect(server, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		std::cout << "Server connection failed with error: " << WSAGetLastError() << std::endl;
		return 1;
	}

	std::cout << "Connected to server!" << std::endl;
	return 0;
}

//��������� ������ �� �������
DWORD WINAPI readTCP(SOCKET& server, std::vector<char>& buffer, int sizeBuff) 
{ 
	int iResult;
	char* buff = new char[sizeBuff];

	iResult = recv(server, buff, sizeBuff, 0);
	if (iResult == SOCKET_ERROR)
	{
		std::cout << "recv function failed with error: " << WSAGetLastError() << std::endl;
		return 1;
	}
	if (iResult == 0)
	{
		std::cout << "Server disconnected." << std::endl;
		return 1;
	}

	iResult = 0;
	//iResult = shutdown(server, SD_SEND);

	for (int i = 0; i < sizeBuff; i++)
	{
		if (buff[i] == 0xFFFFFFCD)
			break;
		buffer.push_back(buff[i]);
	}

	memset(buff, 0, sizeBuff);
	delete [] buff;
	buff = nullptr;

	return 0;
}

//�������� ������ �� ������
DWORD WINAPI writeTCP(SOCKET& server, std::vector<char>& buffer, int sizeBuff)
{ 
	int iResult;
	std::string str(buffer.begin(), buffer.end());
	//char *buff = &str[0];
	std::cout << "Size -> " << str.size() << std::endl;
	std::cout << "-> " << str << std::endl;
	for (char c : buffer)
	{
		std::cout << std::hex << std::uppercase << int(c) << " ";
	}
	std::cout << std::endl;

	iResult = send(server, &str[0], (int) strlen(&str[0]), 0);
	std::cout << "Count byte -> " << iResult << std::endl;
	if (iResult == SOCKET_ERROR)
	{
		std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
		return 1;
	}

	iResult = 0;
	//iResult = shutdown(server, SD_SEND);

	return 0;
}

DWORD WINAPI closeTCP(SOCKET& server)
{
	closesocket(server);
	WSACleanup();

	std::cout << "Server disconnected." << std::endl;

	return 0;
}