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
	return pService.TestPinpad();
}

int close_day(auth_answer* auth_answer)
{
	return pService.close_day(auth_answer);
}

int card_authorize15(const char* track2, auth_answer14* auth_answer, payment_info_item* payinfo, CONTEXT_PTR dataIn, CONTEXT_PTR dataOut)
{
	return pService.card_authorize15(track2, auth_answer, payinfo, dataIn, dataOut);
}

CONTEXT_PTR ctxAlloc()
{
	return pService.ctxAlloc();
}

void ctxClear(CONTEXT_PTR ctx)
{
	pService.ctxClear(ctx);
}

void ctxFree(CONTEXT_PTR ctx)
{
	pService.ctxFree(ctx);
}

int ctxGetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* pVal, int* pOutSz, int MAXSIZE)
{
	return pService.ctxGetBinary(ctx, name, pVal, pOutSz, MAXSIZE);
}

int ctxGetInt(CONTEXT_PTR ctx, EParameterName name, int* pVal)
{
	return pService.ctxGetInt(ctx, name, pVal);
}

int ctxGetString(CONTEXT_PTR ctx, EParameterName name, char* str, int sz)
{
	return pService.ctxGetString(ctx, name, str, sz);
}

int ctxSetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* val, int sz)
{
	return pService.ctxSetBinary(ctx, name, val, sz);
}

int ctxSetInt(CONTEXT_PTR ctx, EParameterName name, int val)
{
	return pService.ctxSetInt(ctx, name, val);
}

int ctxSetString(CONTEXT_PTR ctx, EParameterName name, const char* str)
{
	return pService.ctxSetString(ctx, name, str);
}

int ReadCardContext(CONTEXT_PTR dataOut)
{
	return pService.ReadCardContext(dataOut);
}

int CommitTrx(DWORD dwAmount, char* pAuthCode)
{
	return pService.CommitTrx(dwAmount, pAuthCode);
}

int RollbackTrx(DWORD dwAmount, char* pAuthCode)
{
	return pService.RollbackTrx(dwAmount, pAuthCode);
}

int SuspendTrx(DWORD dwAmount, char* pAuthCode)
{
	return pService.SuspendTrx(dwAmount, pAuthCode);
}

int AbortTransaction()
{
	return pService.AbortTransaction();
}

void Done()
{	
	pService.Done();
}