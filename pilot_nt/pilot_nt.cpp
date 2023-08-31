#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include "pilot_nt.h"
#include "com_port.h"
#include "cmd_msb.h"
#include "PacketProcessing.h"
#include "base64.h"
#include <iostream>
#include "PilotService.h"

PilotService pService;

int __cdecl _TestPinpad()
{	
	return pService.TestPinpad();
}

int __cdecl _close_day(auth_answer* auth_answer)
{
	std::cout << "_close_day SizeOf argument: " << sizeof(*auth_answer) << std::endl;
	std::cout << "_close_day SizeOf ammessage: " << sizeof(auth_answer->AMessage) << std::endl;
	return pService.close_day(*auth_answer, check);
}

int __cdecl _card_authorize(char* track2, auth_answer* auth_answer)
{
	return pService.card_authorize(track2, auth_answer);
}

int __cdecl _card_authorize15(const char* track2, auth_answer14* auth_answer, payment_info_item* payinfo, CONTEXT_PTR dataIn, CONTEXT_PTR dataOut)
{
	return pService.card_authorize15(track2, *auth_answer, *payinfo, dataIn, dataOut, check);
}

CONTEXT_PTR __cdecl _ctxAlloc()
{
	return pService.ctxAlloc();
}

void __cdecl _ctxClear(CONTEXT_PTR ctx)
{
	pService.ctxClear(ctx);
}

void __cdecl _ctxFree(CONTEXT_PTR ctx)
{
	pService.ctxFree(ctx);
}

int __cdecl _ctxGetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* pVal, int* pOutSz, int MAXSIZE)
{
	return pService.ctxGetBinary(ctx, name, pVal, pOutSz, MAXSIZE);
}

int __cdecl _ctxGetInt(CONTEXT_PTR ctx, EParameterName name, int* pVal)
{
	return pService.ctxGetInt(ctx, name, pVal);
}

int __cdecl _ctxGetString(CONTEXT_PTR ctx, EParameterName name, char* str, int sz)
{
	return pService.ctxGetString(ctx, name, str, sz);
}

int __cdecl _ctxSetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* val, int sz)
{
	return pService.ctxSetBinary(ctx, name, val, sz);
}

int __cdecl _ctxSetInt(CONTEXT_PTR ctx, EParameterName name, int val)
{
	return pService.ctxSetInt(ctx, name, val);
}

int __cdecl _ctxSetString(CONTEXT_PTR ctx, EParameterName name, const char* str)
{
	return pService.ctxSetString(ctx, name, str);
}

int __cdecl _ReadCardContext(CONTEXT_PTR dataOut)
{
	return pService.ReadCardContext(dataOut);
}

int __cdecl _CommitTrx(DWORD dwAmount, char* pAuthCode)
{
	return pService.CommitTrx(dwAmount, pAuthCode);
}

int __cdecl _RollbackTrx(DWORD dwAmount, char* pAuthCode)
{
	return pService.RollbackTrx(dwAmount, pAuthCode);
}

int __cdecl _SuspendTrx(DWORD dwAmount, char* pAuthCode)
{
	return pService.SuspendTrx(dwAmount, pAuthCode);
}

int __cdecl _AbortTransaction()
{
	return pService.AbortTransaction();
}

void __cdecl _Done()
{	
	pService.Done();
}