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