#pragma once

#include <Windows.h>
#include <string>

void open_port(HANDLE* hSerial);
void read_port(HANDLE* hSerial, std::string& data);
void write_port(HANDLE* hSerial, char* data, const int size);
void close_port(HANDLE* hSerial);
void read_ini_file(LPCTSTR& sPortName, int& speedPort);