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

    StartWork(*auth_answer, lastResponsePax);

    char errCode[] = { lastResponsePax[0], lastResponsePax[1] };
    int RCode = *((unsigned short*)errCode);
    return RCode;
}

int PilotService::card_authorize15(const char* track2, auth_answer14* auth_answer, payment_info_item* payinfo, CONTEXT_PTR dataIn, CONTEXT_PTR dataOut)
{
    this->TestPinpad();
    std::cout << "\ncard_authorize15" << std::endl;
    std::string track;
    std::vector<unsigned char> lastResponsePax;

    if (track2 != nullptr)
        track = track2;

    StartWork(auth_answer->ans, lastResponsePax);

    char errCode[] = { lastResponsePax[0], lastResponsePax[1] };
    int RCode = *((unsigned short*)errCode);

    return RCode;
}

CONTEXT_PTR PilotService::ctxAlloc()
{
    return CONTEXT_PTR();
}

void PilotService::ctxClear(CONTEXT_PTR ctx)
{
}

void PilotService::ctxFree(CONTEXT_PTR ctx)
{
}

int PilotService::ctxGetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* pVal, int* pOutSz, int MAXSIZE)
{
    return 0;
}

int PilotService::ctxGetInt(CONTEXT_PTR ctx, EParameterName name, int* pVal)
{
    return 0;
}

int PilotService::ctxGetString(CONTEXT_PTR ctx, EParameterName name, char* str, int sz)
{
    return 0;
}

int PilotService::ctxSetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* val, int sz)
{
    return 0;
}

int PilotService::ctxSetInt(CONTEXT_PTR ctx, EParameterName name, int val)
{
    return 0;
}

int PilotService::ctxSetString(CONTEXT_PTR ctx, EParameterName name, const char* str)
{
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

    StartWork(argument, lastResponsePax);

    char errCode[] = { lastResponsePax[0], lastResponsePax[1] };
    int RCode = *((unsigned short*)errCode);

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

    StartWork(argument, lastResponsePax);

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

    StartWork(argument, lastResponsePax);

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

    StartWork(argument, lastResponsePax);

    char errCode[] = { lastResponsePax[0], lastResponsePax[1] };
    int RCode = *((unsigned short*)errCode);

    return RCode;
}

int PilotService::AbortTransaction()
{
    this->TestPinpad();
    std::cout << "\nAbortTransaction" << std::endl;

    return 2000;
}

void PilotService::Done()
{
}
