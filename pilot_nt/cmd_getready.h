#pragma once

#include <vector>

/// <summary>
/// Команда CMD_GETREADY без Crc16
/// </summary>
/// <param name="function_id"></param>
/// <returns></returns>
std::vector<unsigned char> GetFrameGetReady(unsigned short function_id);