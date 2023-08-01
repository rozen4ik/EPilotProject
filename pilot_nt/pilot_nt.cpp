#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include "pilot_nt.h"
#include "com_port.h"
#include "cmd_msb.h"
#include "PacketProcessing.h"
#include "base64.h"
#include <iostream>
#include "PilotService.h"

PilotService pService;

int TestPinpad()
{	
	PilotService* ps = &pService;
	return ps->TestPinpad();
}

int close_day(auth_answer* auth_answer)
{
	PilotService* ps = &pService;
	return ps->close_day(auth_answer);
}

int card_authorize15(const char* track2, auth_answer14* auth_answer, payment_info_item* payinfo, CONTEXT_PTR dataIn, CONTEXT_PTR dataOut)
{
	PilotService* ps = &pService;
	return ps->card_authorize15(track2, auth_answer, payinfo, dataIn, dataOut);
}

CONTEXT_PTR ctxAlloc()
{
	return 0;
}

void ctxClear(CONTEXT_PTR ctx)
{
	//
}

void ctxFree(CONTEXT_PTR ctx)
{
	//
}

int ctxGetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* pVal, int* pOutSz, int MAXSIZE)
{
	return 0;
}

int ctxGetInt(CONTEXT_PTR ctx, EParameterName name, int* pVal)
{
	return 0;
}

int ctxGetString(CONTEXT_PTR ctx, EParameterName name, char* str, int sz)
{
	return 0;
}

int ctxSetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* val, int sz)
{
	return 0;
}

int ctxSetInt(CONTEXT_PTR ctx, EParameterName name, int val)
{
	return 0;
}

int ctxSetString(CONTEXT_PTR ctx, EParameterName name, const char* str)
{
	return 0;
}

int ReadCardContext(CONTEXT_PTR dataOut)
{
	PilotService* ps = &pService;
	return ps->ReadCardContext(dataOut);
}

int CommitTrx(DWORD dwAmount, char* pAuthCode)
{
	PilotService* ps = &pService;
	return ps->CommitTrx(dwAmount, pAuthCode);
}

int RollbackTrx(DWORD dwAmount, char* pAuthCode)
{
	PilotService* ps = &pService;
	return ps->RollbackTrx(dwAmount, pAuthCode);
}

int SuspendTrx(DWORD dwAmount, char* pAuthCode)
{
	PilotService* ps = &pService;
	return ps->SuspendTrx(dwAmount, pAuthCode);
}

int AbortTransaction()
{
	PilotService* ps = &pService;
	return ps->AbortTransaction();
}

void Done()
{	
	//
}