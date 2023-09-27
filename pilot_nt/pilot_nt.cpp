#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include "pilot_nt.h"
#include "PacketProcessing.h"
#include <iostream>
#include "PilotService.h"

//PilotService pService;

int __cdecl _TestPinpad()
{	
	PilotService pService;
	return pService.TestPinpad();
}

int __cdecl _close_day(auth_answer* auth_answer)
{
	PilotService pService;
	return pService.close_day(auth_answer, check);
}

int __cdecl _card_authorize(char* track2, auth_answer* auth_answer)
{
	PilotService pService;
	return pService.card_authorize(track2, auth_answer);
}

int __cdecl _card_authorize15(const char* track2, auth_answer14* auth_answer, payment_info_item* payinfo, CONTEXT_PTR dataIn, CONTEXT_PTR dataOut)
{
	PilotService pService;
	return pService.card_authorize15(track2, *auth_answer, *payinfo, dataIn, dataOut, check);
}

CONTEXT_PTR __cdecl _ctxAlloc()
{
	PilotService pService;
	return pService.ctxAlloc(map_context);
}

void __cdecl _ctxClear(CONTEXT_PTR ctx)
{
	PilotService pService;
	pService.ctxClear(ctx);
}

void __cdecl _ctxFree(CONTEXT_PTR ctx)
{
	PilotService pService;
	pService.ctxFree(ctx, map_context);
}

int __cdecl _ctxGetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* pVal, int* pOutSz, int MAXSIZE)
{
	PilotService pService;
	return pService.ctxGetBinary(ctx, name, pVal, pOutSz, MAXSIZE, map_context);
}

int __cdecl _ctxGetInt(CONTEXT_PTR ctx, EParameterName name, int* pVal)
{
	PilotService pService;
	return pService.ctxGetInt(ctx, name, pVal, map_context);
}

int __cdecl _ctxGetString(CONTEXT_PTR ctx, EParameterName name, char* str, int sz)
{
	PilotService pService;
	return pService.ctxGetString(ctx, name, str, sz, hash, par_pan, map_context);
}

int __cdecl _ctxSetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* val, int sz)
{
	PilotService pService;
	return pService.ctxSetBinary(ctx, name, val, sz, map_context);
}

int __cdecl _ctxSetInt(CONTEXT_PTR ctx, EParameterName name, int val)
{
	PilotService pService;
	return pService.ctxSetInt(ctx, name, val, map_context);
}

int __cdecl _ctxSetString(CONTEXT_PTR ctx, EParameterName name, const char* str)
{
	PilotService pService;
	return pService.ctxSetString(ctx, name, str, map_context);
}

int __cdecl _ReadCardContext(CONTEXT_PTR dataOut)
{
	PilotService pService;
	return pService.ReadCardContext(dataOut, map_context);
}

int __cdecl _CommitTrx(DWORD dwAmount, char* pAuthCode)
{
	PilotService pService;
	return pService.CommitTrx(dwAmount, pAuthCode);
}

int __cdecl _RollbackTrx(DWORD dwAmount, char* pAuthCode)
{
	PilotService pService;
	return pService.RollbackTrx(dwAmount, pAuthCode);
}

int __cdecl _SuspendTrx(DWORD dwAmount, char* pAuthCode)
{
	PilotService pService;
	return pService.SuspendTrx(dwAmount, pAuthCode);
}

int __cdecl _AbortTransaction()
{
	PilotService pService;
	return pService.AbortTransaction();
}

void __cdecl _Done()
{	
	PilotService pService;
	pService.Done();
}