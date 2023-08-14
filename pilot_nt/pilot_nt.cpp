#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include "pilot_nt.h"
#include "com_port.h"
#include "cmd_msb.h"
#include "PacketProcessing.h"
#include "base64.h"
#include <iostream>
#include "PilotService.h"

PilotService pService;

int _TestPinpad()
{	
	return pService.TestPinpad();
}

int _close_day(auth_answer* auth_answer)
{
	return pService.close_day(auth_answer);
}

int _card_authorize(char* track2, auth_answer* auth_answer)
{
	return pService.card_authorize(track2, auth_answer);
}

int _card_authorize15(const char* track2, auth_answer14* auth_answer, payment_info_item* payinfo, CONTEXT_PTR dataIn, CONTEXT_PTR dataOut)
{
	return pService.card_authorize15(track2, auth_answer, payinfo, dataIn, dataOut);
}

CONTEXT_PTR _ctxAlloc()
{
	return pService.ctxAlloc();
}

void _ctxClear(CONTEXT_PTR ctx)
{
	pService.ctxClear(ctx);
}

void _ctxFree(CONTEXT_PTR ctx)
{
	pService.ctxFree(ctx);
}

int _ctxGetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* pVal, int* pOutSz, int MAXSIZE)
{
	return pService.ctxGetBinary(ctx, name, pVal, pOutSz, MAXSIZE);
}

int _ctxGetInt(CONTEXT_PTR ctx, EParameterName name, int* pVal)
{
	return pService.ctxGetInt(ctx, name, pVal);
}

int _ctxGetString(CONTEXT_PTR ctx, EParameterName name, char* str, int sz)
{
	return pService.ctxGetString(ctx, name, str, sz);
}

int _ctxSetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* val, int sz)
{
	return pService.ctxSetBinary(ctx, name, val, sz);
}

int _ctxSetInt(CONTEXT_PTR ctx, EParameterName name, int val)
{
	return pService.ctxSetInt(ctx, name, val);
}

int _ctxSetString(CONTEXT_PTR ctx, EParameterName name, const char* str)
{
	return pService.ctxSetString(ctx, name, str);
}

int _ReadCardContext(CONTEXT_PTR dataOut)
{
	return pService.ReadCardContext(dataOut);
}

int _CommitTrx(DWORD dwAmount, char* pAuthCode)
{
	return pService.CommitTrx(dwAmount, pAuthCode);
}

int _RollbackTrx(DWORD dwAmount, char* pAuthCode)
{
	return pService.RollbackTrx(dwAmount, pAuthCode);
}

int _SuspendTrx(DWORD dwAmount, char* pAuthCode)
{
	return pService.SuspendTrx(dwAmount, pAuthCode);
}

int _AbortTransaction()
{
	return pService.AbortTransaction();
}

void _Done()
{	
	pService.Done();
}