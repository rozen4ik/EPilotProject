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
	void open_port();
	void read_port(std::string& data);
	void write_port(char* data, const int size);
	void close_port();
	void io_port(std::string& inData, std::string& outData);
};

