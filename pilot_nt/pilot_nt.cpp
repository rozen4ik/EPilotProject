#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include "pilot_nt.h"
#include "com_port.h"
#include "cmd_msb.h"
#include "PacketProcessing.h"
#include "base64.h"
#include <iostream>

int TestPinpad()
{	
	std::cout << "\nTestPinpad" << std::endl;

	HANDLE hSerialPort;
	std::string outData = "";
	std::vector<unsigned char> frame = GetFrameGetReady(0x0000);

	GetFrameWithCrc16(frame);

	std::string resFrame = "\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";	
	std::cout << resFrame << std::endl;
	
	ioPort(resFrame, outData);

	std::vector<unsigned char> response = GetBinaryOutData(outData);

	return response[2];
}

int close_day(auth_answer* auth_answer)
{
	TestPinpad();
	std::cout << "\nclose_day" << std::endl;

	HANDLE hSerialPort;
	std::string outData = "";
	std::vector<unsigned char> frame = GetFrameTrx(*auth_answer);

	GetFrameWithCrc16(frame);

	std::string resFrame = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";
	std::cout << resFrame << std::endl;

	ioPort(resFrame, outData);

	std::vector<unsigned char> response = GetBinaryOutData(outData);
	BodyWorkPilotTrx(*auth_answer, response);

	return 0;
}