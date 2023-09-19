#include "pch.h"
#include "PilotService.h"

int PilotService::TestPinpad()
{
	Logger("\nTestPinpad");
	ComPort com_port;
	SberCmd sber_cmd;

	std::string outData = "";
	sber_cmd.RunGetReady(0x0000);
	sber_cmd.SetResFrame("\u0002#");
	std::string res_frame = sber_cmd.GetResFrame();
	com_port.io_port(res_frame, outData);
	sber_cmd.SetBinaryOutData(outData);
	Logger("End Command");

	return sber_cmd.GetResponse()[2];
}

//int PilotService::close_day(auth_answer* auth_answer, std::string& check)
//{
//	lastResponsePax.clear();
//	check = "";
//	TestPinpad();
//	Logger("\nclose_day");
//	StartWork(*auth_answer, lastResponsePax, str);	
//
//	rStr = str;
//	OemToCharBuffA(str.c_str(), &rStr[0], str.size());
//	check = rStr;
//	auth_answer->Check = check.data();
//
//	char errCode[] = { lastResponsePax[0], lastResponsePax[1] };
//	int RCode = *((unsigned short*)errCode);
//
//	for (int i = 0; i < 3; i++)
//		auth_answer->RCode[i] = std::to_string(RCode)[i];
//
//	std::string result;
//
//	if (RCode == 0)
//		result = "��������";
//	else
//		result = "���������";
//
//	for (int i = 0; i < 16; i++)
//		auth_answer->AMessage[i] = result[i];
//
//	Logger("End Command");
//	return RCode;
//}

int PilotService::close_day(auth_answer* auth_answer, std::string& check)
{
	lastResponsePax.clear();
	check = "";
	TestPinpad();
	Logger("\nclose_day");
	SberCmd sber_cmd;
	StartWork(*auth_answer, sber_cmd, str);

	rStr = str;
	OemToCharBuffA(str.c_str(), &rStr[0], str.size());
	check = rStr;
	auth_answer->Check = check.data();

	char errCode[] = { sber_cmd.GetLastResponse()[0], sber_cmd.GetLastResponse()[1] };
	int RCode = *((unsigned short*)errCode);

	for (int i = 0; i < 3; i++)
		auth_answer->RCode[i] = std::to_string(RCode)[i];

	std::string result;

	if (RCode == 0)
		result = "��������";
	else
		result = "���������";

	for (int i = 0; i < 16; i++)
		auth_answer->AMessage[i] = result[i];

	Logger("End Command");
	return RCode;
}

int PilotService::card_authorize(const char* track2, auth_answer* auth_answer)
{
	lastResponsePax.clear();
	runCardAuth["cardAuth"] = 1;
	if (runCardAuth["cardAuth"] == 0) return 2000;
	TestPinpad();
	if (runCardAuth["cardAuth"] == 0) return 2000;
	Logger("\ncard_authorize");
	std::string track;	

	if (track2 != nullptr)
		track = track2;

	if (runCardAuth["cardAuth"] == 0) return 2000;
	StartWork(*auth_answer, lastResponsePax, str, runCardAuth);

	rStr = str;
	OemToCharBuffA(str.c_str(), &rStr[0], str.size());
	auth_answer->Check = &rStr[0];

	std::cout << std::endl;

	if (runCardAuth["cardAuth"] == 0) return 2000;

	char errCode[] = { lastResponsePax[0], lastResponsePax[1] };
	int RCode = *((unsigned short*)errCode);
	Logger("End command");

	return RCode;
}

int PilotService::card_authorize15(const char* track2, auth_answer14& auth_answer, payment_info_item& payinfo, CONTEXT_PTR dataIn, CONTEXT_PTR dataOut, std::string& check)
{
	lastResponsePax.clear();
	check = "";
	runCardAuth["cardAuth15"] = 1;
	if (runCardAuth["cardAuth15"] == 0) return 2000;
	TestPinpad();
	if (runCardAuth["cardAuth15"] == 0) return 2000;
	Logger("\ncard_authorize15");
	std::string track;

	if (track2 != nullptr)
		track = track2;

	if (runCardAuth["cardAuth15"] == 0) return 2000;
	StartWork(auth_answer.ans, lastResponsePax, str, runCardAuth);

	rStr = str;
	OemToCharBuffA(str.c_str(), &rStr[0], str.size());
	check = rStr;
	auth_answer.ans.Check = check.data();

	if (runCardAuth["cardAuth15"] == 0) return 2000;

	char errCode[] = { lastResponsePax[0], lastResponsePax[1] };
	int RCode = *((unsigned short*)errCode);
	ParsingResponseResCard(resRecCard, lastResponsePax);

	for (int i = 0; i < 7; i++)
		auth_answer.AuthCode[i] = resRecCard[CODE_AUTH][i];

	for (int i = 0; i < 20; i++)
		auth_answer.CardID[i] = resRecCard[NUMBER_CARD][i];

	auth_answer.ErrorCode = RCode;

	for (int i = 0; i < 4; i++)
		auth_answer.TransDate[i] = resRecCard[DATE_OPERATION][i];

	for (int i = 0; i < 4; i++)
		auth_answer.TransDate[i] = resRecCard[TIME_OPERATION][i];

	std::string transNumber(resRecCard[NUMBER_OPERATION_DAY].begin(), resRecCard[NUMBER_OPERATION_DAY].end());
	auth_answer.TransNumber = std::stoi(transNumber);

	auth_answer.SberOwnCard = resRecCard[BANK_AFFILIATION][0];

	for (int i = 0; i < 20; i++)
		auth_answer.Hash[i] = resRecCard[HASH_CARD][i];

	for (int i = 0; i < 13; i++)
		auth_answer.RRN[i] = resRecCard[RRN][i];

	for (int i = 0; i < 32; i++)
		auth_answer.CardName[i] = resRecCard[NAME_CARD][i];

	for (int i = 0; i < 3; i++)
		auth_answer.ans.RCode[i] = std::to_string(RCode)[i];;

	std::string result;

	if (RCode == 0)
		result = "��������";
	else
		result = "���������";

	for (int i = 0; i < 16; i++)
		auth_answer.ans.AMessage[i] = result[i];

	Logger("End command");

	return RCode;
}

CONTEXT_PTR PilotService::ctxAlloc(std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context)
{
	Logger("\nctxAlloc");
	CONTEXT_PTR ctx = 0;
	CONTEXT_PTR addr = (CONTEXT_PTR)&ctx;

	//Logger("������ �������� �� ������: " + addr);
	std::cout << "������ �������� �� ������: " << addr << std::endl;

	auth_answer14 argument;
	memset(&argument, 0, sizeof(argument));

	std::vector<std::unordered_map<std::string, std::vector<unsigned char>>> listTagExtraData;
	dContext[addr] = argument;
	DTagExtraData[addr] = listTagExtraData;
	map_context = DTagExtraData;
	Logger("End command");
	return addr;
}

void PilotService::ctxClear(CONTEXT_PTR ctx)
{
	Logger("\nctxClear");
	auth_answer14 argument;
	memset(&argument, 0, sizeof(argument));

	dContext[ctx] = argument;
	Logger("End command");
}

