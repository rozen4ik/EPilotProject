#pragma once

#include "pilot_nt.h"
#include "com_port.h"
#include "cmd_msb.h"
#include "PacketProcessing.h"
#include "base64.h"
#include <iostream>
#include <unordered_map>

class PilotService
{
public:
	std::unordered_map<CONTEXT_PTR, auth_answer14> dContext;
	std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>> DTagExtraData;
	std::unordered_map<ResponseRCardContext, std::vector<unsigned char>> resRecCard;
	std::unordered_map<std::string, int>runCardAuth;
	std::string str;
	std::string rStr;


	int TestPinpad();
	int close_day(auth_answer& auth_answer, std::string& check);
	int card_authorize(const char* track2, auth_answer* auth_answer);
	int card_authorize15(const char* track2, auth_answer14* auth_answer, payment_info_item* payinfo, CONTEXT_PTR dataIn, CONTEXT_PTR dataOut);
	CONTEXT_PTR ctxAlloc();
	void ctxClear(CONTEXT_PTR ctx);
	void ctxFree(CONTEXT_PTR ctx);
	int ctxGetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* pVal, int* pOutSz, int MAXSIZE);
	int ctxGetInt(CONTEXT_PTR ctx, EParameterName name, int* pVal);
	int ctxGetString(CONTEXT_PTR ctx, EParameterName name, char* str, int sz);
	int ctxSetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* val, int sz);
	int ctxSetInt(CONTEXT_PTR ctx, EParameterName name, int val);
	int ctxSetString(CONTEXT_PTR ctx, EParameterName name, const char* str);
	int ReadCardContext(CONTEXT_PTR dataOut);
	int CommitTrx(DWORD dwAmount, char* pAuthCode);
	int RollbackTrx(DWORD dwAmount, char* pAuthCode);
	int SuspendTrx(DWORD dwAmount, char* pAuthCode);
	int AbortTransaction();
	void Done();
};

