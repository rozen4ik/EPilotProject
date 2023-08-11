#pragma once

#include "pch.h"
#include "cmd_msb.h"

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

std::vector<unsigned char> GetFrameTrx(auth_answer& auth_answer)
{    
    std::vector<unsigned char> frameWithoutCrc16 =
    {
        0x00, //номер фрейма кол-во байт 1
        0x4B, //длина фрейма кол-во байт 1
        0x6D, //код команды кол-во байт 1
        0x44, //длина тела сообщения кол-во байт 2
        0x00, //длина тела сообщения кол-во байт 2
        0x00, //номер сообщения (обратный порядок) кол-во байт 4
        0x00, //номер сообщения (обратный порядок) кол-во байт 4
        0x00, //номер сообщения (обратный порядок) кол-во байт 4
        0x00, //номер сообщения (обратный порядок) кол-во байт 4
        (unsigned char)auth_answer.Amount, //сумма в копейках (обратный порядок) кол-во байт 4
        (unsigned char)(auth_answer.Amount >> 8), //сумма в копейках (обратный порядок) кол-во байт 4
        (unsigned char)(auth_answer.Amount >> 16),//сумма в копейках (обратный порядок) кол-во байт 4
        (unsigned char)(auth_answer.Amount >> 24),//сумма в копейках (обратный порядок) кол-во байт 4
        (unsigned char)auth_answer.CType, //тип карты кол-во байт 1
        0x00, //номер отдела кол-во байт 1
        (unsigned char)auth_answer.TType, //тип операции кол-во байт 1
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //request id (обратный порядок) кол-во байт 4
        0x00, //request id (обратный порядок) кол-во байт 4
        0x00, //request id (обратный порядок) кол-во байт 4
        0x00, //request id (обратный порядок) кол-во байт 4
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //доп флаги операции (обратный порядок) кол-во байт 4
        0x00, //доп флаги операции (обратный порядок) кол-во байт 4
        0x00, //доп флаги операции (обратный порядок) кол-во байт 4
        0x00  //доп флаги операции (обратный порядок) кол-во байт 4
    };

    return frameWithoutCrc16;
}

std::vector<unsigned char> GetFrameTrx(auth_answer14& auth_answer)
{
    std::vector<unsigned char> frameWithoutCrc16 =
    {
        0x00, //номер фрейма кол-во байт 1
        0x4B, //длина фрейма кол-во байт 1
        0x6D, //код команды кол-во байт 1
        0x44, //длина тела сообщения кол-во байт 2
        0x00, //длина тела сообщения кол-во байт 2
        0x00, //номер сообщения (обратный порядок) кол-во байт 4
        0x00, //номер сообщения (обратный порядок) кол-во байт 4
        0x00, //номер сообщения (обратный порядок) кол-во байт 4
        0x00, //номер сообщения (обратный порядок) кол-во байт 4
        (unsigned char)auth_answer.ans.Amount, //сумма в копейках (обратный порядок) кол-во байт 4
        (unsigned char)(auth_answer.ans.Amount >> 8), //сумма в копейках (обратный порядок) кол-во байт 4
        (unsigned char)(auth_answer.ans.Amount >> 16),//сумма в копейках (обратный порядок) кол-во байт 4
        (unsigned char)(auth_answer.ans.Amount >> 24),//сумма в копейках (обратный порядок) кол-во байт 4
        (unsigned char)auth_answer.ans.CType, //тип карты кол-во байт 1
        0x00, //номер отдела кол-во байт 1
        (unsigned char)auth_answer.ans.TType, //тип операции кол-во байт 1
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        0x00, //вторая дорожка карты кол-во байт 40
        (unsigned char)auth_answer.RequestID, //request id (обратный порядок) кол-во байт 4
        (unsigned char)(auth_answer.RequestID >> 8), //request id (обратный порядок) кол-во байт 4
        (unsigned char)(auth_answer.RequestID >> 16), //request id (обратный порядок) кол-во байт 4
        (unsigned char)(auth_answer.RequestID >> 24), //request id (обратный порядок) кол-во байт 4
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //rrn кол-во байт 13
        0x00, //доп флаги операции (обратный порядок) кол-во байт 4
        0x00, //доп флаги операции (обратный порядок) кол-во байт 4
        0x00, //доп флаги операции (обратный порядок) кол-во байт 4
        0x00  //доп флаги операции (обратный порядок) кол-во байт 4
    };

    return frameWithoutCrc16;
}

