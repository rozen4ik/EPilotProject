#include "pch.h"
#include "sber_tcp.h"

int open_tcp(WSADATA& wsaData, SOCKET& ConnectSocket, addrinfo* result, addrinfo* ptr, addrinfo hints, PCSTR& ip, PCSTR& port)
{
    //struct addrinfo* result = NULL,
    //                * ptr = NULL,
    //                hints;
    int iResult;
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(ip, port, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    return 0;
}

int read_tcp(SOCKET& ConnectSocket, std::vector<char>& buffer, int sizeBuff)
{
    int iResult;
    char* sendbuf = new char[sizeBuff];
    // Send an initial buffer
    iResult = send(ConnectSocket, sendbuf, sizeBuff, 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }   

    return 0;
}

int write_tcp(SOCKET& server, std::vector<char>& buffer, int sizeBuff)
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

    iResult = send(server, &str[0], str.size(), 0);
    std::cout << "Count byte -> " << iResult << std::endl;
    if (iResult == SOCKET_ERROR)
    {
        std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
        return 1;
    }

    //iResult = 0;
    iResult = shutdown(server, SD_RECEIVE);

    return 0;
}

int close_tcp()
{
    return 0;
}
