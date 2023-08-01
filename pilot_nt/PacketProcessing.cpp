#pragma once

#include "pch.h"
#include "PacketProcessing.h"
#include "com_port.h"
#include <iostream>
#include <chrono>
#include <thread>


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
	std::cout << "Отправлено в порт" << std::endl;
	std::cout << inData << std::endl;
	write_port(&hSerialPort, &inData[0], inData.length());
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	read_port(&hSerialPort, outData);
	std::cout << "Получено из порта" << std::endl;
	std::cout << outData << std::endl;
	std::cout << std::endl;
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
	unsigned char arrPort[] = { response[20], response[21] };
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

int GetSizeBuff(std::vector<unsigned char>& response)
{
	unsigned char arrSizeBuff[] = { response[14], response[15] };
	int sizeBuff = *((unsigned short*)arrSizeBuff);
	std::cout << "Размер буфера: " << std::dec << sizeBuff << " байт" << std::endl;
	return sizeBuff;
}

void GetDataForHost(std::vector<unsigned char>& response, int startIndex, std::vector<char>& inDataTCP)
{
	for (int i = startIndex; i < response.size() - 2; i++)
	{
		inDataTCP.push_back(response[i]);
	}
}

void GetRowCheck(std::vector<unsigned char>& response, std::vector<unsigned char>& check)
{
	for (int i = 15; i < response.size() - 2; i++)
	{
		check.push_back(response[i]);
	}
}

std::string cp866_to_utf8(const char* str) 
{
	std::string res;
	int result_u, result_c;
	result_u = MultiByteToWideChar(866, 0, str, -1, 0, 0);
	if (!result_u) { return 0; }
	wchar_t* ures = new wchar_t[result_u];
	if (!MultiByteToWideChar(866, 0, str, -1, ures, result_u)) 
	{
		delete[] ures;
		return 0;
	}
	result_c = WideCharToMultiByte(65001, 0, ures, -1, 0, 0, 0, 0);
	if (!result_c) 
	{
		delete[] ures;
		return 0;
	}
	char* cres = new char[result_c];
	if (!WideCharToMultiByte(65001, 0, ures, -1, cres, result_c, 0, 0)) 
	{
		delete[] cres;
		return 0;
	}
	delete[] ures;
	res.append(cres);
	delete[] cres;
	return res;
}

