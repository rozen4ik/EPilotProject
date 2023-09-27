#include "pch.h"
#include "SberCmd.h"

unsigned short SberCmd::ComputeChecksum(std::vector<unsigned char>& bytes)
{
	const unsigned short poly = 0x1021;
	unsigned short table[256];
	unsigned short crc = 0xFFFF;

	unsigned short temp, a;
	for (int i = 0; i < 256; ++i)
	{
		temp = 0;
		a = (unsigned short)(i << 8);
		for (int j = 0; j < 8; ++j)
		{
			if (((temp ^ a) & 0x8000) != 0)
			{
				temp = (unsigned short)((temp << 1) ^ poly);
			}
			else
			{
				temp <<= 1;
			}

			a <<= 1;
		}

		table[i] = temp;
	}

	for (char it : bytes)
	{
		crc = (unsigned short)((crc << 8) ^ table[((crc >> 8) ^ (0xFF & it))]);
	}
	return crc;
}

void SberCmd::SetFrameWithCrc16()
{
	unsigned short crc16 = ComputeChecksum(frame);
	std::cout << "crc16: " << std::hex << crc16 << std::endl;
	frame.push_back((unsigned char)crc16);
	frame.push_back((unsigned char)(crc16 >> 8));
}

std::string SberCmd::Base64Encode(unsigned char const* buf, unsigned int bufLen)
{
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (bufLen--) {
		char_array_3[i++] = *(buf++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i < 4); i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while ((i++ < 3))
			ret += '=';
	}

	return ret;
}

std::vector<unsigned char> SberCmd::Base64Decode(std::string const& encoded_string)
{
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::vector<unsigned char> ret;

	while (in_len-- && (encoded_string[in_] != '=') && IsBase64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i == 4) {
			for (i = 0; i < 4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret.push_back(char_array_3[i]);
			i = 0;
		}
	}

	if (i) {
		for (j = i; j < 4; j++)
			char_array_4[j] = 0;

		for (j = 0; j < 4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
	}

	return ret;
}

void SberCmd::SetSerialNumberMessage()
{
	serial_number.clear();

	if (first_response.size() > 0)
		for (int i = 5; i < 9; i++)
			serial_number.push_back(first_response[i]);
	else
		for (int i = 5; i < 9; i++)
			serial_number.push_back(response[i]);

	serial_number[3] = 0x80;
}

void SberCmd::SetSerialNumber()
{
	SetSerialNumberMessage();
	int j = 0;
	for (int i = 5; i < 9; i++)
	{
		frame[i] = serial_number[j];
		j++;
	}
}

std::vector<unsigned char> SberCmd::GetFrame()
{
    return frame;
}

std::string SberCmd::GetResFrame()
{
    return res_frame;
}

void SberCmd::SetResFrame(std::string nf_mark)
{
	res_frame = nf_mark + Base64Encode(&frame[0], frame.size()) + "\u0003";
}

std::vector<unsigned char> SberCmd::GetResponse()
{
    return response;
}

std::vector<unsigned char> SberCmd::GetLastResponse()
{
	return last_response;
}

void SberCmd::SetBinaryOutData(std::string& outData)
{
	std::erase(outData, '\u0002');
	std::erase(outData, '\u0003');
	std::erase(outData, '\u0004');
	std::erase(outData, '#');

	response.clear();
	response = Base64Decode(outData);
}

void SberCmd::SetLastResponse(int start_index)
{
	first_response.clear();	
	for (int i = start_index; i < response.size() - 2; i++)
		last_response.push_back(response[i]);
}

void SberCmd::SetFirstResponse()
{
	first_response.clear();
	first_response = response;
}

std::vector<unsigned char> SberCmd::GetFirstResponse()
{
	return first_response;
}

void SberCmd::ClearFirstResponse()
{
	first_response.clear();
}

//---------------------------------------------------------------------------------------

void SberCmd::RunGetReady(unsigned short function_id)
{
	frame =
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

	SetFrameWithCrc16();
}

void SberCmd::RunTrx(auth_answer& auth_answer)
{
	frame =
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

	SetFrameWithCrc16();
}

void SberCmd::RunTrx(auth_answer14& auth_answer)
{
	frame =
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
		(unsigned char)auth_answer.ans.Amount, //����� � �������� (�������� �������) ���-�� ���� 4
		(unsigned char)(auth_answer.ans.Amount >> 8), //����� � �������� (�������� �������) ���-�� ���� 4
		(unsigned char)(auth_answer.ans.Amount >> 16),//����� � �������� (�������� �������) ���-�� ���� 4
		(unsigned char)(auth_answer.ans.Amount >> 24),//����� � �������� (�������� �������) ���-�� ���� 4
		(unsigned char)auth_answer.ans.CType, //��� ����� ���-�� ���� 1
		0x00, //����� ������ ���-�� ���� 1
		(unsigned char)auth_answer.ans.TType, //��� �������� ���-�� ���� 1
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
		(unsigned char)auth_answer.RequestID, //request id (�������� �������) ���-�� ���� 4
		(unsigned char)(auth_answer.RequestID >> 8), //request id (�������� �������) ���-�� ���� 4
		(unsigned char)(auth_answer.RequestID >> 16), //request id (�������� �������) ���-�� ���� 4
		(unsigned char)(auth_answer.RequestID >> 24), //request id (�������� �������) ���-�� ���� 4
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

	SetFrameWithCrc16();
}

void SberCmd::RunNewHostMasterCall(unsigned char TType)
{
	frame =
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

	SetSerialNumber();
	SetFrameWithCrc16();
}

void SberCmd::RunReadTCPMasterCall(std::vector<char>& out_data_tcp)
{
	int size_data = out_data_tcp.size();
	frame =
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
		0x02,
		0x19,
		0x00,
		(unsigned char)size_data,
		(unsigned char)(size_data >> 8)
	};

	SetSerialNumber();

	for (char c : out_data_tcp)
		frame.push_back(c);

	int size_frame = frame.size() - 2;
	int size_message = frame.size() - 9;

	frame[1] = (unsigned char)size_frame;
	frame[3] = (unsigned char)size_message;
	frame[4] = (unsigned char)(size_message >> 8);

	SetFrameWithCrc16();
}

