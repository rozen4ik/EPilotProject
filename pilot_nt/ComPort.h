#pragma once

#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include "PacketProcessing.h"
#include <chrono>
#include <thread>
#include <cassert>

class ComPort
{
private:
	HANDLE hSerial;
	std::unordered_map<std::string, std::string> settings;
public:
	ComPort();
	void OpenPort();
	void ReadPort(std::string& data);
	void WritePort(char* data, const int size);
	void ClosePort();
	void IOPort(const std::string& inData, std::string& outData);
};