std::vector<unsigned char> GetFrameNewHostMasterCall(std::vector<unsigned char>& serialNumber, unsigned char TType)
{
    std::vector<unsigned char> frameWithoutCrc16 =
    {
        0x00,
        0x0C,
        0x00,
        0x05,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x01,
        TType,
        0x00,
        0x00,
        0x00
    };

    int j = 0;
    for (int i = 5; i < 9; i++)
    {
        frameWithoutCrc16[i] = serialNumber[j];
        j++;
    }

    return frameWithoutCrc16;
}

std::vector<unsigned char> GetFrameReadTCPMasterCall(std::vector<unsigned char>& serialNumber, std::vector<char>& outDataTCP)
{
    int sizeData = outDataTCP.size();
    std::vector<unsigned char> frameWithoutCrc16 =
    {
        0x00,
        0x0C,
        0x00,
        0x05,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x02,
        0x19,
        0x00,
        (unsigned char)sizeData,
        (unsigned char)(sizeData >> 8)
    };

    int j = 0;
    for (int i = 5; i < 9; i++)
    {
        frameWithoutCrc16[i] = serialNumber[j];
        j++;
    }

    for (char c : outDataTCP)
        frameWithoutCrc16.push_back(c);

    int sizeFrame = frameWithoutCrc16.size() - 2;
    int sizeMessage = frameWithoutCrc16.size() - 9;

    frameWithoutCrc16[1] = (unsigned char)sizeFrame;
    frameWithoutCrc16[3] = (unsigned char)sizeMessage;
    frameWithoutCrc16[4] = (unsigned char)(sizeMessage >> 8);

    return frameWithoutCrc16;
}

std::vector<unsigned char> GetFrameWriteTCPMasterCall(std::vector<unsigned char>& serialNumber, int sizeData)
{
    std::vector<unsigned char> frameWithoutCrc16 =
    {
        0x00,
        0x0E,
        0x00,
        0x07,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x03,
        0x19,
        0x00,
        0x02,
        0x00,
        (unsigned char)sizeData,
        (unsigned char)(sizeData >> 8)
    };

    int j = 0;
    for (int i = 5; i < 9; i++)
    {
        frameWithoutCrc16[i] = serialNumber[j];
        j++;
    }

    return frameWithoutCrc16;
}

std::vector<unsigned char> GetFrameCloseTCPMasterCall(std::vector<unsigned char>& serialNumber)
{
    std::vector<unsigned char> frameWithoutCrc16 =
    {
        0x00,
        0x0C,
        0x00,
        0x05,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x04,
        0x19,
        0x00,
        0x00,
        0x00
    };

    int j = 0;
    for (int i = 5; i < 9; i++)
    {
        frameWithoutCrc16[i] = serialNumber[j];
        j++;
    }

    return frameWithoutCrc16;
}

