#include "pch.h"
#include "PilotService.h"

int PilotService::TestPinpad()
{
	Logger("\nTestPinpad");
	//std::cout << "\nTestPinpad" << std::endl;

	std::string outData = "";
	std::vector<unsigned char> frame = GetFrameGetReady(0x0000);

	GetFrameWithCrc16(frame);

	std::string resFrame = "\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";
	ioPort(resFrame, outData);

	std::vector<unsigned char> response = GetBinaryOutData(outData);
	Logger("End Command");

	return response[2];
}

int PilotService::close_day(auth_answer& auth_answer, std::string& check)
{
	std::cout << "0) PilotServce::close_day SizeOf argument: " << sizeof(auth_answer) << std::endl;
	std::cout << "0) PilotServce::close_day SizeOf ammessage: " << sizeof(auth_answer.AMessage) << std::endl;	
	this->TestPinpad();
	std::cout << "1) PilotServce::close_day SizeOf argument: " << sizeof(auth_answer) << std::endl;
	std::cout << "1) PilotServce::close_day SizeOf ammessage: " << sizeof(auth_answer.AMessage) << std::endl;
	Logger("\nclose_day");
	//std::cout << "\nclose_day" << std::endl;
	std::cout << "2) PilotServce::close_day SizeOf argument: " << sizeof(auth_answer) << std::endl;
	std::cout << "2) PilotServce::close_day SizeOf ammessage: " << sizeof(auth_answer.AMessage) << std::endl;
	std::vector<unsigned char> lastResponsePax;
	std::cout << "1) PilotServce::close_day SizeOf argument: " << sizeof(auth_answer) << std::endl;
	std::cout << "1) PilotServce::close_day SizeOf ammessage: " << sizeof(auth_answer.AMessage) << std::endl;
	StartWork(auth_answer, lastResponsePax, str);	
	std::cout << "3) PilotServce::close_day SizeOf argument: " << sizeof(auth_answer) << std::endl;
	std::cout << "3) PilotServce::close_day SizeOf ammessage: " << sizeof(auth_answer.AMessage) << std::endl;

	rStr = str;
	OemToCharBuffA(str.c_str(), &rStr[0], str.size());
	check = rStr;
	auth_answer.Check = check.data();

	char errCode[] = { lastResponsePax[0], lastResponsePax[1] };
	int RCode = *((unsigned short*)errCode);

	std::cout << "4) PilotServce::close_day SizeOf argument: " << sizeof(auth_answer) << std::endl;
	std::cout << "4) PilotServce::close_day SizeOf ammessage: " << sizeof(auth_answer.AMessage) << std::endl;

	for (int i = 0; i < 3; i++)
		auth_answer.RCode[i] = std::to_string(RCode)[i];

	std::string result;

	if (RCode == 0)
		result = "Одобрено";
	else
		result = "Отклонено";

	for (int i = 0; i < 16; i++)
		auth_answer.AMessage[i] = result[i];

	Logger("End Command");
	return RCode;
}

int PilotService::card_authorize(const char* track2, auth_answer* auth_answer)
{
	runCardAuth["cardAuth"] = 1;
	if (runCardAuth["cardAuth"] == 0) return 2000;
	this->TestPinpad();
	if (runCardAuth["cardAuth"] == 0) return 2000;
	Logger("\ncard_authorize");
	//std::cout << "\ncard_authorize" << std::endl;
	std::string track;
	std::vector<unsigned char> lastResponsePax;

	if (track2 != nullptr)
		track = track2;

	if (runCardAuth["cardAuth"] == 0) return 2000;
	StartWork(*auth_answer, lastResponsePax, str, runCardAuth);

	rStr = str;
	OemToCharBuffA(str.c_str(), &rStr[0], str.size());
	auth_answer->Check = &rStr[0];

	for (int i = 0; i < 400; i++)
		std::cout << auth_answer->Check[i];

	std::cout << std::endl;

	if (runCardAuth["cardAuth"] == 0) return 2000;

	char errCode[] = { lastResponsePax[0], lastResponsePax[1] };
	int RCode = *((unsigned short*)errCode);
	Logger("End command");

	return RCode;
}

