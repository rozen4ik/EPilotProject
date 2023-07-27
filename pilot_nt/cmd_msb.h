#pragma once

#include <vector>
#include "pilot_nt.h"

std::vector<unsigned char> GetFrameGetReady(unsigned short function_id);
std::vector<unsigned char> GetFrameTrx(auth_answer& auth_answer);
std::vector<unsigned char> GetFrameNewHostMasterCall(std::vector<unsigned char>& serialNumber, unsigned char TType);
std::vector<unsigned char> GetFrameReadTCP(std::vector<unsigned char>& serialNumber, std::vector<char>& outDataTCP);
std::vector<unsigned char> GetFrameWriteTCP(std::vector<unsigned char>& serialNumber, int sizeData);
std::vector<unsigned char> GetFrameCloseTCP(std::vector<unsigned char>& serialNumber);
std::vector<unsigned char> GetFramePingInfo(std::vector<unsigned char>& serialNumber);
std::vector<unsigned char> GetFramePingInfoTwoMessage();