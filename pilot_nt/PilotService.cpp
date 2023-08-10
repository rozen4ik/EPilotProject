#include "pch.h"
#include "PilotService.h"

int PilotService::TestPinpad()
{
	std::cout << "\nTestPinpad" << std::endl;

	std::string outData = "";
	std::vector<unsigned char> frame = GetFrameGetReady(0x0000);

	GetFrameWithCrc16(frame);

	std::string resFrame = "\u0002#" + base64_encode(&frame[0], frame.size()) + "\u0003";
	ioPort(resFrame, outData);

	std::vector<unsigned char> response = GetBinaryOutData(outData);

	return response[2];
}

int PilotService::close_day(auth_answer* auth_answer)
{
	this->TestPinpad();
	std::cout << "\nclose_day" << std::endl;

	auth_answer->TType = OP_PIL_OT_TOTALS;
	std::vector<unsigned char> lastResponsePax;

	StartWork(*auth_answer, lastResponsePax, runCardAuth);

	char errCode[] = { lastResponsePax[0], lastResponsePax[1] };
	int RCode = *((unsigned short*)errCode);
	return RCode;
}

int PilotService::card_authorize15(const char* track2, auth_answer14* auth_answer, payment_info_item* payinfo, CONTEXT_PTR dataIn, CONTEXT_PTR dataOut)
{
	runCardAuth["cardAuth15"] = 1;
	if (runCardAuth["cardAuth15"] == 0) return 2000;
	this->TestPinpad();
	if (runCardAuth["cardAuth15"] == 0) return 2000;
	std::cout << "\ncard_authorize15" << std::endl;
	std::string track;
	std::vector<unsigned char> lastResponsePax;

	if (track2 != nullptr)
		track = track2;

	if (runCardAuth["cardAuth15"] == 0) return 2000;
	StartWork(auth_answer->ans, lastResponsePax, runCardAuth);
	if (runCardAuth["cardAuth15"] == 0) return 2000;

	char errCode[] = { lastResponsePax[0], lastResponsePax[1] };
	int RCode = *((unsigned short*)errCode);

	return RCode;
}

CONTEXT_PTR PilotService::ctxAlloc()
{
	CONTEXT_PTR ctx = 0;
	CONTEXT_PTR addr = (CONTEXT_PTR)&ctx;

	std::cout << "Создан контекст по адресу: " << addr << std::endl;

	auth_answer14 argument;
	memset(&argument, 0, sizeof(argument));

	std::vector<std::unordered_map<std::string, std::vector<unsigned char>>> listTagExtraData;
	dContext[addr] = argument;
	DTagExtraData[addr] = listTagExtraData;

	return addr;
}

void PilotService::ctxClear(CONTEXT_PTR ctx)
{
	auth_answer14 argument;
	memset(&argument, 0, sizeof(argument));

	dContext[ctx] = argument;
}

void PilotService::ctxFree(CONTEXT_PTR ctx)
{
	dContext.erase(ctx);
	DTagExtraData.erase(ctx);
}

int PilotService::ctxGetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* pVal, int* pOutSz, int MAXSIZE)
{
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

	return 0;
}

int PilotService::ctxGetInt(CONTEXT_PTR ctx, EParameterName name, int* pVal)
{
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

	return 0;
}

int PilotService::ctxGetString(CONTEXT_PTR ctx, EParameterName name, char* str, int sz)
{
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

	return 0;
}

int PilotService::ctxSetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* val, int sz)
{
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

	return 0;
}

int PilotService::ctxSetInt(CONTEXT_PTR ctx, EParameterName name, int val)
{
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

	return 0;
}

int PilotService::ctxSetString(CONTEXT_PTR ctx, EParameterName name, const char* str)
{
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

	return 0;
}

int PilotService::ReadCardContext(CONTEXT_PTR dataOut)
{
	this->TestPinpad();
	std::cout << "\nReadCardContext" << std::endl;

	std::vector<unsigned char> lastResponsePax;
	auth_answer argument;
	memset(&argument, 0, sizeof(argument));

	argument.TType = OP_READ_CARD;
	argument.Amount = 0;
	argument.CType = 0;

	StartWork(argument, lastResponsePax, runCardAuth);

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

	return RCode;
}

int PilotService::CommitTrx(DWORD dwAmount, char* pAuthCode)
{
	this->TestPinpad();
	std::cout << "\nCommitTrx" << std::endl;

	std::vector<unsigned char> lastResponsePax;
	auth_answer argument;
	memset(&argument, 0, sizeof(argument));

	argument.TType = OP_COMMIT;
	argument.Amount = dwAmount;

	StartWork(argument, lastResponsePax, runCardAuth);

	char errCode[] = { lastResponsePax[0], lastResponsePax[1] };
	int RCode = *((unsigned short*)errCode);

	return RCode;
}

int PilotService::RollbackTrx(DWORD dwAmount, char* pAuthCode)
{
	this->TestPinpad();
	std::cout << "\nRollbackTrx" << std::endl;

	std::vector<unsigned char> lastResponsePax;
	auth_answer argument;
	memset(&argument, 0, sizeof(argument));

	argument.TType = OP_ROLLBACK;
	argument.Amount = dwAmount;

	StartWork(argument, lastResponsePax, runCardAuth);

	char errCode[] = { lastResponsePax[0], lastResponsePax[1] };
	int RCode = *((unsigned short*)errCode);

	return RCode;
}

int PilotService::SuspendTrx(DWORD dwAmount, char* pAuthCode)
{
	this->TestPinpad();
	std::cout << "\nSuspendTrx" << std::endl;

	std::vector<unsigned char> lastResponsePax;
	auth_answer argument;
	memset(&argument, 0, sizeof(argument));

	argument.TType = OP_SUSPEND;
	argument.Amount = dwAmount;

	StartWork(argument, lastResponsePax, runCardAuth);

	char errCode[] = { lastResponsePax[0], lastResponsePax[1] };
	int RCode = *((unsigned short*)errCode);

	return RCode;
}

int PilotService::AbortTransaction()
{
	std::cout << "\nAbortTransaction" << std::endl;
	std::vector<unsigned char> frame = GetFrameGetReady(0x0000);

	GetFrameWithCrc16(frame);

	std::string resFrame = "\u0002!" + base64_encode(&frame[0], frame.size()) + "\u0003";
	
	HANDLE hSerial;
	open_port(&hSerial);
	write_port(&hSerial, &resFrame[0], resFrame.length());
	close_port(&hSerial);

	for (const auto& c : runCardAuth)
	{
		runCardAuth[c.first] = 0;
	}

	return 2000;
}

void PilotService::Done()
{
}