void SberCmd::RunFirstReadTCPMasterCall(int number_message, std::vector<char>& out_data_tcp, int size_out_data_tcp)
{
	frame =
	{
		(unsigned char)number_message,
		0x0C,
		0x00,
		0x05,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x02,
		0x19,
		0x00,
		(unsigned char)size_out_data_tcp,
		(unsigned char)(size_out_data_tcp >> 8)
	};

	SetSerialNumber();

	for (char c : out_data_tcp)
		frame.push_back(c);

	int size_frame = frame.size() - 2;
	int size_message = frame.size() - 9;

	frame[1] = (unsigned char)size_frame;
	frame[3] = (unsigned char)size_message;
	frame[4] = (unsigned char)(size_message >> 8);

	SetFrameWithCrc16();
}

void SberCmd::RunNextReadTCPMasterCall(int number_message, std::vector<char>& out_data_tcp)
{
	frame =
	{
		(unsigned char)number_message,
		0x26		
	};

	for (char c : out_data_tcp)
		frame.push_back(c);

	int size_frame = frame.size() - 2;
	frame[1] = (unsigned char)size_frame;

	SetFrameWithCrc16();
}

void SberCmd::RunWriteTCPMasterCall(int sizeData)
{
	frame =
	{
		0x00,
		0x0E,
		0x00,
		0x07,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x03,
		0x19,
		0x00,
		0x02,
		0x00,
		(unsigned char)sizeData,
		(unsigned char)(sizeData >> 8)
	};

	SetSerialNumber();
	SetFrameWithCrc16();
}