int PilotService::card_authorize15(const char* track2, auth_answer14* auth_answer, payment_info_item* payinfo, CONTEXT_PTR dataIn, CONTEXT_PTR dataOut)
{
	runCardAuth["cardAuth15"] = 1;
	if (runCardAuth["cardAuth15"] == 0) return 2000;
	this->TestPinpad();
	if (runCardAuth["cardAuth15"] == 0) return 2000;
	Logger("\ncard_authorize15");
	//std::cout << "\ncard_authorize15" << std::endl;
	std::string track;
	std::vector<unsigned char> lastResponsePax;

	if (track2 != nullptr)
		track = track2;

	if (runCardAuth["cardAuth15"] == 0) return 2000;
	StartWork(auth_answer->ans, lastResponsePax, str, runCardAuth);

	rStr = str;
	OemToCharBuffA(str.c_str(), &rStr[0], str.size());
	auth_answer->ans.Check = &rStr[0];

	for (int i = 0; i < 400; i++)
		std::cout << auth_answer->ans.Check[i];

	std::cout << std::endl;

	if (runCardAuth["cardAuth15"] == 0) return 2000;

	char errCode[] = { lastResponsePax[0], lastResponsePax[1] };
	int RCode = *((unsigned short*)errCode);
	ParsingResponseResCard(resRecCard, lastResponsePax);

	for (int i = 0; i < 7; i++)
		auth_answer->AuthCode[i] = resRecCard[CODE_AUTH][i];

	for (int i = 0; i < 20; i++)
		auth_answer->CardID[i] = resRecCard[NUMBER_CARD][i];

	auth_answer->ErrorCode = RCode;

	for (int i = 0; i < 4; i++)
		auth_answer->TransDate[i] = resRecCard[DATE_OPERATION][i];

	for (int i = 0; i < 4; i++)
		auth_answer->TransDate[i] = resRecCard[TIME_OPERATION][i];

	std::string transNumber(resRecCard[NUMBER_OPERATION_DAY].begin(), resRecCard[NUMBER_OPERATION_DAY].end());
	auth_answer->TransNumber = std::stoi(transNumber);

	auth_answer->SberOwnCard = resRecCard[BANK_AFFILIATION][0];

	for (int i = 0; i < 20; i++)
		auth_answer->Hash[i] = resRecCard[HASH_CARD][i];

	for (int i = 0; i < 13; i++)
		auth_answer->RRN[i] = resRecCard[RRN][i];

	for (int i = 0; i < 32; i++)
		auth_answer->CardName[i] = resRecCard[NAME_CARD][i];

	for (int i = 0; i < 2; i++)
		auth_answer->ans.RCode[i] = resRecCard[ERROR_CODE][i];

	for (int i = 0; i < 16; i++)
		auth_answer->ans.AMessage[i] = resRecCard[TEXT_MESSAGE_ERROR][i];

	//std::vector<std::unordered_map<std::string, std::vector<unsigned char>>> list = DTagExtraData[dataOut];
	//std::unordered_map<std::string, std::vector<unsigned char>> uMap =
	//{
	//	{ "pan", resRecCard[NUMBER_CARD] },
	//	{ "ExpiryDate", resRecCard[CARD_DATE] },
	//	{ "hash", resRecCard[HASH_CARD] },
	//	{ "ownCard", resRecCard[BANK_AFFILIATION] },
	//	{ "EncryptedData", resRecCard[ECRYPTED_DATA] }
	//};
	//list.push_back(uMap);
	//DTagExtraData[dataOut] = list;
	Logger("End command");

	return RCode;
}

