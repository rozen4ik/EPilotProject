#pragma once

#include <vector>
#include "base64.h"
#include "pilot_nt.h"
#include "CommunicationWithSber.h"
#include "cmd_msb.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <inaddr.h>
#include <stdio.h>
#include <vector>
#include <tchar.h>
#include <unordered_map>
#include <fstream>

#pragma comment(lib, "ws2_32.lib")

unsigned short ComputeChecksum(std::vector<unsigned char>& bytes);
void GetFrameWithCrc16(std::vector<unsigned char>& frame);
std::vector<unsigned char> GetBinaryOutData(std::string& outData);
void ioPort(std::string& inData, std::string& outData);
std::string GetIp(std::vector<unsigned char>& response);
int GetPort(std::vector<unsigned char>& response);
void GetSerialNumberMessage(std::vector<unsigned char>& response, std::vector<unsigned char>& serialNumber);
int GetSizeBuff(std::vector<unsigned char>& response);
void GetDataForHost(std::vector<unsigned char>& response, int startIndex, std::vector<char>& inDataTCP);
void GetRowCheck(std::vector<unsigned char>& response, std::vector<unsigned char>& check);
int BodyWorkPilotTrx(auth_answer& auth_answer, std::vector<unsigned char>& response, std::vector<unsigned char>& lastResponsePax, std::string& str, std::unordered_map<std::string, int>& runCardAuth);
int BodyWorkPilotTrx(auth_answer& auth_answer, std::vector<unsigned char>& response, std::vector<unsigned char>& lastResponsePax, std::string& str);
int StartWork(auth_answer& auth_answe, std::vector<unsigned char>& lastResponsePax, std::string& str, std::unordered_map<std::string, int>& runCardAuth);
int StartWork(auth_answer& auth_answe, std::vector<unsigned char>& lastResponsePax, std::string& str);
void GetLastResponsePax(std::vector<unsigned char>& response, std::vector<unsigned char>& lastResponsePax, int startIndex);
void ParsingResponseResCard(std::unordered_map<ResponseRCardContext, std::vector<unsigned char>>& resRecCard, std::vector<unsigned char>& lastResponsePax);
void LoopForParsResponseResCard(std::vector<unsigned char>& buffer, std::vector<unsigned char>& lastResponsePax, int stopIter, int& index);
inline std::string getCurrentDateTime(std::string s);
inline void Logger(std::string logMsg);