#pragma once

#include <vector>
#include "pilot_nt.h"

std::vector<unsigned char> GetFrameGetReady(unsigned short function_id);
std::vector<unsigned char> GetFrameTrx(auth_answer& auth_answer);
std::vector<unsigned char> GetFrameNewHostMasterCall(std::vector<unsigned char>& serialNumber, unsigned char TType);