std::vector<unsigned char> GetFramePingInfoMasterCall(std::vector<unsigned char>& serialNumber)
{
    std::vector<unsigned char> frameWithoutCrc16 =
    {
        0x80,
        0xB4,
        0x00,
        0xD3,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x02,
        0x27,
        0x00,
        0xCE,
        0x00,
        0xE2, 0x81, 0xCB, 0xCE, 0x01, 0x02, 0xC4, 0x08, 0x55, 0x50,
        0x4F, 0x53, 0x20, 0x44, 0x4C, 0x4C, 0xC5, 0x0C, 0x50, 0x30,
        0x32, 0x30, 0x32, 0x37, 0x32, 0x35, 0x32, 0x31, 0x35, 0x37,
        0xE1, 0x10, 0xC8, 0x04, 0x55, 0x50, 0x4F, 0x53, 0xC9, 0x08,
        0x32, 0x39, 0x2E, 0x39, 0x37, 0x2E, 0x30, 0x36, 0xE1, 0x10,
        0xC8, 0x02, 0x4F, 0x53, 0xC9, 0x0A, 0x57, 0x49, 0x4E, 0x5F,
        0x31, 0x30, 0x20, 0x20, 0x20, 0x20, 0xE1, 0x19, 0xC8, 0x0C,
        0x6C, 0x6F, 0x61, 0x64, 0x70, 0x61, 0x72, 0x6D, 0x2E, 0x65,
        0x78, 0x65, 0xC9, 0x09, 0x32, 0x39, 0x2E, 0x39, 0x37, 0x2E,
        0x36, 0x2E, 0x30, 0xE1, 0x15, 0xC8, 0x08, 0x67, 0x61, 0x74,
        0x65, 0x2E, 0x64, 0x6C, 0x6C, 0xC9, 0x09, 0x32, 0x39, 0x2E,
        0x39, 0x37, 0x2E, 0x36, 0x2E, 0x30, 0xE1, 0x1A, 0xC8, 0x0D,
        0x73, 0x62, 0x5F, 0x6B, 0x65, 0x72, 0x6E, 0x65, 0x6C, 0x2E,
        0x64, 0x6C, 0x6C, 0xC9, 0x09, 0x32, 0x39, 0x2E, 0x39, 0x37,
        0x2E, 0x36, 0x2E, 0x30, 0xE4, 0x18, 0xD3, 0x01, 0x04, 0xD4,
        0x01, 0x00, 0xD5, 0x04, 0x64, 0x65, 0x00, 0x17, 0xD5, 0x04,
        0xC0, 0xA8, 0x00, 0x80, 0xD5, 0x04, 0xC0, 0xA8
    };

    int j = 0;
    for (int i = 5; i < 9; i++)
    {
        frameWithoutCrc16[i] = serialNumber[j];
        j++;
    }

    return frameWithoutCrc16;
}

std::vector<unsigned char> GetFramePingInfoTwoMessageMasterCall()
{
    std::vector<unsigned char> frameWithoutCrc16 =
    {
        0x01,
        0x26,
        0x7F, 0x0A, 0xE4, 0x06, 0xD3, 0x01, 0x04, 0xD4, 0x01, 0x01,
        0xE4, 0x0C, 0xD3, 0x01, 0x04, 0xD4, 0x01, 0x00, 0xD5, 0x04,
        0xC0, 0xA8, 0x15, 0x01, 0xE4, 0x0C, 0xD3, 0x01, 0x04, 0xD4,
        0x01, 0x00, 0xD5, 0x04, 0xC0, 0xA8, 0xDF, 0x01
    };

    return frameWithoutCrc16;
}

std::vector<unsigned char> GetFrameWriteToCheckMasterCall(std::vector<unsigned char>& serialNumber)
{
    std::vector<unsigned char> frameWithoutCrc16 =
    {
        0x00,
        0x0C,
        0x00,
        0x05,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x03,
        0x03,
        0x00,
        0x00,
        0x00
    };

    int j = 0;
    for (int i = 5; i < 9; i++)
    {
        frameWithoutCrc16[i] = serialNumber[j];
        j++;
    }

    return frameWithoutCrc16;
}

std::vector<unsigned char> GetFraneCloseToCheckMasterCall(std::vector<unsigned char>& serialNumber)
{
    std::vector<unsigned char> frameWithoutCrc16 =
    {
        0x00,
        0x0C,
        0x00,
        0x05,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x04,
        0x03,
        0x00,
        0x00,
        0x00
    };

    int j = 0;
    for (int i = 5; i < 9; i++)
    {
        frameWithoutCrc16[i] = serialNumber[j];
        j++;
    }

    return frameWithoutCrc16;
}