void PilotService::ctxFree(CONTEXT_PTR ctx, std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context)
{
	Logger("\nctxFree");
	dContext.erase(ctx);
	DTagExtraData.erase(ctx);
	map_context.erase(ctx);
	Logger("End command");
}

int PilotService::ctxGetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* pVal, int* pOutSz, int MAXSIZE, std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context)
{
	Logger("\nctxGetBinary");
	std::vector<std::unordered_map<std::string, std::vector<unsigned char>>> listTagExtraData;
	std::unordered_map<std::string, std::vector<unsigned char>> uMap;

	switch (name)
	{
	case PAR_ENCRYPTED_DATA:
	{
		DTagExtraData = map_context;
		listTagExtraData = DTagExtraData[ctx];
		uMap = listTagExtraData[0];
		std::vector<unsigned char> ucVector = uMap["EncryptedData"];
		pVal = &ucVector[0];
		*pOutSz = ucVector.size();

	}
	break;
	default:
		break;
	}
	Logger("End command");
	return 0;
}

int PilotService::ctxGetInt(CONTEXT_PTR ctx, EParameterName name, int* pVal, std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context)
{
	Logger("\nctxGetInt");
	std::vector<std::unordered_map<std::string, std::vector<unsigned char>>> listTagExtraData;
	std::unordered_map<std::string, std::vector<unsigned char>> uMap;
;

	switch (name)
	{
	case PAR_OWN_CARD:
	{
		auth_answer14 argument;
		memset(&argument, 0, sizeof(argument));
		argument = dContext[ctx];
		*pVal = argument.SberOwnCard;
	}
		break;
	case PAR_EXPIRY_DATE:
	{
		DTagExtraData = map_context;
		listTagExtraData = DTagExtraData[ctx];
		uMap = listTagExtraData[0];
		std::string s(uMap["ExpiryDate"].begin(), uMap["ExpiryDate"].end());
		std::erase(s, '/');
		s = s[2] + s[3] + s[0] + s[1];
		int number = std::stoi(s);
		*pVal = number;
	}
		break;
	default:
		break;
	}
	Logger("End command");
	return 0;
}

int PilotService::ctxGetString(CONTEXT_PTR ctx, 
	EParameterName name, 
	char* str, 
	int sz, 
	std::string& _hash, 
	std::string& _par_pan,
	std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context)
{
	Logger("\nctxGetString");
	Logger(std::to_string(name));
	std::vector<std::unordered_map<std::string, std::vector<unsigned char>>> listTagExtraData;
	std::unordered_map<std::string, std::vector<unsigned char>> uMap;

	switch (name)
	{
	case PAR_PAN:
	{
		DTagExtraData = map_context;
		listTagExtraData = DTagExtraData[ctx];
		uMap = listTagExtraData[0];
		std::string s(uMap["pan"].begin(), uMap["pan"].end());
		_par_pan = s;
		for (int i = 0; i < _par_pan.size(); i++)
			str[i] = _par_pan[i];
		Logger(_par_pan);
	}
		break;
	case PAR_HASH:
	{
		std::string hash{"H"};
		std::stringstream symbol;
		symbol.setf(std::ios::hex, std::ios::basefield);
		symbol.unsetf(std::ios::showbase);

		DTagExtraData = map_context;
		listTagExtraData = DTagExtraData[ctx];
		uMap = listTagExtraData[0];

		for (auto& a : uMap["hash"])
			symbol << std::hex << std::uppercase << int(a);

		hash = symbol.str();
		_hash = "H" + hash;
		for (int i = 0; i < _hash.size(); i++)
			str[i] = _hash[i];
		Logger(_hash);
	}
		break;
	case PAR_EXPIRY_DATE:
	{
		DTagExtraData = map_context;
		listTagExtraData = DTagExtraData[ctx];
		uMap = listTagExtraData[0];
		std::string s(uMap["ExpiryDate"].begin(), uMap["ExpiryDate"].end());
		std::erase(s, '/');
		str = s.data();
	}
		break;
	default:
		break;
	}
	Logger("End command");
	return 0;
}

int PilotService::ctxSetBinary(CONTEXT_PTR ctx, 
	EParameterName name, 
	unsigned char* val, 
	int sz, 
	std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context)
{
	Logger("\nctxSetBinary");
	std::vector<unsigned char> ucVector;
	for (int i = 0; i < sz; i++)
		ucVector.push_back(val[i]);

	std::vector<std::unordered_map<std::string, std::vector<unsigned char>>> listTagExtraData;

	switch (name)
	{
	case PAR_ENCRYPTED_DATA:
	{
		listTagExtraData = DTagExtraData[ctx];
		std::unordered_map<std::string, std::vector<unsigned char>> uMap = {{ "EncryptedData", ucVector }};
		listTagExtraData.push_back(uMap);
		DTagExtraData[ctx] = listTagExtraData;
		map_context = DTagExtraData;
	}
		break;
	default:
		break;
	}
	Logger("End command");
	return 0;
}

int PilotService::ctxSetInt(CONTEXT_PTR ctx, 
	EParameterName name, 
	int val, 
	std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context)
{
	Logger("\nctxSetInt");
	std::vector<std::unordered_map<std::string, std::vector<unsigned char>>> listTagExtraData;
	auth_answer14 argument;
	memset(&argument, 0, sizeof(argument));
	argument = dContext[ctx];

	switch (name)
	{
	case PAR_CARD_TYPE:
		argument.ans.CType = val;
		break;
	case PAR_OWN_CARD:
		argument.SberOwnCard = val;
		break;
	case PAR_AMOUNT_OTHER:
	{
		std::vector<unsigned char> amountOther =
		{
			(unsigned char)val,
			(unsigned char)(val >> 8),
			(unsigned char)(val >> 16),
			(unsigned char)(val >> 24),
		};

		listTagExtraData = DTagExtraData[ctx];
		std::unordered_map<std::string, std::vector<unsigned char>> uMap = { { "AmountOther", amountOther } };
		listTagExtraData.push_back(uMap);
		DTagExtraData[ctx] = listTagExtraData;
		map_context = DTagExtraData;
	}
		break;
	default:
		break;
	}

	dContext[ctx] = argument;
	Logger("End command");
	return 0;
}

int PilotService::ctxSetString(CONTEXT_PTR ctx, 
	EParameterName name, 
	const char* str, 
	std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context)
{
	Logger("\nctxSetString");
	std::vector<std::unordered_map<std::string, std::vector<unsigned char>>> listTagExtraData;

	switch (name)
	{
	case PAR_BASKET_ID:
	{
		std::string s(str);
		std::vector<unsigned char> baskedId(s.begin(), s.end());
		listTagExtraData = DTagExtraData[ctx];
		std::unordered_map<std::string, std::vector<unsigned char>> uMap = { {"BasketId", baskedId} };
		listTagExtraData.push_back(uMap);
		DTagExtraData[ctx] = listTagExtraData;
		map_context = DTagExtraData;
	}
		break;
	default:
		break;
	}
	Logger("End command");
	return 0;
}

