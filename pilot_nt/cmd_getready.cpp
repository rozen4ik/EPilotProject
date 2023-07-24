#pragma once

#include "pch.h"
#include "cmd_getready.h"

std::vector<unsigned char> GetFrameGetReady(unsigned short function_id)
{
    std::vector<unsigned char> frameWithoutCrc16 =
    {
        0x00,
        0x0C,
        0x50,
        0x05,
        0x00,
        0x6B,
        0xF4,
        0x05,
        0x00,
        0x00,
        0x00,
        0x00,
        (unsigned char)(function_id >> 8),
        (unsigned char)function_id                
    };

    return frameWithoutCrc16;
}
