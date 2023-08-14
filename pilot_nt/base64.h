#pragma once

#include <vector>
#include <string>

std::string base64_encode(unsigned char const* buf, unsigned int bufLen);
std::vector<unsigned char> base64_decode(std::string const&);