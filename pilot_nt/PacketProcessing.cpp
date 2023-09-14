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