int PilotService::ReadCardContext(CONTEXT_PTR dataOut, 
	std::unordered_map<CONTEXT_PTR, 
	std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context)
{
	this->TestPinpad();
	Logger("\nReadCardContext");
	//std::cout << "\nReadCardContext" << std::endl;

	std::vector<unsigned char> lastResponsePax;
	auth_answer argument;
	memset(&argument, 0, sizeof(argument));

	argument.TType = OP_READ_CARD;
	argument.Amount = 0;
	argument.CType = 0;

	StartWork(argument, lastResponsePax, str);

	rStr = str;
	OemToCharBuffA(str.c_str(), &rStr[0], str.size());
	argument.Check = &rStr[0];

	for (int i = 0; i < 400; i++)
		std::cout << argument.Check[i];

	char errCode[] = { lastResponsePax[0], lastResponsePax[1] };
	int RCode = *((unsigned short*)errCode);

	ParsingResponseResCard(resRecCard, lastResponsePax);

	std::vector<std::unordered_map<std::string, std::vector<unsigned char>>> list = DTagExtraData[dataOut];
	std::unordered_map<std::string, std::vector<unsigned char>> uMap =
	{
		{ "pan", resRecCard[NUMBER_CARD] },
		{ "ExpiryDate", resRecCard[CARD_DATE] },
		{ "hash", resRecCard[HASH_CARD] },
		{ "ownCard", resRecCard[BANK_AFFILIATION] },
		{ "EncryptedData", resRecCard[ECRYPTED_DATA] }
	};
	list.push_back(uMap);
	DTagExtraData[dataOut] = list;
	map_context = DTagExtraData;
	Logger("End command");
	return RCode;
}

int PilotService::CommitTrx(DWORD dwAmount, char* pAuthCode)
{
	lastResponsePax.clear();
	TestPinpad();
	Logger("\nCommitTrx");

	auth_answer argument;
	memset(&argument, 0, sizeof(argument));

	argument.TType = OP_COMMIT;
	argument.Amount = dwAmount;

	StartWork(argument, lastResponsePax, str);
	
	rStr = str;
	OemToCharBuffA(str.c_str(), &rStr[0], str.size());
	argument.Check = &rStr[0];

	for (int i = 0; i < 400; i++)
		std::cout << argument.Check[i];

	char errCode[] = { lastResponsePax[0], lastResponsePax[1] };
	int RCode = *((unsigned short*)errCode);
	Logger("End command");
	return RCode;
}

int PilotService::RollbackTrx(DWORD dwAmount, char* pAuthCode)
{
	TestPinpad();
	Logger("\nRollbackTrx");
	
	auth_answer argument;
	memset(&argument, 0, sizeof(argument));

	argument.TType = OP_ROLLBACK;
	argument.Amount = dwAmount;

	StartWork(argument, lastResponsePax, str);
	
	rStr = str;
	OemToCharBuffA(str.c_str(), &rStr[0], str.size());
	argument.Check = &rStr[0];

	for (int i = 0; i < 400; i++)
		std::cout << argument.Check[i];

	char errCode[] = { lastResponsePax[0], lastResponsePax[1] };
	int RCode = *((unsigned short*)errCode);
	Logger("End command");
	return RCode;
}

int PilotService::SuspendTrx(DWORD dwAmount, char* pAuthCode)
{
	this->TestPinpad();
	Logger("\nSuspendTrx");
		
	auth_answer argument;
	memset(&argument, 0, sizeof(argument));

	argument.TType = OP_SUSPEND;
	argument.Amount = dwAmount;

	StartWork(argument, lastResponsePax, str);
	
	rStr = str;
	OemToCharBuffA(str.c_str(), &rStr[0], str.size());
	argument.Check = &rStr[0];

	for (int i = 0; i < 400; i++)
		std::cout << argument.Check[i];

	char errCode[] = { lastResponsePax[0], lastResponsePax[1] };
	int RCode = *((unsigned short*)errCode);
	Logger("End command");
	return RCode;
}

int PilotService::AbortTransaction()
{
	Logger("\nAbortTransaction");
	ComPort com_port;
	std::vector<unsigned char> frame = GetFrameGetReady(0x0000);

	GetFrameWithCrc16(frame);

	std::string resFrame = "\u0002!" + base64_encode(&frame[0], frame.size()) + "\u0003";
	
	com_port.open_port();
	Logger("-> " + resFrame);
	com_port.write_port(&resFrame[0], resFrame.length());
	com_port.close_port();

	for (const auto& c : runCardAuth)
	{
		runCardAuth[c.first] = 0;
	}

	Logger("End command");
	return 2000;
}

void PilotService::Done()
{
	Logger("\nDone");	
	Logger("End command");
}

