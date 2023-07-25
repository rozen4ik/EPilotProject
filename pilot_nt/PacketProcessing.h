#pragma once

#include <vector>
#include "base64.h"
#include "pilot_nt.h"
#include "CommunicationWithSber.h"
#include "cmd_msb.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <inaddr.h>
#include <stdio.h>
#include <vector>
#include <tchar.h>

#pragma comment(lib, "ws2_32.lib")

unsigned short ComputeChecksum(std::vector<unsigned char>& bytes);
void GetFrameWithCrc16(std::vector<unsigned char>& frame);
std::vector<unsigned char> GetBinaryOutData(std::string& outData);
void ioPort(std::string& inData, std::string& outData);
std::string GetIp(std::vector<unsigned char>& response);
int GetPort(std::vector<unsigned char>& response);
void GetSerialNumberMessage(std::vector<unsigned char>& response, std::vector<unsigned char>& serialNumber);
void BodyWorkPilotTrx(auth_answer& auth_answer, std::vector<unsigned char>& response);