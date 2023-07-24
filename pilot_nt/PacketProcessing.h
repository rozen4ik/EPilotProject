#pragma once

#include <vector>

unsigned short ComputeChecksum(std::vector<unsigned char>& bytes);
void GetFrameWithCrc16(std::vector<unsigned char>& frame);