#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include "pilot_nt.h"
#include "com_port.h"
#include "cmd_getready.h"
#include "PacketProcessing.h"
#include "base64.h"
#include <iostream>

void hello_test(char* name)
{
	HANDLE hSerialPort;
	std::string data = { "\u0002#AAxQBQBr9AUAAAAAAABaAz==\u0003" };
	std::string outData = "";
	int size = sizeof(data);

	open_port(&hSerialPort);

	write_port(&hSerialPort, &data[0], size);

	read_port(&hSerialPort, outData);

	std::cout << outData.size() << std::endl;	
	std::cout << outData << std::endl;

	outData = "";

	char d[] = { "\u0004" };
	size = sizeof(d);
	write_port(&hSerialPort, &d[0], size);

	read_port(&hSerialPort, outData);

	std::cout << outData.size() << std::endl;
	std::cout << outData << std::endl;

	close_port(&hSerialPort);
}

int TestPinpad()
{
	HANDLE hSerialPort;
	std::string outData = "";

	std::vector<unsigned char> frame = GetFrameGetReady(0x0000);

	for (unsigned char c : frame)
	{
		std::cout << std::hex << std::uppercase <<int(c) << " ";
	}

	std::cout << std::endl;

	GetFrameWithCrc16(frame);

	for (unsigned char c : frame)
	{
		std::cout << std::hex << std::uppercase << int(c) << " ";
	}

	std::cout << std::endl;

	std::string resFrame = "\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";
	
	std::cout << resFrame << std::endl;
	
	open_port(&hSerialPort);
	write_port(&hSerialPort, &resFrame[0], resFrame.length());
	read_port(&hSerialPort, outData);
	std::cout << outData << std::endl;
	close_port(&hSerialPort);

	return 0;
}
