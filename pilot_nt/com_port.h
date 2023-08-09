#pragma once

#include <Windows.h>
#include <string>
#include <unordered_map>

void open_port(HANDLE* hSerial);
void read_port(HANDLE* hSerial, std::string& data);
void write_port(HANDLE* hSerial, char* data, const int size);
void close_port(HANDLE* hSerial);
void getSettingsForPinpad(std::unordered_map<std::string, std::string>& settings);