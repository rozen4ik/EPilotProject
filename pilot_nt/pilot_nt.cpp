#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include "pilot_nt.h"
#include "com_port.h"
#include "cmd_msb.h"
#include "PacketProcessing.h"
#include "base64.h"
#include <iostream>

int TestPinpad()
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

int close_day(auth_answer* auth_answer)
{
	TestPinpad();
	std::cout << "\nclose_day" << std::endl;
	
	auth_answer->TType = OP_PIL_OT_TOTALS;
	std::vector<unsigned char> lastResponsePax;

	StartWork(*auth_answer, lastResponsePax);

	char errCode[] = { lastResponsePax[0], lastResponsePax[1]};
	int RCode = *((unsigned short*)errCode);

	return RCode;
}

int card_authorize15(const char* track2, auth_answer14* auth_answer, payment_info_item*, CONTEXT_PTR dataInt, CONTEXT_PTR dataOut)
{
	TestPinpad();
	std::cout << "\ncard_authorize15" << std::endl;
	std::string track;
	std::vector<unsigned char> lastResponsePax;

	if (track2 != nullptr)
		track = track2;

	StartWork(auth_answer->ans, lastResponsePax);

	return 0;
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