int PilotService::BodyWorkPilotTrx(auth_answer& auth_answer, std::vector<unsigned char>& response, std::vector<unsigned char>& lastResponsePax, std::string& str, std::unordered_map<std::string, int>& runCardAuth)
{
	int codeInstruction;
	int codeDevice;
	bool loop = true;
	std::string ipString = GetIp(response);
	std::wstring stemp = std::wstring(ipString.begin(), ipString.end());
	LPCWSTR ip = stemp.c_str();
	int port = GetPort(response);
	std::vector<unsigned char> frame, serialNumber;
	std::string outDataPax = "";
	std::string resFramePax = "";
	std::vector<char> outDataTCP;
	std::vector<char> inDataTCP;
	std::vector<unsigned char> check;

	TCPClient tcp_client{ ip, port };
	ComPort com_port;
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	while (loop)
	{
		if (runCardAuth["cardAuth15"] == 0) return 2000;
		codeInstruction = response[9];
		codeDevice = response[10];
		switch (codeInstruction)
		{
		case MMCMD_OPEN:
			std::cout << "MMCD_OPEN" << std::endl;
			if (codeDevice == MDC_LAN)
			{
				std::cout << "MDC_LAN" << std::endl;
				tcp_client.open_tcp(ip, port);
				GetSerialNumberMessage(response, serialNumber);
				frame = GetFrameNewHostMasterCall(serialNumber, 25);
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				com_port.io_port(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			else if (codeDevice == MDC_PRINTER)
			{
				std::cout << "MDC_PRINTER" << std::endl;
				GetSerialNumberMessage(response, serialNumber);
				frame = GetFrameNewHostMasterCall(serialNumber, 3);
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				com_port.io_port(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			break;
		case MMCMD_READ:
			std::cout << "MMCD_READ" << std::endl;
			if (codeDevice == MDC_LAN)
			{
				std::cout << "MDC_LAN" << std::endl;
				outDataTCP.clear();
				Logger("������ ������ TCP");
				tcp_client.read_tcp(outDataTCP, GetSizeBuff(response));

				for (char c : outDataTCP)
				{
					std::cout << std::hex << std::uppercase << int(c) << " ";
				}
				std::cout << std::endl;

				GetSerialNumberMessage(response, serialNumber);
				frame = GetFrameReadTCPMasterCall(serialNumber, outDataTCP);
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				com_port.io_port(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			else if (codeDevice == MDC_PINGINFO)
			{
				std::cout << "MDC_PINGINFO" << std::endl;
				GetSerialNumberMessage(response, serialNumber);
				frame = GetFramePingInfoMasterCall(serialNumber);
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				com_port.open_port();
				Logger("-> " + resFramePax);
				com_port.write_port(&resFramePax[0], resFramePax.length());
				com_port.close_port();

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();

				frame = GetFramePingInfoTwoMessageMasterCall();
				GetFrameWithCrc16(frame);

				resFramePax = "\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				com_port.io_port(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			break;
		case MMCMD_WRITE:
			std::cout << "MMCD_WRITE" << std::endl;
			if (codeDevice == MDC_LAN)
			{
				std::cout << "MDC_LAN" << std::endl;
				while (true)
				{
					if (response[0] == 0)
					{
						GetSerialNumberMessage(response, serialNumber);
						GetDataForHost(response, 14, inDataTCP);
						break;
					}
					else if (response[0] == 128)
					{
						GetSerialNumberMessage(response, serialNumber);
						GetDataForHost(response, 14, inDataTCP);

						resFramePax = "\u0006";

						com_port.io_port(resFramePax, outDataPax);
						response = GetBinaryOutData(outDataPax);

						frame.clear();
						outDataPax = "";
						resFramePax = "";

						GetDataForHost(response, 2, inDataTCP);

						resFramePax = "\u0007";

						com_port.io_port(resFramePax, outDataPax);
						response = GetBinaryOutData(outDataPax);

						frame.clear();
						outDataPax = "";
						resFramePax = "";
					}
					else
					{
						GetDataForHost(response, 2, inDataTCP);
						break;
					}
				}

				Logger("������ ������ TCP");
				tcp_client.write_tcp(inDataTCP, inDataTCP.size());

				frame = GetFrameWriteTCPMasterCall(serialNumber, inDataTCP.size());
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				com_port.io_port(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			else if (codeDevice == MDC_PRINTER)
			{
				std::cout << "MDC_PRINTER" << std::endl;
				GetRowCheck(response, check);

				GetSerialNumberMessage(response, serialNumber);
				frame = GetFrameWriteToCheckMasterCall(serialNumber);
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				com_port.io_port(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			break;
		case MMCMD_CLOSE:
			std::cout << "MMCD_CLOSE" << std::endl;
			if (codeDevice == MDC_LAN)
			{
				std::cout << "MDC_LAN" << std::endl;
				GetSerialNumberMessage(response, serialNumber);
				frame = GetFrameCloseTCPMasterCall(serialNumber);
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				com_port.io_port(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);
				tcp_client.close_tcp();

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			else if (codeDevice == MDC_PRINTER)
			{
				std::cout << "MDC_PRINTER" << std::endl;
				GetSerialNumberMessage(response, serialNumber);
				frame = GetFraneCloseToCheckMasterCall(serialNumber);
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				com_port.io_port(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			break;
		default:
			std::cout << "DEFAULT" << std::endl;
			loop = false;
			GetLastResponsePax(response, lastResponsePax, 9);

			resFramePax = "\u0006";

			com_port.io_port(resFramePax, outDataPax);
			response = GetBinaryOutData(outDataPax);

			while (true)
			{
				if (response[0] == 128)
				{
					GetLastResponsePax(response, lastResponsePax, 9);

					resFramePax = "\u0006";

					com_port.io_port(resFramePax, outDataPax);
					response = GetBinaryOutData(outDataPax);

					frame.clear();
					outDataPax = "";
					resFramePax = "";

					GetLastResponsePax(response, lastResponsePax, 2);

					resFramePax = "\u0007";

					com_port.io_port(resFramePax, outDataPax);
					response = GetBinaryOutData(outDataPax);

					frame.clear();
					outDataPax = "";
					resFramePax = "";
				}
				else
				{
					GetLastResponsePax(response, lastResponsePax, 2);
					char errCode[] = { lastResponsePax[0], lastResponsePax[1] };
					int RCode = *((unsigned short*)errCode);

					if (RCode != 0)
						return RCode;

					break;
				}
			}
			break;
		}
	}

	check.push_back('\0');
	std::string strCheck(check.begin(), check.end());
	check.clear();
	str = strCheck;

	return 0;
}

int PilotService::BodyWorkPilotTrx(auth_answer& auth_answer, std::vector<unsigned char>& response, std::vector<unsigned char>& lastResponsePax, std::string& str)
{
	int codeInstruction;
	int codeDevice;
	bool loop = true;
	std::string ipString = GetIp(response);
	std::wstring stemp = std::wstring(ipString.begin(), ipString.end());
	LPCWSTR ip = stemp.c_str();
	int port = GetPort(response);
	std::vector<unsigned char> frame, serialNumber;
	std::string outDataPax = "";
	std::string resFramePax = "";
	std::vector<char> outDataTCP;
	std::vector<char> inDataTCP;
	std::vector<unsigned char> check;

	TCPClient tcp_client{ ip, port };
	ComPort com_port;

	while (loop)
	{
		codeInstruction = response[9];
		codeDevice = response[10];
		switch (codeInstruction)
		{
		case MMCMD_OPEN:
			std::cout << "MMCD_OPEN" << std::endl;
			if (codeDevice == MDC_LAN)
			{
				std::cout << "MDC_LAN" << std::endl;
				tcp_client.open_tcp(ip, port);
				GetSerialNumberMessage(response, serialNumber);
				frame = GetFrameNewHostMasterCall(serialNumber, 25);
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				com_port.io_port(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			else if (codeDevice == MDC_PRINTER)
			{
				std::cout << "MDC_PRINTER" << std::endl;
				GetSerialNumberMessage(response, serialNumber);
				frame = GetFrameNewHostMasterCall(serialNumber, 3);
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				com_port.io_port(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			break;
		case MMCMD_READ:
			std::cout << "MMCD_READ" << std::endl;
			if (codeDevice == MDC_LAN)
			{
				std::cout << "MDC_LAN" << std::endl;
				outDataTCP.clear();
				Logger("������ ������ TCP");
				tcp_client.read_tcp(outDataTCP, GetSizeBuff(response));

				for (char c : outDataTCP)
				{
					std::cout << std::hex << std::uppercase << int(c) << " ";
				}
				std::cout << std::endl;

				GetSerialNumberMessage(response, serialNumber);
				frame = GetFrameReadTCPMasterCall(serialNumber, outDataTCP);
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				com_port.io_port(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			else if (codeDevice == MDC_PINGINFO)
			{
				std::cout << "MDC_PINGINFO" << std::endl;
				GetSerialNumberMessage(response, serialNumber);
				frame = GetFramePingInfoMasterCall(serialNumber);
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";


				com_port.open_port();
				Logger("-> " + resFramePax);
				com_port.write_port(&resFramePax[0], resFramePax.length());
				com_port.close_port();

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();

				frame = GetFramePingInfoTwoMessageMasterCall();
				GetFrameWithCrc16(frame);

				resFramePax = "\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				com_port.io_port(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			break;
		case MMCMD_WRITE:
			std::cout << "MMCD_WRITE" << std::endl;
			if (codeDevice == MDC_LAN)
			{
				std::cout << "MDC_LAN" << std::endl;
				while (true)
				{
					if (response[0] == 0)
					{
						GetSerialNumberMessage(response, serialNumber);
						GetDataForHost(response, 14, inDataTCP);
						break;
					}
					else if (response[0] == 128)
					{
						GetSerialNumberMessage(response, serialNumber);
						GetDataForHost(response, 14, inDataTCP);

						resFramePax = "\u0006";

						com_port.io_port(resFramePax, outDataPax);
						response = GetBinaryOutData(outDataPax);

						frame.clear();
						outDataPax = "";
						resFramePax = "";

						GetDataForHost(response, 2, inDataTCP);

						resFramePax = "\u0007";

						com_port.io_port(resFramePax, outDataPax);
						response = GetBinaryOutData(outDataPax);

						frame.clear();
						outDataPax = "";
						resFramePax = "";
					}
					else
					{
						GetDataForHost(response, 2, inDataTCP);
						break;
					}
				}

				Logger("������ ������ TCP");
				tcp_client.write_tcp(inDataTCP, inDataTCP.size());

				frame = GetFrameWriteTCPMasterCall(serialNumber, inDataTCP.size());
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				com_port.io_port(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			else if (codeDevice == MDC_PRINTER)
			{
				std::cout << "MDC_PRINTER" << std::endl;
				GetRowCheck(response, check);

				GetSerialNumberMessage(response, serialNumber);
				frame = GetFrameWriteToCheckMasterCall(serialNumber);
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				com_port.io_port(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			break;
		case MMCMD_CLOSE:
			std::cout << "MMCD_CLOSE" << std::endl;
			if (codeDevice == MDC_LAN)
			{
				std::cout << "MDC_LAN" << std::endl;
				GetSerialNumberMessage(response, serialNumber);
				frame = GetFrameCloseTCPMasterCall(serialNumber);
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				com_port.io_port(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);
				tcp_client.close_tcp();

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			else if (codeDevice == MDC_PRINTER)
			{
				std::cout << "MDC_PRINTER" << std::endl;
				GetSerialNumberMessage(response, serialNumber);
				frame = GetFraneCloseToCheckMasterCall(serialNumber);
				GetFrameWithCrc16(frame);

				resFramePax = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

				com_port.io_port(resFramePax, outDataPax);
				response = GetBinaryOutData(outDataPax);

				frame.clear();
				serialNumber.clear();
				outDataPax = "";
				resFramePax = "";
				outDataTCP.clear();
				inDataTCP.clear();
			}
			break;
		default:
			std::cout << "DEFAULT" << std::endl;
			loop = false;
			GetLastResponsePax(response, lastResponsePax, 9);

			resFramePax = "\u0006";

			com_port.io_port(resFramePax, outDataPax);
			response = GetBinaryOutData(outDataPax);

			while (true)
			{
				if (response[0] == 128)
				{
					GetLastResponsePax(response, lastResponsePax, 9);

					resFramePax = "\u0006";

					com_port.io_port(resFramePax, outDataPax);
					response = GetBinaryOutData(outDataPax);

					frame.clear();
					outDataPax = "";
					resFramePax = "";

					GetLastResponsePax(response, lastResponsePax, 2);

					resFramePax = "\u0007";

					com_port.io_port(resFramePax, outDataPax);
					response = GetBinaryOutData(outDataPax);

					frame.clear();
					outDataPax = "";
					resFramePax = "";
				}
				else
				{
					GetLastResponsePax(response, lastResponsePax, 2);
					char errCode[] = { lastResponsePax[0], lastResponsePax[1] };
					int RCode = *((unsigned short*)errCode);

					if (RCode != 0)
						return RCode;

					break;
				}
			}
			break;
		}
	}

	check.push_back('\0');
	std::string strCheck(check.begin(), check.end());
	check.clear();
	str = strCheck;

	return 0;
}

int PilotService::StartWork(auth_answer& auth_answe, std::vector<unsigned char>& lastResponsePax, std::string& str, std::unordered_map<std::string, int>& runCardAuth)
{
	std::string outData = "";
	std::vector<unsigned char> frame = GetFrameTrx(auth_answe);
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	GetFrameWithCrc16(frame);
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	std::string resFrame = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

	ComPort com_port;
	com_port.io_port(resFrame, outData);
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	std::vector<unsigned char> response = GetBinaryOutData(outData);
	if (runCardAuth["cardAuth15"] == 0) return 2000;
	int result = BodyWorkPilotTrx(auth_answe, response, lastResponsePax, str, runCardAuth);
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	outData = "";
	response.clear();
	return result;
}

int PilotService::StartWork(auth_answer& auth_answe, std::vector<unsigned char>& lastResponsePax, std::string& str)
{
	std::string outData = "";
	std::vector<unsigned char> frame = GetFrameTrx(auth_answe);

	GetFrameWithCrc16(frame);

	std::string resFrame = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

	ComPort com_port;
	com_port.io_port(resFrame, outData);

	std::vector<unsigned char> response = GetBinaryOutData(outData);
	int result = BodyWorkPilotTrx(auth_answe, response, lastResponsePax, str);
	outData = "";
	response.clear();
	return result;
}

int PilotService::StartWork(auth_answer14& auth_answe, std::vector<unsigned char>& lastResponsePax, std::string& str, std::unordered_map<std::string, int>& runCardAuth)
{
	std::string outData = "";
	std::vector<unsigned char> frame = GetFrameTrx(auth_answe);
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	GetFrameWithCrc16(frame);
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	std::string resFrame = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

	ComPort com_port;
	com_port.io_port(resFrame, outData);
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	std::vector<unsigned char> response = GetBinaryOutData(outData);
	if (runCardAuth["cardAuth15"] == 0) return 2000;
	int result = BodyWorkPilotTrx(auth_answe.ans, response, lastResponsePax, str, runCardAuth);
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	outData = "";
	response.clear();
	return result;
}

int PilotService::StartWork(auth_answer14& auth_answe, std::vector<unsigned char>& lastResponsePax, std::string& str)
{
	std::string outData = "";
	std::vector<unsigned char> frame = GetFrameTrx(auth_answe);

	GetFrameWithCrc16(frame);

	std::string resFrame = "\u0004\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";

	ComPort com_port;
	com_port.io_port(resFrame, outData);

	std::vector<unsigned char> response = GetBinaryOutData(outData);
	int result = BodyWorkPilotTrx(auth_answe.ans, response, lastResponsePax, str);
	outData = "";
	response.clear();
	return result;
}

int PilotService::StartWork(auth_answer& auth_answe, SberCmd& sber_cmd, std::string& str, std::unordered_map<std::string, int>& runCardAuth)
{
	std::string outData = "";
	sber_cmd.RunTrx(auth_answe);
	if (runCardAuth["cardAuth15"] == 0) return 2000;
	sber_cmd.SetResFrame("\u0004\u0002#");

	ComPort com_port;
	std::string res_frame = sber_cmd.GetResFrame();
	com_port.io_port(res_frame, outData);
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	sber_cmd.SetBinaryOutData(outData);	
	if (runCardAuth["cardAuth15"] == 0) return 2000;
	int result = BodyWorkPilotTrx(auth_answe, sber_cmd, str, runCardAuth);
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	outData = "";
	return result;
}

int PilotService::StartWork(auth_answer& auth_answe, SberCmd& sber_cmd, std::string& str)
{
	std::string outData = "";
	sber_cmd.RunTrx(auth_answe);
	sber_cmd.SetResFrame("\u0004\u0002#");

	ComPort com_port;
	std::string res_frame = sber_cmd.GetResFrame();
	com_port.io_port(res_frame, outData);

	sber_cmd.SetBinaryOutData(outData);
	int result = BodyWorkPilotTrx(auth_answe, sber_cmd, str);
	outData = "";
	return result;
}

int PilotService::StartWork(auth_answer14& auth_answe, SberCmd& sber_cmd, std::string& str, std::unordered_map<std::string, int>& runCardAuth)
{
	std::string outData = "";
	sber_cmd.RunTrx(auth_answe);
	if (runCardAuth["cardAuth15"] == 0) return 2000;
	sber_cmd.SetResFrame("\u0004\u0002#");

	ComPort com_port;
	std::string res_frame = sber_cmd.GetResFrame();
	com_port.io_port(res_frame, outData);
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	sber_cmd.SetBinaryOutData(outData);
	if (runCardAuth["cardAuth15"] == 0) return 2000;
	int result = BodyWorkPilotTrx(auth_answe.ans, sber_cmd, str, runCardAuth);
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	outData = "";
	return result;;
}

int PilotService::StartWork(auth_answer14& auth_answe, SberCmd& sber_cmd, std::string& str)
{
	std::string outData = "";
	sber_cmd.RunTrx(auth_answe);
	sber_cmd.SetResFrame("\u0004\u0002#");

	ComPort com_port;
	std::string res_frame = sber_cmd.GetResFrame();
	com_port.io_port(res_frame, outData);

	sber_cmd.SetBinaryOutData(outData);
	int result = BodyWorkPilotTrx(auth_answe.ans, sber_cmd, str);
	outData = "";
	return result;
}

int PilotService::BodyWorkPilotTrx(auth_answer& auth_answer, SberCmd& sber_cmd, std::string& str, std::unordered_map<std::string, int>& runCardAuth)
{
	int code_instruction;
	int code_device;
	bool loop = true;
	std::vector<unsigned char> response = sber_cmd.GetResponse();
	std::string ip_string = GetIp(response);
	std::wstring stemp = std::wstring(ip_string.begin(), ip_string.end());
	LPCWSTR ip = stemp.c_str();
	int port = GetPort(response);
	std::string out_data_pax = "";
	std::string res_frame = "";
	std::vector<char> out_data_tcp;
	std::vector<char> in_data_tcp;
	std::vector<unsigned char> check;

	TCPClient tcp_client{ ip, port };
	ComPort com_port;
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	while (loop)
	{
		if (runCardAuth["cardAuth15"] == 0) return 2000;		
		code_instruction = sber_cmd.GetResponse()[9];
		code_device = sber_cmd.GetResponse()[10];
		switch (code_instruction)
		{
		case MMCMD_OPEN:
			std::cout << "MMCMD_OPEN" << std::endl;
			if (code_device == MDC_LAN)
			{
				std::cout << "MDC_LAN" << std::endl;
				tcp_client.open_tcp(ip, port);

				sber_cmd.RunNewHostMasterCall(25);
				sber_cmd.SetResFrame("\u0004\u0002#");
				res_frame = sber_cmd.GetResFrame();
				com_port.io_port(res_frame, out_data_pax);
				sber_cmd.SetBinaryOutData(out_data_pax);				

				sber_cmd.GetFrame().clear();
				out_data_pax = "";
				out_data_tcp.clear();
				in_data_tcp.clear();
			}
			else if (code_device == MDC_PRINTER)
			{
				std::cout << "MDC_PRINTER" << std::endl;
				sber_cmd.RunNewHostMasterCall(3);
				sber_cmd.SetResFrame("\u0004\u0002#");
				res_frame = sber_cmd.GetResFrame();
				com_port.io_port(res_frame, out_data_pax);
				sber_cmd.SetBinaryOutData(out_data_pax);

				sber_cmd.GetFrame().clear();
				out_data_pax = "";
				out_data_tcp.clear();
				in_data_tcp.clear();
			}
			break;
		case MMCMD_READ:
			std::cout << "MMCMD_READ" << std::endl;
			if (code_device == MDC_LAN)
			{
				std::cout << "MDC_LAN" << std::endl;
				out_data_tcp.clear();
				Logger("������ ������ TCP");
				std::cout << "������ �����x TCP" << std::endl;
				tcp_client.read_tcp(out_data_tcp, GetSizeBuff(sber_cmd.GetResponse()));

				for (char c : out_data_tcp)
				{
					std::cout << std::hex << std::uppercase << int(c) << " ";
				}
				std::cout << std::endl;

				sber_cmd.RunReadTCPMasterCall(out_data_tcp);
				sber_cmd.SetResFrame("\u0004\u0002#");
				res_frame = sber_cmd.GetResFrame();
				com_port.io_port(res_frame, out_data_pax);
				sber_cmd.SetBinaryOutData(out_data_pax);

				sber_cmd.GetFrame().clear();
				out_data_pax = "";
				out_data_tcp.clear();
				in_data_tcp.clear();
			}
			else if (code_device == MDC_PINGINFO)
			{
				std::cout << "MDC_PINGINFO" << std::endl;
				sber_cmd.RunPingInfoMasterCall();
				sber_cmd.SetResFrame("\u0004\u0002#");
				res_frame = sber_cmd.GetResFrame();
				com_port.open_port();
				Logger("-> " + res_frame);
				com_port.write_port(&res_frame[0], res_frame.length());
				com_port.close_port();
				
				sber_cmd.GetFrame().clear();
				out_data_pax = "";				
				out_data_tcp.clear();
				in_data_tcp.clear();

				sber_cmd.RunPingInfoTwoMessageMasterCall();
				sber_cmd.SetResFrame("\u0004\u0002#");
				res_frame = sber_cmd.GetResFrame();
				com_port.io_port(res_frame, out_data_pax);
				sber_cmd.SetBinaryOutData(out_data_pax);
				
				sber_cmd.GetFrame().clear();
				out_data_pax = "";
				out_data_tcp.clear();
				in_data_tcp.clear();
			}
			break;
		case MMCMD_WRITE:
			std::cout << "MMCMD_WRITE" << std::endl;
			if (code_device == MDC_LAN)
			{
				std::cout << "MDC_LAN" << std::endl;
				while (true)
				{
					if (sber_cmd.GetResponse()[0] == 0)
					{	
						std::cout << "��������� ��� ������� response[0] == 0" << std::endl;
						GetDataForHost(sber_cmd.GetResponse(), 14, in_data_tcp);
						break;
					}
					else if (sber_cmd.GetResponse()[0] == 128)
					{
						std::cout << "��������� ��� ������� response[0] == 128" << std::endl;
						sber_cmd.SetFirstResponse();
						GetDataForHost(sber_cmd.GetResponse(), 14, in_data_tcp);
						res_frame = "\u0006";
						com_port.io_port(res_frame, out_data_pax);
						sber_cmd.SetBinaryOutData(out_data_pax);

						out_data_pax = "";

						GetDataForHost(sber_cmd.GetResponse(), 2, in_data_tcp);
						res_frame = "\u0007";
						com_port.io_port(res_frame, out_data_pax);
						sber_cmd.SetBinaryOutData(out_data_pax);

						out_data_pax = "";
					}
					else
					{
						std::cout << "��������� ��� ��������� ��������" << std::endl;
						GetDataForHost(sber_cmd.GetResponse(), 2, in_data_tcp);
						break;
					}
				}

				Logger("������ ������ TCP");
				std::cout << "������ ������ TCP" << std::endl;
				tcp_client.write_tcp(in_data_tcp, in_data_tcp.size());
				sber_cmd.RunWriteTCPMasterCall(in_data_tcp.size());
				sber_cmd.ClearFirstResponse();
				sber_cmd.SetResFrame("\u0004\u0002#");
				res_frame = sber_cmd.GetResFrame();
				com_port.io_port(res_frame, out_data_pax);
				sber_cmd.SetBinaryOutData(out_data_pax);

				sber_cmd.GetFrame().clear();
				out_data_pax = "";
				out_data_tcp.clear();
				in_data_tcp.clear();
			}
			else if (code_device == MDC_PRINTER)
			{
				std::cout << "MDC_PRINTER" << std::endl;
				GetRowCheck(sber_cmd.GetResponse(), check);
				sber_cmd.RunWriteToCheckMasterCall();
				sber_cmd.SetResFrame("\u0004\u0002#");
				res_frame = sber_cmd.GetResFrame();
				com_port.io_port(res_frame, out_data_pax);
				sber_cmd.SetBinaryOutData(out_data_pax);
				
				sber_cmd.GetFrame().clear();
				out_data_pax = "";
				out_data_tcp.clear();
				in_data_tcp.clear();
			}
			break;
		case MMCMD_CLOSE:
			std::cout << "MMCMD_CLOSE" << std::endl;
			if (code_device == MDC_LAN)
			{
				std::cout << "MDC_LAN" << std::endl;
				sber_cmd.RunCloseTCPMasterCall();
				sber_cmd.SetResFrame("\u0004\u0002#");
				res_frame = sber_cmd.GetResFrame();
				com_port.io_port(res_frame, out_data_pax);
				sber_cmd.SetBinaryOutData(out_data_pax);
				tcp_client.close_tcp();

				out_data_pax = "";
				out_data_tcp.clear();
				in_data_tcp.clear();
			}
			else if (code_device == MDC_PRINTER)
			{
				std::cout << "MDC_PRINTER" << std::endl;
				sber_cmd.RunCloseToCheckMasterCall();
				sber_cmd.SetResFrame("\u0004\u0002#");
				res_frame = sber_cmd.GetResFrame();
				com_port.io_port(res_frame, out_data_pax);
				sber_cmd.SetBinaryOutData(out_data_pax);

				sber_cmd.GetFrame().clear();
				out_data_pax = "";
				out_data_tcp.clear();
				in_data_tcp.clear();
			}
			break;
		default:
			std::cout << "DEFAULT" << std::endl;
			loop = false;
			sber_cmd.SetLastResponse(9);
			res_frame = "\u0006";
			com_port.io_port(res_frame, out_data_pax);
			sber_cmd.SetBinaryOutData(out_data_pax);

			while (true)
			{
				if (sber_cmd.GetResponse()[0] == 128)
				{
					sber_cmd.SetLastResponse(9);
					res_frame = "\u0006";
					com_port.io_port(res_frame, out_data_pax);
					sber_cmd.SetBinaryOutData(out_data_pax);
					
					out_data_pax = "";

					sber_cmd.SetLastResponse(2);
					res_frame = "\u0007";
					com_port.io_port(res_frame, out_data_pax);
					sber_cmd.SetBinaryOutData(out_data_pax);
					
					out_data_pax = "";
				}
				else
				{
					sber_cmd.SetLastResponse(2);
					char errCode[] = { sber_cmd.GetLastResponse()[0], sber_cmd.GetLastResponse()[1] };
					int RCode = *((unsigned short*)errCode);

					if (RCode != 0)
						return RCode;

					break;
				}
			}
			break;
		}
	}

	check.push_back('\0');
	std::string strCheck(check.begin(), check.end());
	check.clear();
	str = strCheck;

	return 0;
}

int PilotService::BodyWorkPilotTrx(auth_answer& auth_answer, SberCmd& sber_cmd, std::string& str)
{
	int code_instruction;
	int code_device;
	bool loop = true;
	std::vector<unsigned char> response = sber_cmd.GetResponse();
	std::string ip_string = GetIp(response);
	std::wstring stemp = std::wstring(ip_string.begin(), ip_string.end());
	LPCWSTR ip = stemp.c_str();
	int port = GetPort(response);
	std::string out_data_pax = "";
	std::string res_frame = "";
	std::vector<char> out_data_tcp;
	std::vector<char> in_data_tcp;
	std::vector<unsigned char> check;

	TCPClient tcp_client{ ip, port };
	ComPort com_port;

	while (loop)
	{
		code_instruction = sber_cmd.GetResponse()[9];
		code_device = sber_cmd.GetResponse()[10];
		switch (code_instruction)
		{
		case MMCMD_OPEN:
			std::cout << "MMCMD_OPEN" << std::endl;
			if (code_device == MDC_LAN)
			{
				std::cout << "MDC_LAN" << std::endl;
				tcp_client.open_tcp(ip, port);

				sber_cmd.RunNewHostMasterCall(25);
				sber_cmd.SetResFrame("\u0004\u0002#");
				res_frame = sber_cmd.GetResFrame();
				com_port.io_port(res_frame, out_data_pax);
				sber_cmd.SetBinaryOutData(out_data_pax);

				out_data_pax = "";
				out_data_tcp.clear();
				in_data_tcp.clear();
			}
			else if (code_device == MDC_PRINTER)
			{
				std::cout << "MDC_PRINTER" << std::endl;
				sber_cmd.RunNewHostMasterCall(3);
				sber_cmd.SetResFrame("\u0004\u0002#");
				res_frame = sber_cmd.GetResFrame();
				com_port.io_port(res_frame, out_data_pax);
				sber_cmd.SetBinaryOutData(out_data_pax);

				sber_cmd.GetFrame().clear();
				out_data_pax = "";
				out_data_tcp.clear();
				in_data_tcp.clear();
			}
			break;
		case MMCMD_READ:
			std::cout << "MMCMD_READ" << std::endl;
			if (code_device == MDC_LAN)
			{
				std::cout << "MDC_LAN" << std::endl;
				out_data_tcp.clear();
				Logger("������ ������ TCP");
				std::cout << "������ ������ TCP" << std::endl;
				tcp_client.read_tcp(out_data_tcp, GetSizeBuff(sber_cmd.GetResponse()));

				for (char c : out_data_tcp)
				{
					std::cout << std::hex << std::uppercase << int(c) << " ";
				}
				std::cout << std::endl;

				sber_cmd.RunReadTCPMasterCall(out_data_tcp);
				sber_cmd.SetResFrame("\u0004\u0002#");
				res_frame = sber_cmd.GetResFrame();
				com_port.io_port(res_frame, out_data_pax);
				sber_cmd.SetBinaryOutData(out_data_pax);

				sber_cmd.GetFrame().clear();
				out_data_pax = "";
				out_data_tcp.clear();
				in_data_tcp.clear();
			}
			else if (code_device == MDC_PINGINFO)
			{
				std::cout << "MDC_PINGINFO" << std::endl;
				sber_cmd.RunPingInfoMasterCall();
				sber_cmd.SetResFrame("\u0004\u0002#");
				res_frame = sber_cmd.GetResFrame();
				com_port.open_port();
				Logger("-> " + res_frame);
				com_port.write_port(&res_frame[0], res_frame.length());
				com_port.close_port();

				sber_cmd.GetFrame().clear();
				out_data_pax = "";
				out_data_tcp.clear();
				in_data_tcp.clear();

				sber_cmd.RunPingInfoTwoMessageMasterCall();
				sber_cmd.SetResFrame("\u0004\u0002#");
				res_frame = sber_cmd.GetResFrame();
				com_port.io_port(res_frame, out_data_pax);
				sber_cmd.SetBinaryOutData(out_data_pax);

				sber_cmd.GetFrame().clear();
				out_data_pax = "";
				out_data_tcp.clear();
				in_data_tcp.clear();
			}
			break;
		case MMCMD_WRITE:
			std::cout << "MMCMD_WRITE" << std::endl;
			if (code_device == MDC_LAN)
			{
				std::cout << "MDC_LAN" << std::endl;
				while (true)
				{
					if (sber_cmd.GetResponse()[0] == 0)
					{
						std::cout << "��������� ��� ������� response[0] == 0" << std::endl;
						GetDataForHost(sber_cmd.GetResponse(), 14, in_data_tcp);
						break;
					}
					else if (sber_cmd.GetResponse()[0] == 128)
					{
						std::cout << "��������� ��� ������� response[0] == 128" << std::endl;
						sber_cmd.SetFirstResponse();
						GetDataForHost(sber_cmd.GetResponse(), 14, in_data_tcp);
						res_frame = "\u0006";
						com_port.io_port(res_frame, out_data_pax);
						sber_cmd.SetBinaryOutData(out_data_pax);

						out_data_pax = "";

						GetDataForHost(sber_cmd.GetResponse(), 2, in_data_tcp);
						res_frame = "\u0007";
						com_port.io_port(res_frame, out_data_pax);
						sber_cmd.SetBinaryOutData(out_data_pax);

						out_data_pax = "";
					}
					else
					{
						std::cout << "��������� ��� ��������� ��������" << std::endl;
						GetDataForHost(sber_cmd.GetResponse(), 2, in_data_tcp);
						break;
					}
				}

				Logger("������ ������ TCP");
				std::cout << "������ ����� TCP" << std::endl;
				tcp_client.write_tcp(in_data_tcp, in_data_tcp.size());

				sber_cmd.RunWriteTCPMasterCall(in_data_tcp.size());
				sber_cmd.ClearFirstResponse();
				sber_cmd.SetResFrame("\u0004\u0002#");
				res_frame = sber_cmd.GetResFrame();
				com_port.io_port(res_frame, out_data_pax);
				sber_cmd.SetBinaryOutData(out_data_pax);

				sber_cmd.GetFrame().clear();
				out_data_pax = "";
				out_data_tcp.clear();
				in_data_tcp.clear();
			}
			else if (code_device == MDC_PRINTER)
			{
				std::cout << "MDC_PRINTER" << std::endl;
				GetRowCheck(sber_cmd.GetResponse(), check);
				sber_cmd.RunWriteToCheckMasterCall();
				sber_cmd.SetResFrame("\u0004\u0002#");
				res_frame = sber_cmd.GetResFrame();
				com_port.io_port(res_frame, out_data_pax);
				sber_cmd.SetBinaryOutData(out_data_pax);

				sber_cmd.GetFrame().clear();
				out_data_pax = "";
				out_data_tcp.clear();
				in_data_tcp.clear();
			}
			break;
		case MMCMD_CLOSE:
			std::cout << "MMCMD_CLOSE" << std::endl;
			if (code_device == MDC_LAN)
			{
				std::cout << "MDC_LAN" << std::endl;
				sber_cmd.RunCloseTCPMasterCall();
				sber_cmd.SetResFrame("\u0004\u0002#");
				res_frame = sber_cmd.GetResFrame();
				com_port.io_port(res_frame, out_data_pax);
				sber_cmd.SetBinaryOutData(out_data_pax);
				tcp_client.close_tcp();

				sber_cmd.GetFrame().clear();
				out_data_pax = "";
				out_data_tcp.clear();
				in_data_tcp.clear();
			}
			else if (code_device == MDC_PRINTER)
			{
				std::cout << "MDC_PRINTER" << std::endl;
				sber_cmd.RunCloseToCheckMasterCall();
				sber_cmd.SetResFrame("\u0004\u0002#");
				res_frame = sber_cmd.GetResFrame();
				com_port.io_port(res_frame, out_data_pax);
				sber_cmd.SetBinaryOutData(out_data_pax);

				sber_cmd.GetFrame().clear();
				out_data_pax = "";
				out_data_tcp.clear();
				in_data_tcp.clear();
			}
			break;
		default:
			std::cout << "DEFAULT" << std::endl;
			loop = false;
			sber_cmd.SetLastResponse(9);
			res_frame = "\u0006";
			com_port.io_port(res_frame, out_data_pax);
			sber_cmd.SetBinaryOutData(out_data_pax);

			while (true)
			{
				if (sber_cmd.GetResponse()[0] == 128)
				{
					sber_cmd.SetLastResponse(9);
					res_frame = "\u0006";
					com_port.io_port(res_frame, out_data_pax);
					sber_cmd.SetBinaryOutData(out_data_pax);

					out_data_pax = "";

					sber_cmd.SetLastResponse(2);
					res_frame = "\u0007";
					com_port.io_port(res_frame, out_data_pax);
					sber_cmd.SetBinaryOutData(out_data_pax);

					out_data_pax = "";
				}
				else
				{
					sber_cmd.SetLastResponse(2);
					char errCode[] = { sber_cmd.GetLastResponse()[0], sber_cmd.GetLastResponse()[1] };
					int RCode = *((unsigned short*)errCode);

					if (RCode != 0)
						return RCode;

					break;
				}
			}
			break;
		}
	}

	check.push_back('\0');
	std::string strCheck(check.begin(), check.end());
	check.clear();
	str = strCheck;

	return 0;
}
