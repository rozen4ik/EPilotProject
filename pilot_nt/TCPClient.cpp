#include "pch.h"
#include "TCPClient.h"

TCPClient::TCPClient(LPCWSTR& ip, int& port)
{
	WSAStartup(MAKEWORD(2, 0), &wsData);
	server = socket(AF_INET, SOCK_STREAM, 0);
	Logger("�������� �������: " + server);
	if (server == INVALID_SOCKET)
	{
		Logger("������ ��� �������� ������: " + WSAGetLastError());		
		return;
	}
}

DWORD TCPClient::OpenTCP(LPCWSTR& ip, int& port)
{
	this->ip = ip;
	this->port = port;
	InetPton(AF_INET, this->ip, &addr.sin_addr.s_addr); //������� � �������
	addr.sin_family = AF_INET;
	addr.sin_port = htons(this->port); //����
	int iResult = connect(server, (SOCKADDR*)&addr, sizeof(addr));

	if (iResult == SOCKET_ERROR)
	{
		Logger("������ ��� ����������� �������: " + WSAGetLastError());
		return 1;
	}

	Logger("�������� ����������� � �������!");
	return 0;
}

DWORD TCPClient::ReadTCP(std::vector<char>& buffer, int sizeBuff)
{
	int iResult;
	char* buff = new char[sizeBuff];

	iResult = recv(server, buff, sizeBuff, 0);
	if (iResult == SOCKET_ERROR)
	{
		Logger("������ ��� ������ ������: " + WSAGetLastError());
		return 1;
	}
	if (iResult == 0)
	{
		Logger("���������� �� �������.");
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
	Logger("���-�� ���� -> " + iResult);
	if (iResult == SOCKET_ERROR)
	{
		Logger("������ ��� ������: " + WSAGetLastError());
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
	Logger("���������� �� �������.");

	return 0;
}
