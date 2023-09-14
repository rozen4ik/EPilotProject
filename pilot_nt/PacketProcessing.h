#pragma once

#include <vector>
#include "base64.h"
#include "pilot_nt.h"
#include "cmd_msb.h"
//#include <stdio.h>
#include <vector>
#include <tchar.h>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <cassert>
#include "ComPort.h"
#include "TCPClient.h"

unsigned short ComputeChecksum(std::vector<unsigned char>& bytes);
void GetFrameWithCrc16(std::vector<unsigned char>& frame);
std::vector<unsigned char> GetBinaryOutData(std::string& outData);
std::string GetIp(std::vector<unsigned char>& response);
int GetPort(std::vector<unsigned char>& response);
void GetSerialNumberMessage(std::vector<unsigned char>& response, std::vector<unsigned char>& serialNumber);
int GetSizeBuff(std::vector<unsigned char>& response);
void GetDataForHost(std::vector<unsigned char>& response, int startIndex, std::vector<char>& inDataTCP);
void GetRowCheck(std::vector<unsigned char>& response, std::vector<unsigned char>& check);
void GetLastResponsePax(std::vector<unsigned char>& response, std::vector<unsigned char>& lastResponsePax, int startIndex);
void ParsingResponseResCard(std::unordered_map<ResponseRCardContext, std::vector<unsigned char>>& resRecCard, std::vector<unsigned char>& lastResponsePax);
void LoopForParsResponseResCard(std::vector<unsigned char>& buffer, std::vector<unsigned char>& lastResponsePax, int stopIter, int& index);
inline std::string getCurrentDateTime(std::string s);
inline void Logger(std::string logMsg);
HMODULE GetThisDllHandle();