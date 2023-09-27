#pragma once

#include "pilot_nt.h"
#include "PacketProcessing.h"
#include <iostream>
#include <unordered_map>
#include "ComPort.h"
#include "SberCmd.h"

// ��� ����������
#define MMCMD_OPEN 0x01 // � ������� 
#define MMCMD_READ 0x02 // � ������ ������ 
#define MMCMD_WRITE 0x03 // � ������ ������ 
#define MMCMD_CLOSE 0x04 // � �������

// ��� ����������
#define MDC_NULL 0 // � �� ������������ 
#define MDC_PRINTER 3// � ������� 
#define MDC_RTCLOCK 5// � �������� ����� � ���� 
#define MDC_LAN 25 // � ������� ��������� 
#define MDC_PINGINFO 39 // � ����� ����������� � ��������� ��������� 
#define MDC_NOTIFY_REBOOT 41 // � ����������� � ������������ ��� ���������� ��(REBOOT) 
#define MDC_TIMER 45 // � �������� ��������� ���������

class PilotService
{
public:
	int TestPinpad(); ///< �������� ���������� �������
	int close_day(auth_answer* auth_answer, std::string& check); ///< �������� ���. 
	int card_authorize(const char* track2, auth_answer* auth_answer); ///< ���������� �������� �� ������
	int card_authorize15(const char* track2, auth_answer14& auth_answer, payment_info_item& payinfo, CONTEXT_PTR dataIn, CONTEXT_PTR dataOut, std::string& check); ///< ���������� �������� �� ������
	CONTEXT_PTR ctxAlloc(std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context); ///< ������� �������� ��������. ������� ������� ������ �������� ��������. 
	void ctxClear(CONTEXT_PTR ctx); ///< ��������� ��������. ������� ������� ��� ��������� �� ���������.
	void ctxFree(CONTEXT_PTR ctx, std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context); ///< ������� �������� �������� 
	int ctxGetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* pVal, int* pOutSz, int MAXSIZE, std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context); ///< ������� �� ��������� �������� ���������� � ���� ������������������ ����.
	int ctxGetInt(CONTEXT_PTR ctx, EParameterName name, int* pVal, std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context); ///< ������� �� ��������� �������� ���������� � ���� ������ �����. 
	int ctxGetString(CONTEXT_PTR ctx, EParameterName name, char* str, int sz, std::string& _hash, std::string& _par_pan, std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context); ///< ������� �� ��������� �������� ���������� � ���� ������. 
	int ctxSetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* val, int sz, std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context); ///< �������� � �������� �������� ��������� � ���� ������������������ ����. 
	int ctxSetInt(CONTEXT_PTR ctx, EParameterName name, int val, std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context); ///< �������� � �������� �������� �������������� ���������. 
	int ctxSetString(CONTEXT_PTR ctx, EParameterName name, const char* str, std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context); ///< �������� � �������� �������� ���������� ���������. 
	int ReadCardContext(CONTEXT_PTR dataOut, std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context); ///< ������ ����� 
	int CommitTrx(DWORD dwAmount, char* pAuthCode); ///< ������� ���������� ��������� �������� ���������� � ����������� ���������. ����� ����� ���������� ����� �������� � ����� � ��������������� ��� ��������. ��������� �� ����� � ������������ ��������� ����� ��� ������.
	int RollbackTrx(DWORD dwAmount, char* pAuthCode); ///< ������� �������� ����������� ������ ��������� �������� �������� 
	int SuspendTrx(DWORD dwAmount, char* pAuthCode); ///< ������� ��������� ��������� �������� ���������� � ������������ ���������. ���� ���������� ��������� � ���� ���������, �� ��� ��������� ������ ����� � ������ ��� ����� ��������.. 
	int AbortTransaction(); ///< ������� ��������� ������ ������� card_authorizeX() 
	void Done(); ///< ��������������� ���������� pilot_nt.dll. 

private:
	std::unordered_map<CONTEXT_PTR, auth_answer14> dContext;
	std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>> DTagExtraData;
	std::unordered_map<ResponseRCardContext, std::vector<unsigned char>> resRecCard;
	std::unordered_map<std::string, int>runCardAuth;
	std::string str;
	std::string rStr;

	template<typename T> int StartWork(T& auth_answe, SberCmd& sber_cmd);
	int BodyWorkPilotTrx(SberCmd& sber_cmd);
	std::string GetIp(const std::vector<unsigned char>& response);
	int GetPort(const std::vector<unsigned char>& response);
};

template<typename T>
inline int PilotService::StartWork(T& auth_answe, SberCmd& sber_cmd)
{
	std::string outData = "";
	sber_cmd.RunTrx(auth_answe);
	if (runCardAuth.find("cardAuth15") != runCardAuth.end() && runCardAuth["cardAuth15"] == 0) return 2000;
	sber_cmd.SetResFrame("\u0004\u0002#");

	ComPort com_port;
	std::string res_frame = sber_cmd.GetResFrame();
	com_port.IOPort(res_frame, outData);
	if (runCardAuth.find("cardAuth15") != runCardAuth.end() && runCardAuth["cardAuth15"] == 0) return 2000;

	sber_cmd.SetBinaryOutData(outData);
	if (runCardAuth.find("cardAuth15") != runCardAuth.end() && runCardAuth["cardAuth15"] == 0) return 2000;
	int result = BodyWorkPilotTrx(sber_cmd);
	if (runCardAuth.find("cardAuth15") != runCardAuth.end() && runCardAuth["cardAuth15"] == 0) return 2000;

	outData = "";
	return result;
}