void SberCmd::RunCloseTCPMasterCall()
{
	frame =
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
		0x04,
		0x19,
		0x00,
		0x00,
		0x00
	};

	SetSerialNumber();
	SetFrameWithCrc16();
}

void SberCmd::RunPingInfoMasterCall()
{
	frame =
	{
		0x80,
		0xB4,
		0x00,
		0xD3,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x02,
		0x27,
		0x00,
		0xCE,
		0x00,
		0xE2, 0x81, 0xCB, 0xCE, 0x01, 0x02, 0xC4, 0x08, 0x55, 0x50,
		0x4F, 0x53, 0x20, 0x44, 0x4C, 0x4C, 0xC5, 0x0C, 0x50, 0x30,
		0x32, 0x30, 0x32, 0x37, 0x32, 0x35, 0x32, 0x31, 0x35, 0x37,
		0xE1, 0x10, 0xC8, 0x04, 0x55, 0x50, 0x4F, 0x53, 0xC9, 0x08,
		0x32, 0x39, 0x2E, 0x39, 0x37, 0x2E, 0x30, 0x36, 0xE1, 0x10,
		0xC8, 0x02, 0x4F, 0x53, 0xC9, 0x0A, 0x57, 0x49, 0x4E, 0x5F,
		0x31, 0x30, 0x20, 0x20, 0x20, 0x20, 0xE1, 0x19, 0xC8, 0x0C,
		0x6C, 0x6F, 0x61, 0x64, 0x70, 0x61, 0x72, 0x6D, 0x2E, 0x65,
		0x78, 0x65, 0xC9, 0x09, 0x32, 0x39, 0x2E, 0x39, 0x37, 0x2E,
		0x36, 0x2E, 0x30, 0xE1, 0x15, 0xC8, 0x08, 0x67, 0x61, 0x74,
		0x65, 0x2E, 0x64, 0x6C, 0x6C, 0xC9, 0x09, 0x32, 0x39, 0x2E,
		0x39, 0x37, 0x2E, 0x36, 0x2E, 0x30, 0xE1, 0x1A, 0xC8, 0x0D,
		0x73, 0x62, 0x5F, 0x6B, 0x65, 0x72, 0x6E, 0x65, 0x6C, 0x2E,
		0x64, 0x6C, 0x6C, 0xC9, 0x09, 0x32, 0x39, 0x2E, 0x39, 0x37,
		0x2E, 0x36, 0x2E, 0x30, 0xE4, 0x18, 0xD3, 0x01, 0x04, 0xD4,
		0x01, 0x00, 0xD5, 0x04, 0x64, 0x65, 0x00, 0x17, 0xD5, 0x04,
		0xC0, 0xA8, 0x00, 0x80, 0xD5, 0x04, 0xC0, 0xA8
	};

	SetSerialNumber();
	SetFrameWithCrc16();
}

void SberCmd::RunPingInfoTwoMessageMasterCall()
{
	frame =
	{
		0x01,
		0x26,
		0x7F, 0x0A, 0xE4, 0x06, 0xD3, 0x01, 0x04, 0xD4, 0x01, 0x01,
		0xE4, 0x0C, 0xD3, 0x01, 0x04, 0xD4, 0x01, 0x00, 0xD5, 0x04,
		0xC0, 0xA8, 0x15, 0x01, 0xE4, 0x0C, 0xD3, 0x01, 0x04, 0xD4,
		0x01, 0x00, 0xD5, 0x04, 0xC0, 0xA8, 0xDF, 0x01
	};

	SetFrameWithCrc16();
}

void SberCmd::RunWriteToCheckMasterCall()
{
	frame =
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
		0x03,
		0x03,
		0x00,
		0x00,
		0x00
	};

	SetSerialNumber();
	SetFrameWithCrc16();
}

void SberCmd::RunCloseToCheckMasterCall()
{
	frame =
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
		0x04,
		0x03,
		0x00,
		0x00,
		0x00
	};

	SetSerialNumber();
	SetFrameWithCrc16();
}
