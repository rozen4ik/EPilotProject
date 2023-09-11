#pragma once

#include "pch.h"
#include "PacketProcessing.h"

ComPort com_port;

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
	Logger("Размер буфера: " + sizeBuff);
	//std::cout << "Размер буфера: " << std::dec << sizeBuff << " байт" << std::endl;
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
	for (int i = 15; i < response.size() - 3; i++)
	{
		check.push_back(response[i]);
	}
}

int BodyWorkPilotTrx(auth_answer& auth_answer, std::vector<unsigned char>& response, std::vector<unsigned char>& lastResponsePax, std::string& str, std::unordered_map<std::string, int>& runCardAuth)
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
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	while (loop)
	{
		if (runCardAuth["cardAuth15"] == 0) return 2000;
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

				com_port.io_port(resFramePax, outDataPax);
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

				com_port.io_port(resFramePax, outDataPax);
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
				Logger("Чтение данных TCP");
				//std::cout << "Чтение данных TCP" << std::endl;
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

				com_port.io_port(resFramePax, outDataPax);
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

				com_port.open_port();
				Logger("-> " + resFramePax);
				com_port.write_port(&resFramePax[0], resFramePax.length());
				com_port.close_port();

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();

				frame = GetFramePingInfoTwoMessageMasterCall();
				GetFrameWithCrc16(frame);

				resFramePax = "\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				com_port.io_port(resFramePax, outDataPax);
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

						com_port.io_port(resFramePax, outDataPax);
						response = GetBinaryOutData(outDataPax);

						frame.clear();
						outDataPax = "";
						resFramePax = "";

						GetDataForHost(response, 2, inDataTCP);

						resFramePax = "\u0007";

						com_port.io_port(resFramePax, outDataPax);
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

				Logger("Запись данных TCP");
				//std::cout << "Запись данных TCP" << std::endl;
				writeTCP(server, inDataTCP, inDataTCP.size());

				frame = GetFrameWriteTCPMasterCall(serialNumber, inDataTCP.size());
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				com_port.io_port(resFramePax, outDataPax);
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
				GetRowCheck(response, check);

				GetSerialNumberMessage(response, serialNumber);
				frame = GetFrameWriteToCheckMasterCall(serialNumber);
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				com_port.io_port(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);				

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

				com_port.io_port(resFramePax, outDataPax);
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

				com_port.io_port(resFramePax, outDataPax);
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

			com_port.io_port(resFramePax, outDataPax);
			response = GetBinaryOutData(outDataPax);

			while (true)
			{
				if (response[0] == 128)
				{
					GetLastResponsePax(response, lastResponsePax, 9);

					resFramePax = "\u0006";

					com_port.io_port(resFramePax, outDataPax);
					response = GetBinaryOutData(outDataPax);

					frame.clear();
					outDataPax = "";
					resFramePax = "";

					GetLastResponsePax(response, lastResponsePax, 2);

					resFramePax = "\u0007";

					com_port.io_port(resFramePax, outDataPax);
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

	check.push_back('\0');
	std::string strCheck(check.begin(), check.end());
	check.clear();
	str = strCheck;

	return 0;
}

int BodyWorkPilotTrx(auth_answer& auth_answer, std::vector<unsigned char>& response, std::vector<unsigned char>& lastResponsePax, std::string& str)
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

				com_port.io_port(resFramePax, outDataPax);
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

				com_port.io_port(resFramePax, outDataPax);
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
				Logger("Чтение данных TCP");
				//std::cout << "Чтение данных TCP" << std::endl;
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

				com_port.io_port(resFramePax, outDataPax);
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


				com_port.open_port();
				Logger("-> " + resFramePax);
				com_port.write_port(&resFramePax[0], resFramePax.length());
				com_port.close_port();

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();

				frame = GetFramePingInfoTwoMessageMasterCall();
				GetFrameWithCrc16(frame);

				resFramePax = "\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				com_port.io_port(resFramePax, outDataPax);
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

						com_port.io_port(resFramePax, outDataPax);
						response = GetBinaryOutData(outDataPax);

						frame.clear();
						outDataPax = "";
						resFramePax = "";

						GetDataForHost(response, 2, inDataTCP);

						resFramePax = "\u0007";

						com_port.io_port(resFramePax, outDataPax);
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

				Logger("Запись данных TCP");
				//std::cout << "Запись данных TCP" << std::endl;
				writeTCP(server, inDataTCP, inDataTCP.size());

				frame = GetFrameWriteTCPMasterCall(serialNumber, inDataTCP.size());
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				com_port.io_port(resFramePax, outDataPax);
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
				GetRowCheck(response, check);

				GetSerialNumberMessage(response, serialNumber);
				frame = GetFrameWriteToCheckMasterCall(serialNumber);
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				com_port.io_port(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);
				
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

				com_port.io_port(resFramePax, outDataPax);
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

				com_port.io_port(resFramePax, outDataPax);
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

			com_port.io_port(resFramePax, outDataPax);
			response = GetBinaryOutData(outDataPax);

			while (true)
			{
				if (response[0] == 128)
				{
					GetLastResponsePax(response, lastResponsePax, 9);

					resFramePax = "\u0006";

					com_port.io_port(resFramePax, outDataPax);
					response = GetBinaryOutData(outDataPax);

					frame.clear();
					outDataPax = "";
					resFramePax = "";

					GetLastResponsePax(response, lastResponsePax, 2);

					resFramePax = "\u0007";

					com_port.io_port(resFramePax, outDataPax);
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

	check.push_back('\0');
	std::string strCheck(check.begin(), check.end());
	check.clear();
	str = strCheck;

	return 0;
}

int StartWork(auth_answer& auth_answe, std::vector<unsigned char>& lastResponsePax, std::string& str, std::unordered_map<std::string, int>& runCardAuth)
{
	std::cout << "2) PilotServce::close_day SizeOf argument: " << sizeof(auth_answe) << std::endl;
	std::cout << "2) PilotServce::close_day SizeOf ammessage: " << sizeof(auth_answe.AMessage) << std::endl;
	std::string outData = "";
	std::vector<unsigned char> frame = GetFrameTrx(auth_answe);
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	GetFrameWithCrc16(frame);
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	std::string resFrame = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

	com_port.io_port(resFrame, outData);
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	std::vector<unsigned char> response = GetBinaryOutData(outData);
	if (runCardAuth["cardAuth15"] == 0) return 2000;
	BodyWorkPilotTrx(auth_answe, response, lastResponsePax, str, runCardAuth);
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	outData = "";
	response.clear();
	return 0;
}

int StartWork(auth_answer& auth_answe, std::vector<unsigned char>& lastResponsePax, std::string& str)
{
	std::cout << "2) PilotServce::close_day SizeOf argument: " << sizeof(auth_answe) << std::endl;
	std::cout << "2) PilotServce::close_day SizeOf ammessage: " << sizeof(auth_answe.AMessage) << std::endl;
	std::string outData = "";
	std::vector<unsigned char> frame = GetFrameTrx(auth_answe);

	GetFrameWithCrc16(frame);

	std::string resFrame = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

	com_port.io_port(resFrame, outData);

	std::vector<unsigned char> response = GetBinaryOutData(outData);
	BodyWorkPilotTrx(auth_answe, response, lastResponsePax, str);

	outData = "";
	response.clear();
	return 0;
}

int StartWork(auth_answer14& auth_answe, std::vector<unsigned char>& lastResponsePax, std::string& str, std::unordered_map<std::string, int>& runCardAuth)
{
	std::cout << "2) PilotServce::close_day SizeOf argument: " << sizeof(auth_answe.ans) << std::endl;
	std::cout << "2) PilotServce::close_day SizeOf ammessage: " << sizeof(auth_answe.ans.AMessage) << std::endl;
	std::string outData = "";
	std::vector<unsigned char> frame = GetFrameTrx(auth_answe);
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	GetFrameWithCrc16(frame);
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	std::string resFrame = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

	com_port.io_port(resFrame, outData);
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	std::vector<unsigned char> response = GetBinaryOutData(outData);
	if (runCardAuth["cardAuth15"] == 0) return 2000;
	BodyWorkPilotTrx(auth_answe.ans, response, lastResponsePax, str, runCardAuth);
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	outData = "";
	response.clear();
	return 0;
}

int StartWork(auth_answer14& auth_answe, std::vector<unsigned char>& lastResponsePax, std::string& str)
{
	std::cout << "2) PilotServce::close_day SizeOf argument: " << sizeof(auth_answe.ans) << std::endl;
	std::cout << "2) PilotServce::close_day SizeOf ammessage: " << sizeof(auth_answe.ans.AMessage) << std::endl;
	std::string outData = "";
	std::vector<unsigned char> frame = GetFrameTrx(auth_answe);

	GetFrameWithCrc16(frame);

	std::string resFrame = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

	com_port.io_port(resFrame, outData);

	std::vector<unsigned char> response = GetBinaryOutData(outData);
	BodyWorkPilotTrx(auth_answe.ans, response, lastResponsePax, str);

	outData = "";
	response.clear();
	return 0;
}

void GetLastResponsePax(std::vector<unsigned char>& response, std::vector<unsigned char>& lastResponsePax, int startIndex)
{
	for (int i = startIndex; i < response.size() - 2; i++)
		lastResponsePax.push_back(response[i]);
}

void ParsingResponseResCard(std::unordered_map<ResponseRCardContext, std::vector<unsigned char>>& resRecCard, std::vector<unsigned char>& lastResponsePax)
{
	int index = 0;

	std::vector<unsigned char> buffer;

	std::cout << "\nresRecCard.ErrorCode" << std::endl;
	LoopForParsResponseResCard(buffer, lastResponsePax, 2, index);
	resRecCard[ERROR_CODE] = buffer;
	buffer.clear();

	std::cout << "\nresRecCard.CodeAuth" << std::endl;
	LoopForParsResponseResCard(buffer, lastResponsePax, 7, index);
	resRecCard[CODE_AUTH] = buffer;
	buffer.clear();

	std::cout << "\nresRecCard.RRN" << std::endl;
	LoopForParsResponseResCard(buffer, lastResponsePax, 13, index);
	resRecCard[RRN] = buffer;
	buffer.clear();

	std::cout << "\nresRecCard.NumberOperationDay" << std::endl;
	LoopForParsResponseResCard(buffer, lastResponsePax, 5, index);
	resRecCard[NUMBER_OPERATION_DAY] = buffer;
	buffer.clear();

	std::cout << "\nresRecCard.NumberCard" << std::endl;
	LoopForParsResponseResCard(buffer, lastResponsePax, 20, index);
	resRecCard[NUMBER_CARD] = buffer;
	buffer.clear();

	std::cout << "\nresRecCard.CardDate" << std::endl;
	LoopForParsResponseResCard(buffer, lastResponsePax, 6, index);
	resRecCard[CARD_DATE] = buffer;
	buffer.clear();

	std::cout << "\nresRecCard.TextMessageError" << std::endl;
	LoopForParsResponseResCard(buffer, lastResponsePax, 32, index);
	resRecCard[TEXT_MESSAGE_ERROR] = buffer;
	buffer.clear();

	std::cout << "\nresRecCard.DateOperation" << std::endl;
	LoopForParsResponseResCard(buffer, lastResponsePax, 4, index);
	resRecCard[DATE_OPERATION] = buffer;
	buffer.clear();

	std::cout << "\nresRecCard.TimeOperation" << std::endl;
	LoopForParsResponseResCard(buffer, lastResponsePax, 4, index);
	resRecCard[TIME_OPERATION] = buffer;
	buffer.clear();

	std::cout << "\nresRecCard.BankAffiliation" << std::endl;
	buffer.push_back(lastResponsePax[index]);
	std::cout << std::hex << std::uppercase << int(lastResponsePax[index]) << " ";
	index++;
	resRecCard[BANK_AFFILIATION] = buffer;
	buffer.clear();

	std::cout << "\nresRecCard.NumberTerminal" << std::endl;
	LoopForParsResponseResCard(buffer, lastResponsePax, 9, index);
	resRecCard[NUMBER_TERMINAL] = buffer;
	buffer.clear();

	std::cout << "\nresRecCard.NameCard" << std::endl;
	LoopForParsResponseResCard(buffer, lastResponsePax, 32, index);
	resRecCard[NAME_CARD] = buffer;
	buffer.clear();

	std::cout << "\nresRecCard.HashCard" << std::endl;
	LoopForParsResponseResCard(buffer, lastResponsePax, 20, index);
	resRecCard[HASH_CARD] = buffer;
	buffer.clear();

	std::cout << "\nresRecCard.EncryptedData" << std::endl;
	LoopForParsResponseResCard(buffer, lastResponsePax, 32, index);
	resRecCard[ECRYPTED_DATA] = buffer;
	buffer.clear();

	std::cout << "\nresRecCard.CardID" << std::endl;
	buffer.push_back(lastResponsePax[index]);
	std::cout << std::hex << std::uppercase << int(lastResponsePax[index]) << " ";
	resRecCard[CARD_ID] = buffer;
	buffer.clear();
	std::cout << std::endl;
}

void LoopForParsResponseResCard(std::vector<unsigned char>& buffer, std::vector<unsigned char>& lastResponsePax, int stopIter, int& index)
{
	for (int i = 0; i < stopIter; i++)
	{
		buffer.push_back(lastResponsePax[index]);
		std::cout << std::hex << std::uppercase << int(lastResponsePax[index]) << " ";
		index++;
	}
}

inline std::string getCurrentDateTime(std::string s)
{
	time_t now = time(0);
	struct tm  tstruct;
	char  buf[80];
	tstruct = *localtime(&now);
	if (s == "now")
		strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
	else if (s == "date")
		strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
	return std::string(buf);
}

inline void Logger(std::string logMsg)
{
	wchar_t wPathToDll[MAX_PATH];

	GetModuleFileName(GetThisDllHandle(), wPathToDll, MAX_PATH);

	std::wstring wStringPathToDll = wPathToDll;
	std::string pathToDll(wStringPathToDll.begin(), wStringPathToDll.end());
	std::string del = "pilot_nt.dll";
	std::string::size_type pos = pathToDll.find("pilot_nt.dll");
	pathToDll.erase(pos, del.size());
	std::string rusPathToDll = pathToDll;
	OemToCharBuffA(pathToDll.c_str(), &rusPathToDll[0], pathToDll.size());


	rusPathToDll += "log_" + getCurrentDateTime("date") + ".txt";
	std::string now = getCurrentDateTime("now");
	std::ofstream ofs(rusPathToDll.c_str(), std::ios_base::out | std::ios_base::app);
	ofs << now << '\t' << logMsg << '\n';
	ofs.close();
}

HMODULE GetThisDllHandle()
{
	MEMORY_BASIC_INFORMATION info;
	size_t len = VirtualQueryEx(GetCurrentProcess(), (void*)GetThisDllHandle, &info, sizeof(info));
	assert(len == sizeof(info));
	return len ? (HMODULE)info.AllocationBase : NULL;
}