void BodyWorkPilotTrx(auth_answer& auth_answer, std::vector<unsigned char>& response, std::vector<unsigned char>& lastResponsePax)
{
	int codeInstruction;
	int codeDevice;
	bool loop = true;
	WSADATA WSAData;
	SOCKET server;
	SOCKADDR_IN addr;
	std::string ipString = GetIp(response);
	std::wstring stemp = std::wstring(ipString.begin(), ipString.end());
	LPCWSTR ip = stemp.c_str();
	int port = GetPort(response);
	std::vector<unsigned char> frame, serialNumber;
	std::string outDataPax = "";
	std::string resFramePax = "";
	std::vector<char> outDataTCP;
	std::vector<char> inDataTCP;
	std::vector<unsigned char> check;
	
	openTCP(WSAData, server, addr, ip, port);

	while (loop)
	{
		codeInstruction = response[9];
		codeDevice = response[10];
		switch (codeInstruction)
		{
		case 1:
			if (codeDevice == 25)
			{
				//openTCP(WSAData, server, addr, ip, port);
				GetSerialNumberMessage(response, serialNumber);
				frame = GetFrameNewHostMasterCall(serialNumber, 25);
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				ioPort(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			else if (codeDevice == 3)
			{
				GetSerialNumberMessage(response, serialNumber);
				frame = GetFrameNewHostMasterCall(serialNumber, 3);
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				ioPort(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			break;
		case 2:
			if (codeDevice == 25)
			{
				outDataTCP.clear();
				std::cout << "Чтение данных TCP" << std::endl;
				readTCP(server, outDataTCP, GetSizeBuff(response), addr, ip, port);

				for (char c : outDataTCP)
				{
					std::cout << std::hex << std::uppercase << int(c) << " ";
				}
				std::cout << std::endl;

				GetSerialNumberMessage(response, serialNumber);
				frame = GetFrameReadTCPMasterCall(serialNumber, outDataTCP);
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				ioPort(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			else if (codeDevice == 39)
			{
				GetSerialNumberMessage(response, serialNumber);
				frame = GetFramePingInfoMasterCall(serialNumber);
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				HANDLE hSerialPort;
				open_port(&hSerialPort);
				std::cout << "Запись данных ComPort" << std::endl;
				std::cout << resFramePax << std::endl;
				write_port(&hSerialPort, &resFramePax[0], resFramePax.length());
				close_port(&hSerialPort);

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();

				frame = GetFramePingInfoTwoMessageMasterCall();
				GetFrameWithCrc16(frame);

				resFramePax = "\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				ioPort(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			break;
		case 3:
			if (codeDevice == 25)
			{
				while (true)
				{
					if (response[0] == 0)
					{
						GetSerialNumberMessage(response, serialNumber);
						GetDataForHost(response, 14, inDataTCP);
						break;
					}
					else if (response[0] == 128)
					{
						GetSerialNumberMessage(response, serialNumber);
						GetDataForHost(response, 14, inDataTCP);

						resFramePax = "\u0006";

						ioPort(resFramePax, outDataPax);
						response = GetBinaryOutData(outDataPax);

						frame.clear();
						outDataPax = "";
						resFramePax = "";

						GetDataForHost(response, 2, inDataTCP);

						resFramePax = "\u0007";

						ioPort(resFramePax, outDataPax);
						response = GetBinaryOutData(outDataPax);

						frame.clear();
						outDataPax = "";
						resFramePax = "";
					}
					else
					{
						GetDataForHost(response, 2, inDataTCP);
						break;
					}
				}

				std::cout << "Запись данных TCP" << std::endl;
				writeTCP(server, inDataTCP, inDataTCP.size());

				frame = GetFrameWriteTCPMasterCall(serialNumber, inDataTCP.size());
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				ioPort(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			else if (codeDevice == 3)
			{
				GetSerialNumberMessage(response, serialNumber);
				frame = GetFrameWriteToCheckMasterCall(serialNumber);
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				ioPort(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);

				GetRowCheck(response, check);

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			break;
		case 4:
			if (codeDevice == 25)
			{
				//closeTCP(server);

				GetSerialNumberMessage(response, serialNumber);
				frame = GetFrameCloseTCPMasterCall(serialNumber);
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				ioPort(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			else if (codeDevice == 3)
			{
				GetSerialNumberMessage(response, serialNumber);
				frame = GetFraneCloseToCheckMasterCall(serialNumber);
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				ioPort(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			break;
		default:
			loop = false;
			GetLastResponsePax(response, lastResponsePax, 9);

			resFramePax = "\u0006";

			ioPort(resFramePax, outDataPax);
			response = GetBinaryOutData(outDataPax);

			while (true)
			{
				if (response[0] == 128)
				{
					GetLastResponsePax(response, lastResponsePax, 9);

					resFramePax = "\u0006";

					ioPort(resFramePax, outDataPax);
					response = GetBinaryOutData(outDataPax);

					frame.clear();
					outDataPax = "";
					resFramePax = "";

					GetLastResponsePax(response, lastResponsePax, 2);

					resFramePax = "\u0007";

					ioPort(resFramePax, outDataPax);
					response = GetBinaryOutData(outDataPax);

					frame.clear();
					outDataPax = "";
					resFramePax = "";
				}
				else
				{
					GetLastResponsePax(response, lastResponsePax, 2);
					break;
				}
			}
			break;
		}
	}

	std::string strCheck(check.begin(), check.end());
	check.clear();

	std::cout << "Чек об операции:" << std::endl;
	std::cout << strCheck << std::endl;

	auth_answer.Check = &strCheck[0];
}

void StartWork(auth_answer& auth_answe, std::vector<unsigned char>& lastResponsePax)
{
	std::string outData = "";
	std::vector<unsigned char> frame = GetFrameTrx(auth_answe);

	GetFrameWithCrc16(frame);

	std::string resFrame = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

	ioPort(resFrame, outData);

	std::vector<unsigned char> response = GetBinaryOutData(outData);
	BodyWorkPilotTrx(auth_answe, response, lastResponsePax);

	outData = "";
	response.clear();
}

void GetLastResponsePax(std::vector<unsigned char>& response, std::vector<unsigned char>& lastResponsePax, int startIndex)
{
	for (int i = startIndex; i < response.size() - 2; i++)
		lastResponsePax.push_back(response[i]);
}
