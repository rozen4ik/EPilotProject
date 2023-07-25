#pragma once

#include "pch.h"
#include "PacketProcessing.h"
#include "com_port.h"
#include <iostream>


unsigned short ComputeChecksum(std::vector<unsigned char>& bytes)
{	
	const unsigned short poly = 0x1021;
	unsigned short table[256];
	unsigned short crc = 0xFFFF;

	unsigned short temp, a;
	for (int i = 0; i < 256; ++i)
	{
		temp = 0;
		a = (unsigned short)(i << 8);
		for (int j = 0; j < 8; ++j)
		{
			if (((temp ^ a) & 0x8000) != 0)
			{
				temp = (unsigned short)((temp << 1) ^ poly);
			}
			else
			{
				temp <<= 1;
			}

			a <<= 1;
		}

		table[i] = temp;
	}

	for (char it : bytes)
	{
		crc = (unsigned short)((crc << 8) ^ table[((crc >> 8) ^ (0xFF & it))]);
	}
	return crc;
}

void GetFrameWithCrc16(std::vector<unsigned char>& frame)
{
	unsigned short crc16 = ComputeChecksum(frame);
	std::cout << "crc16: " << std::hex << crc16 << std::endl;
	frame.push_back((unsigned char)crc16);
	frame.push_back((unsigned char)(crc16 >> 8));	
}

std::vector<unsigned char> GetBinaryOutData(std::string& outData)
{
	std::erase(outData, '\u0002');
	std::erase(outData, '\u0003');
	std::erase(outData, '\u0004');
	std::erase(outData, '#');

	return base64_decode(outData);
}

void ioPort(std::string& inData, std::string& outData)
{
	HANDLE hSerialPort;	
	open_port(&hSerialPort);
	write_port(&hSerialPort, &inData[0], inData.length());
	read_port(&hSerialPort, outData);
	std::cout << outData << std::endl;
	close_port(&hSerialPort);
}

std::string GetIp(std::vector<unsigned char>& response)
{
	std::string ip = "";
	for (int i = 16; i < 20; i++)
	{
		ip += std::to_string(response[i]);
		ip += ".";
	}

	ip.erase(ip.length() - 1, 1);

	std::cout << ip << std::endl;

	return ip;
}

int GetPort(std::vector<unsigned char>& response)
{
	//std::string port = std::to_string(response[21]) + std::to_string(response[20]);
	unsigned char arrPort[] = {response[20], response[21]};
	int port = *((unsigned short*)arrPort);
	std::cout << std::dec << port << std::endl;
	return port;
}

void GetSerialNumberMessage(std::vector<unsigned char>& response, std::vector<unsigned char>& serialNumber)
{
	for (int i = 5; i < 9; i++)
	{
		serialNumber.push_back(response[i]);		
	}

	serialNumber[3] = 0x80;
}

void BodyWorkPilotTrx(auth_answer& auth_answer, std::vector<unsigned char>& response)
{
	int codeInstruction = response[9];
	int codeDevice = response[10];
	bool loop = true;
	WSADATA WSAData;
	SOCKET server;
	SOCKADDR_IN addr;
	DWORD tid;
	HANDLE t1, t2;
	std::string ipString = GetIp(response);
	std::wstring stemp = std::wstring(ipString.begin(), ipString.end());
	LPCWSTR ip = stemp.c_str();
	int port = GetPort(response);
	std::vector<unsigned char> frame, serialNumber;
	std::string outData = "";
	std::string resFrame = "";
	
	while (loop)
	{
		switch (codeInstruction)
		{
		case 1:
			if (codeDevice == 25)
			{				
				//openTCP(WSAData, server, addr, ip, port);

				//closeTCP(server, t1, t2);

				GetSerialNumberMessage(response, serialNumber);
				frame = GetFrameNewHostMasterCall(serialNumber, 25);
				GetFrameWithCrc16(frame);

				resFrame = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				ioPort(resFrame, outData);
				response = GetBinaryOutData(outData);

				frame.clear();
				serialNumber.clear();
				outData = "";
				resFrame = "";
			}
			else if (codeDevice == 3)
			{
				GetSerialNumberMessage(response, serialNumber);
				frame = GetFrameNewHostMasterCall(serialNumber, 3);
				GetFrameWithCrc16(frame);

				resFrame = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				ioPort(resFrame, outData);
				response = GetBinaryOutData(outData);

				frame.clear();
				serialNumber.clear();
				outData = "";
				resFrame = "";
			}
			break;
		case 2:
			if (codeDevice == 25)
			{
				t1 = CreateThread(NULL, 0, clientReceive, &server, 0, &tid);
				if (t1 == NULL) std::cout << "Thread creation error: " << GetLastError();
			}
			else if (codeDevice == 39)
			{
				//
			}
			break;
		case 3:
			if (codeDevice == 25)
			{
				t2 = CreateThread(NULL, 0, clientSend, &server, 0, &tid);
				if (t2 == NULL) std::cout << "Thread creation error: " << GetLastError();
			}
			else if (codeDevice == 3)
			{
				//
			}
			break;
		case 4:
			if (codeDevice == 25)
			{
				closeTCP(server, t1, t2);
				//
			}
			else if (codeDevice == 3)
			{
				//
			}
			break;
		default:
			loop = false;
			break;
		}
	}
}
