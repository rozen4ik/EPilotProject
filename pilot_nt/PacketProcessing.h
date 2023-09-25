#pragma once

#include <vector>
#include "pilot_nt.h"
#include <tchar.h>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <cassert>
#include "ComPort.h"
#include "TCPClient.h"

int GetSizeBuff(const std::vector<unsigned char>& response);
void GetDataForHost(const std::vector<unsigned char>& response, int startIndex, std::vector<char>& inDataTCP);
void GetRowCheck(const std::vector<unsigned char>& response, std::vector<unsigned char>& check);
void ParsingResponseResCard(std::unordered_map<ResponseRCardContext, std::vector<unsigned char>>& resRecCard, const std::vector<unsigned char>& lastResponsePax);
void LoopForParsResponseResCard(std::vector<unsigned char>& buffer, const std::vector<unsigned char>& lastResponsePax, int stopIter, int& index);
inline std::string getCurrentDateTime(std::string s);
inline void Logger(std::string logMsg);
HMODULE GetThisDllHandle();