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
        0x00, //����� ������ ���-�� ���� 1
        0x4B, //����� ������ ���-�� ���� 1
        0x6D, //��� ������� ���-�� ���� 1
        0x44, //����� ���� ��������� ���-�� ���� 2
        0x00, //����� ���� ��������� ���-�� ���� 2
        0x00, //����� ��������� (�������� �������) ���-�� ���� 4
        0x00, //����� ��������� (�������� �������) ���-�� ���� 4
        0x00, //����� ��������� (�������� �������) ���-�� ���� 4
        0x00, //����� ��������� (�������� �������) ���-�� ���� 4
        (unsigned char)auth_answer.Amount, //����� � �������� (�������� �������) ���-�� ���� 4
        (unsigned char)(auth_answer.Amount >> 8), //����� � �������� (�������� �������) ���-�� ���� 4
        (unsigned char)(auth_answer.Amount >> 16),//����� � �������� (�������� �������) ���-�� ���� 4
        (unsigned char)(auth_answer.Amount >> 24),//����� � �������� (�������� �������) ���-�� ���� 4
        (unsigned char)auth_answer.CType, //��� ����� ���-�� ���� 1
        0x00, //����� ������ ���-�� ���� 1
        (unsigned char)auth_answer.TType, //��� �������� ���-�� ���� 1
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //������ ������� ����� ���-�� ���� 40
        0x00, //request id (�������� �������) ���-�� ���� 4
        0x00, //request id (�������� �������) ���-�� ���� 4
        0x00, //request id (�������� �������) ���-�� ���� 4
        0x00, //request id (�������� �������) ���-�� ���� 4
        0x00, //rrn ���-�� ���� 13
        0x00, //rrn ���-�� ���� 13
        0x00, //rrn ���-�� ���� 13
        0x00, //rrn ���-�� ���� 13
        0x00, //rrn ���-�� ���� 13
        0x00, //rrn ���-�� ���� 13
        0x00, //rrn ���-�� ���� 13
        0x00, //rrn ���-�� ���� 13
        0x00, //rrn ���-�� ���� 13
        0x00, //rrn ���-�� ���� 13
        0x00, //rrn ���-�� ���� 13
        0x00, //rrn ���-�� ���� 13
        0x00, //rrn ���-�� ���� 13
        0x00, //��� ����� �������� (�������� �������) ���-�� ���� 4
        0x00, //��� ����� �������� (�������� �������) ���-�� ���� 4
        0x00, //��� ����� �������� (�������� �������) ���-�� ���� 4
        0x00  //��� ����� �������� (�������� �������) ���-�� ���� 4
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

std::vector<unsigned char> GetFrameReadTCP(std::vector<unsigned char>& serialNumber, std::vector<char>& outDataTCP)
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

std::vector<unsigned char> GetFrameWriteTCP(std::vector<unsigned char>& serialNumber, int sizeData)
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

std::vector<unsigned char> GetFrameCloseTCP(std::vector<unsigned char>& serialNumber)
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
