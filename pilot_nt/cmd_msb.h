#pragma once

#include <vector>
#include "pilot_nt.h"

std::vector<unsigned char> GetFrameGetReady(unsigned short function_id);
std::vector<unsigned char> GetFrameTrx(auth_answer& auth_answer);
std::vector<unsigned char> GetFrameNewHostMasterCall(std::vector<unsigned char>& serialNumber, unsigned char TType);
std::vector<unsigned char> GetFrameReadTCPMasterCall(std::vector<unsigned char>& serialNumber, std::vector<char>& outDataTCP);
std::vector<unsigned char> GetFrameWriteTCPMasterCall(std::vector<unsigned char>& serialNumber, int sizeData);
std::vector<unsigned char> GetFrameCloseTCPMasterCall(std::vector<unsigned char>& serialNumber);
std::vector<unsigned char> GetFramePingInfoMasterCall(std::vector<unsigned char>& serialNumber);
std::vector<unsigned char> GetFramePingInfoTwoMessageMasterCall();
std::vector<unsigned char> GetFrameWriteToCheckMasterCall(std::vector<unsigned char>& serialNumber);
std::vector<unsigned char> GetFraneCloseToCheckMasterCall(std::vector<unsigned char>& serialNumber);