CONTEXT_PTR PilotService::ctxAlloc()
{
	Logger("\nctxAlloc");
	CONTEXT_PTR ctx = 0;
	CONTEXT_PTR addr = (CONTEXT_PTR)&ctx;

	Logger("Создан контекст по адресу: " + addr);
	//std::cout << "Создан контекст по адресу: " << addr << std::endl;

	auth_answer14 argument;
	memset(&argument, 0, sizeof(argument));

	std::vector<std::unordered_map<std::string, std::vector<unsigned char>>> listTagExtraData;
	dContext[addr] = argument;
	DTagExtraData[addr] = listTagExtraData;
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

void PilotService::ctxFree(CONTEXT_PTR ctx)
{
	Logger("\nctxFree");
	dContext.erase(ctx);
	DTagExtraData.erase(ctx);
	Logger("End command");
}

int PilotService::ctxGetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* pVal, int* pOutSz, int MAXSIZE)
{
	Logger("\nctxGetBinary");
	std::vector<std::unordered_map<std::string, std::vector<unsigned char>>> listTagExtraData;
	std::unordered_map<std::string, std::vector<unsigned char>> uMap;

	switch (name)
	{
	case PAR_ENCRYPTED_DATA:
	{
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

int PilotService::ctxGetInt(CONTEXT_PTR ctx, EParameterName name, int* pVal)
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

int PilotService::ctxGetString(CONTEXT_PTR ctx, EParameterName name, char* str, int sz)
{
	Logger("\nctxGetString");
	std::vector<std::unordered_map<std::string, std::vector<unsigned char>>> listTagExtraData;
	std::unordered_map<std::string, std::vector<unsigned char>> uMap;

	switch (name)
	{
	case PAR_PAN:
	{
		listTagExtraData = DTagExtraData[ctx];
		uMap = listTagExtraData[0];
		std::string s(uMap["pan"].begin(), uMap["pan"].end());
		str = s.data();
	}
		break;
	case PAR_HASH:
	{
		listTagExtraData = DTagExtraData[ctx];
		uMap = listTagExtraData[0];
		std::string s(uMap["hash"].begin(), uMap["hash"].end());
		str = s.data();
	}
		break;
	case PAR_EXPIRY_DATE:
	{
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

int PilotService::ctxSetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* val, int sz)
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
	}
		break;
	default:
		break;
	}
	Logger("End command");
	return 0;
}

int PilotService::ctxSetInt(CONTEXT_PTR ctx, EParameterName name, int val)
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
	}
		break;
	default:
		break;
	}

	dContext[ctx] = argument;
	Logger("End command");
	return 0;
}

int PilotService::ctxSetString(CONTEXT_PTR ctx, EParameterName name, const char* str)
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
	}
		break;
	default:
		break;
	}
	Logger("End command");
	return 0;
}

int PilotService::ReadCardContext(CONTEXT_PTR dataOut)
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
	Logger("End command");
	return RCode;
}

int PilotService::CommitTrx(DWORD dwAmount, char* pAuthCode)
{
	this->TestPinpad();
	Logger("\nCommitTrx");
	//std::cout << "\nCommitTrx" << std::endl;

	std::vector<unsigned char> lastResponsePax;
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
	this->TestPinpad();
	Logger("\nRollbackTrx");
	//std::cout << "\nRollbackTrx" << std::endl;

	std::vector<unsigned char> lastResponsePax;
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
	//std::cout << "\nSuspendTrx" << std::endl;

	std::vector<unsigned char> lastResponsePax;
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
	//std::cout << "\nAbortTransaction" << std::endl;
	std::vector<unsigned char> frame = GetFrameGetReady(0x0000);

	GetFrameWithCrc16(frame);

	std::string resFrame = "\u0002!" + base64_encode(&frame[0], frame.size()) + "\u0003";
	
	HANDLE hSerial;
	open_port(&hSerial);
	Logger("-> " + resFrame);
	write_port(&hSerial, &resFrame[0], resFrame.length());
	close_port(&hSerial);

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
