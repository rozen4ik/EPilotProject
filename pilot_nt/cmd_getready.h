#pragma once

#include <vector>

/// <summary>
/// ������� CMD_GETREADY ��� Crc16
/// </summary>
/// <param name="function_id"></param>
/// <returns></returns>
std::vector<unsigned char> GetFrameGetReady(unsigned short function_id);