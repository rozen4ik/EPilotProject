#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "pilot_nt.h"

class SberCmd
{
private:
	std::vector<unsigned char> frame;
	std::string res_frame;
	std::vector<unsigned char> first_response;
	std::vector<unsigned char> response;
	std::vector<unsigned char> last_response;
	std::vector<unsigned char> serial_number;

	const std::string base64_chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";


	static inline bool IsBase64(unsigned char c) 
	{
		return (isalnum(c) || (c == '+') || (c == '/'));
	}

	unsigned short ComputeChecksum(std::vector<unsigned char>& bytes);
	void SetFrameWithCrc16();
	std::string Base64Encode(unsigned char const* buf, unsigned int bufLen);
	std::vector<unsigned char> Base64Decode(std::string const& encoded_string);
	void SetSerialNumberMessage();
	void SetSerialNumber();
public:
	std::vector<unsigned char> GetFrame();
	std::string GetResFrame();
	void SetResFrame(std::string nf_mark);
	std::vector<unsigned char> GetResponse();
	std::vector<unsigned char> GetLastResponse();
	void SetBinaryOutData(std::string& outData);
	void SetLastResponse(int start_index);
	void SetFirstResponse();
	std::vector<unsigned char> GetFirstResponse();
	void ClearFirstResponse();
	//---------------------------------------------------------------------------------------
	void RunGetReady(unsigned short function_id); ///<
	void RunTrx(auth_answer& auth_answer); ///<
	void RunTrx(auth_answer14& auth_answer); ///<
	void RunNewHostMasterCall(unsigned char TType); ///<
	void RunReadTCPMasterCall(std::vector<char>& outDataTCP); ///<
	void RunFirstReadTCPMasterCall(int number_message, std::vector<char>& out_data_tcp, int size_out_data_tcp);
	void RunNextReadTCPMasterCall(int number_message, std::vector<char>& out_data_tcp);
	void RunWriteTCPMasterCall(int sizeData); ///<
	void RunCloseTCPMasterCall(); ///<
	void RunPingInfoMasterCall(); ///<
	void RunPingInfoTwoMessageMasterCall(); ///<
	void RunWriteToCheckMasterCall(); ///<
	void RunCloseToCheckMasterCall(); ///<
};