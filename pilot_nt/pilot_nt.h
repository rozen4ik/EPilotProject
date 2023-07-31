#pragma once

#include "paramsln.h"
#include <windows.h>

#ifdef PILOT_NT_EXPORTS
#define PILOT_NT_API __declspec(dllexport)
#else
#define PILOT_NT_API __declspec(dllimport)
#endif


/** ������������� ��������� �������� */
typedef long CONTEXT_PTR;


/** �������������� ����������, ������� ����� �������� ��� �������� ����� �������� �������� */
typedef enum {
    PAR_LLT_ID = 1,    ///< [out] ����� ��������� ���������� � ������� ������ �����, �����. �������� ������������ ��� ������ ������� ::card_authorize15 � ::ReadCardContext
    PAR_PAN = 2,    ///< [out] ������������� ����� �����, ������. �������� ������������ ��� ������ ������� ::ReadCardContext
    PAR_HASH = 3,    ///< [out] ���. ������. �������� ������������ �������� ::ReadCardContext
    PAR_CARD_TYPE = 4,    ///< [in/out] ��� �����. ������������ ::CardTypes. �������� ������������ ��������� ::ReadCardContext � ::card_authorize15 � �������� �������� ��� ���������� ������� VAS � ::ReadCardContext � �������� ��������� ���������. 
    PAR_OWN_CARD = 5,    ///< [out] ������� "����� �������� ����������". �����. �������� ������������ �������� ::ReadCardContext
    PAR_VAS_LENGTH = 6,    ///< [out] ����� ������ � xml ������� VAS ��� ����� ������������ ����. �����. �������� ������������ �������� ::ReadCardContext � ::card_authorize15
    PAR_VAS_STRING = 7,    ///< [out] ������ � xml ������� VAS. ������. �������� ������������ �������� ::ReadCardContext � ::card_authorize15
    PAR_ENCRYPTED_DATA = 8,    ///< [in/out] ����������� ����� ����� � ���� ��������. �������� ��������. ������������ �������� ::card_authorize15 � ��� ��� �� ����� �������������� � �������� �������� ���������. ���� ����������� ������ ���������� ����� �������� track2, �� ������ ������ ���� ��������� �������� 'E' ����� � ����� ������� �������� ��������. �� ���������� ��� ��������� ���. @see ::auth_answer8
    PAR_LLT_DATA = 9,    ///< [out] ��������� � ����� ������ ����������. ������������ �������� ::ReadCardContext
    PAR_AMOUNT_OTHER = 10,   ///< [in] �������������� ����� ���������� (�������� EMV ���� 9F03 ��� SmartVista ���� DF50)
    PAR_REALPAN = 11,   ///< [out] 
    PAR_EXPIRY_DATE = 12,   ///< [out] ���� �������� �����
    PAR_BASKET_ID = 13,   ///< [in] ������. BasketId - ������������� ������� ������� ��� ������ ��� �������� � ����������� ������������.
    PAR_EXTRAHASHTYPE = 14,   ///< [out] ��� ���� N -hash256. 
    PAR_EXTRAHASH = 15,   ///< [out] ���. ������. �������� ������������ �������� ::ReadCardContext
    PAR_PAR = 16    ///< [out]
} EParameterName;



/**
 * @defgroup FINANCIAL ���������� �������� pilot_nt.dll
   @ingroup DOCUMENT_NAME
 */
 /**@{*/

 /** ���� �������� */
typedef enum {
    OP_PURCHASE = 1,   ///< ������ �������
    OP_CASH = 2,   ///< ������ �������� (������ ���)
    OP_RETURN = 3,   ///< ������� ���� ������ �������
    OP_BALANCE = 4,   ///< ������ (������ ���)
    OP_FUNDS = 6,   ///< ������.������� (������ ���)
    OP_PIL_OT_TOTALS = 7, ///< ������ ������
    OP_CANCEL = 8,   ///< ������ �������. ���������� �� ��������� ������ � ������� � ������ ��������� �������. ��� ���������� ������ ������, � ����� pinpad.ini ������ ���� ������ �������� PilotNtSeparateCancelAndRefund=1
    OP_ADD_AUTH = 42,  ///< ���������� ���������������
    OP_CANC_AUTH = 43,  ///< ������ ���������������
    OP_PREAUTH = 51,  ///< ���������������
    OP_COMPLETION = 52,  ///< ���������� �������
    OP_CASHIN = 53,  ///< ����� �������� (������ ���)
    OP_CASHIN_COMP = 54,  ///< ������������� ������ (������ ���)
    OP_PILOT_START = 55,  ///< �������� ��������������� � �� ������ ��������������
    OP_PILOT_STATUS = 56,  ///< �������� ��������������� � �� ������ ��������������
    OP_PILOT_STOP = 57,  ///< �������� ��������������� � �� ������ ��������������
    OP_SETPIN = 58,  ///< ��������� ���� (������ ���)
    OP_CHANGEPIN = 59,  ///< ����� ���� (������ ���)
    OP_SEND_TICKETS = 60,  ///< �������� ������� ������� (��������)
    OP_PUR_CASHBACK = 61,  ///< ������ ������� � ������� ��������
    OP_CASH_ON_TRADE = 62,  ///< ������ �������� � �������� ������
    OP_RFU_CANCEL = 63,///< ������ �������. !!!����� �������� ����������� � ����� �� ����� �������� U2!!! !!!�� ������������!!! �������� ������� � ������ 31.00 � � ������� ����� ���������������� � U2
    OP_PAY_EL_CERT = 88, ///< ������ � �������������� �� ����
    OP_CANCEL_EL_CERT = 89 ///< ������ ������ � �������������� �� ����
} OperationTypes;

#define OpetationTypes OperationTypes //fix spelling error with backward compatibility

struct payment_info_item {
    DWORD  dwTag;                    /**< ��� ��������� ������� */
    char   Value[MAX_PAYMENT_ITEM];  /**< �������� ���� ��������� �������. 128 ����. ::MAX_PAYMENT_ITEM */
    BYTE   Flags;                    /**< must be 0x40 for immediate sending    */
    void* pNextItem;                /**< ?? */
};

/**
 * �������� ��������� ��������.
 * ���������, ������������ ��� �������� �������� � ��������� ����������� ���������� ��������.
 */
 /** @struct auth_answer
  *  @brief �������� ��������� ��������.
  */
typedef struct 
{
    int TType;             /**< [in] ��� ��������. �� ::OpetationTypes */
    unsigned long Amount;  /**< [in] ����� � ��������                    */
    char RCode[3];         /**< [out] ���������� ��� ���������� �����������. �������� 00 �������� ����� ��������, 01-98 - ���������� �� ����� ������ �����������, 99 - ��� ��������� ������, � ��� ����� ��������� �������� � �� �������� � �������� �����.*/
    char AMessage[16];     /**< [out] ����� ���������� �����������       */
    int  CType;            /**< [in,out] ��� �����                       */
    char* Check;           /**< [out] ����� ����, ������ ������������� GlobalFree � ���������� ��������� */
} auth_answer;

/** @struct auth_answer14
 *  @brief ���������� card_authorize13 ������������ ������� ���������� � ������.
 */
typedef struct 
{
    auth_answer   ans;               /**< [in, out]  �������� ��������� ��������. ��. ::auth_answer */
    char   AuthCode[MAX_AUTHCODE];  /**< [out] ��� �����������. 7 ����.              */
    char   CardID[CARD_ID_LEN];      /**< [out] ������������� �����. 25 ����. ��� ������������� ���� ��� �������, ����� ������ 6 � ��������� 4, ����� �������� ��������� �*�.*/
    int    ErrorCode;                /**< [out] ��� ������.                                         */
    char   TransDate[TRANSDATE_LEN]; /**< [out] ���� � ����� ��������                               */
    int    TransNumber;              /**< [out] ����� �������� �� ����. ����, ��. ����� �� ����     */
    int    SberOwnCard;              /**< [out] ���� �������������� ����� ���������                 */
    char   Hash[CARD_HASH_LEN];      /**< [in, out] ��� SHA1 �� ������ �����, � ������� ASCII � ������� ������ � �����. 40 ����.*/
    char   Track3[CARD_TRACK3_LEN];  /**< [out] ������ ������� �����. � PCI DSS �������� �������� ������ � ����� ����������*/
    DWORD  RequestID;                /**< [in,out] ���������� ����� ��������. ������ PCI DSS �������.*/
    DWORD  Department;              /**< [in] ���������� ����� ������ �� 0 �� 14-��, ������������.
                                              ��� ��������� ������ ������ � 0xFFFFFFFF, ����� ������
                                              ����� �������� ����� ��������� ��������� ����� ������� �����.
                                              ���� ����� ������ ����� ������ ��� ������������ ���������,
                                              �� �������� ������ ��� ������ 4191. */
    char   RRN[MAX_REFNUM];          /**< [in,out] ����� ������ ��������, ����������� ������. ������������
                                                  ��� �������� �������, ������������� ����������� � ���������� �������.
                                                  �������� ���������� 12-������� ��������� �����.
                                                  ��� ��������������� ��� ���� �������� ��������
                                                  (��� ��������� ���������� pilot_nt.dll), � ���
                                                  ���������� ������� � ������� (�������� ������
                                                  ���� ��������� ���������� ����������; ��� ������
                                                  ��������� �� ���������, ������������ ��� ���������������).*/
    DWORD  CurrencyCode;             /**< [in] ������������� ��� ������ (810, 643, 840, 978 � �.�.) */
    char   CardEntryMode;            /**< [out] ������ ������ ����� ('D'-����.������, 'M'-������ ����, 'C'-���, 'E'-���������� EMV, 'R'-���������� magstripe, 'F'-fallback, 'Q'-QR ���, 'P'- Bio)*/
    char   CardName[MAX_CARD_NAME_LEN]; /**< [out] �������� ���� ����� */
    char   AID[MAX_AID_ASCII_LEN];   /**< [out] Application ID ������� ����� (� ���� ASCIIZ-������). �� ������������ ��� PCI-DSS �������*/
    char   FullErrorText[MAX_FULL_ERROR_TEXT]; /**< [out] ������ ����� ��������� �� ������*/
    DWORD  GoodsPrice;                /**< [in] ���� �� ������� ������, ��� (34.99->3499)*/
    DWORD  GoodsVolume;               /**< [in] ���������� ������, � ���. ����� (30.234->30234)*/
    char   GoodsCode[MAX_GOODS_CODE + 1]; /**< [in] ��� ������ �� ������� �������.*/
    char   GoodsName[MAX_GOODS_NAME]; /**< [in] ������������ ������ �� ������� �������. ��������! � ��������� auth_answer14 �������� ������ �� ���� ������ ������ ��� � gate.dll TGoodsData. ����������� ��� ������ ��� ��������*/
} auth_answer14;

/// <summary>
/// �������� ���������� ������� 
/// ������� ��������� ������� �������. ��� �������� ���������� ���������� 0 (������ ���������), ��� ��������� � ��� ������ (������ �� ��������� ��� ����������). 
/// </summary>
/// <returns>int ��� ������. </returns>
extern "C" PILOT_NT_API int TestPinpad();

/// <summary>
/// �������� ���.  
/// </summary>/// <param name="auth_answer">���� TType, Amount, CType ��������� �� �����.</param>/// <returns>int ��� ������.</returns>
extern "C" PILOT_NT_API int close_day(auth_answer* auth_answer);

/// <summary>
/// ���������� �������� �� ������ 
/// ������� ������������� ��������� ������� � �������� ��������� ��������. 
/// �� ������� ��������� � ���������� ���������� �������������� ��������� �������� � �������� ��������� ������������ ����������� ��������� ��������. 
/// ���������, ������� ����� �������� ��� �������� ����� �������� �������� ����������� � EParameterName. 
/// </summary>
/// <param name="track2">������ ������� ����� � ��������� �������. ���� NULL, �� ����� ���������� ������� �����.</param>
/// <param name="auth_answer">��. auth_answer14 </param>
/// <param name="">���������� ��� ��������� �������. ������ ���� ����� NULL, ���� �������� �� �������� ����������� ������� ��� �������� �������������� ���������� �� ����.</param>
/// <param name="dataInt">��. ctxAlloc</param>
/// <param name="dataOut">��. ctxAlloc</param>
/// <returns>int ��� ������. </returns>
extern "C" PILOT_NT_API int card_authorize15(const char* track2, auth_answer14* auth_answer, payment_info_item*, CONTEXT_PTR dataInt, CONTEXT_PTR dataOut);

/// <summary>
/// ������� �������� ��������. ������� ������� ������ �������� ��������. 
/// </summary>
/// <returns>CONTEXT_PTR ������������� ��������� �������� ��� 0, ���� ��������� ������ </returns>
extern "C" PILOT_NT_API CONTEXT_PTR ctxAlloc();

/// <summary>
/// ��������� ��������. ������� ������� ��� ��������� �� ���������. 
/// </summary>
/// <param name="ctx">������������� ���������. </param>
/// <returns></returns>
extern "C" PILOT_NT_API void ctxClear(CONTEXT_PTR ctx);

/// <summary>
///������� �������� �������� 
/// </summary>
/// <param name="ctx">������������� ���������.</param>
/// <returns></returns>
extern "C" PILOT_NT_API void ctxFree(CONTEXT_PTR ctx);

/// <summary>
/// ������� �� ��������� �������� ���������� � ���� ������������������ ����. 
/// ��� ��������� ���������� ����� �������� ����� ���������� ��� ���������, 
/// ��� ������������� ���������� ������� ������ ������������������ �� ������� ���� � ������ �������� ����. 
/// ���� ������ ������ ����������, � ���������� pVal ����� ����������� MAXSIZE ����, � ����� ���������� ������� ERR_CTX_GET.
/// </summary>
/// <param name="ctx">������������� ���������. </param>
/// <param name="name">������������� ���������. </param>
/// <param name="pVal">��������� �� ����� ����������. </param>
/// <param name="pOutSz">���������� ����, ������������� � �����.</param>
/// <param name="MAXSIZE">����������� ��������� ���������� ����. </param>
/// <returns>int ��� ������. ERR_OK - ���������� ������� �������. ERR_CTX_GET - ������ ��������� ����������. </returns>
extern "C" PILOT_NT_API int ctxGetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* pVal, int* pOutSz, int MAXSIZE);

/// <summary>
/// ������� �� ��������� �������� ���������� � ���� ������ �����. 
/// ��� ��������� ���������� ����� ��������� �������������� ������ � �����, 
/// ��� ������������������ ���� ������� ����������� ������ ������ ����� ������������������ � ����� ����� � ������ �������� ����. 
/// ��� ����������� ������ �������� *pVal �� ���������� 
/// </summary>
/// <param name="ctx">������������� ���������. </param>
/// <param name="name">������������� ���������. </param>
/// <param name="pVal">��������� �� �����.</param>
/// <returns>int ��� ������. ERR_OK - ���������� ������� �������. ERR_CTX_GET - ������ ��������� ����������. </returns>
extern "C" PILOT_NT_API int ctxGetInt(CONTEXT_PTR ctx, EParameterName name, int* pVal);

/// <summary>
/// ������� �� ��������� �������� ���������� � ���� ������. 
/// ��� ������������� ���������� �� �������� �� ��������� �������������, 
/// � ��� ������������������ ���� ������� ������ hex ������. 
/// ���� ������ ������ ����������, � ���������� pVal ����� ����������� sz-1 ���� + ����������� ����, � ����� ���������� ������� ERR_CTX_GET. 
/// </summary>
/// <param name="ctx">������������� ���������. </param>
/// <param name="name">������������� ���������. </param>
/// <param name="str">��������� �� ������. </param>
/// <param name="sz">����������� ��������� ����� ������. </param>
/// <returns>int ��� ������. </returns>
extern "C" PILOT_NT_API int ctxGetString(CONTEXT_PTR ctx, EParameterName name, char* str, int sz);

/// <summary>
/// �������� � �������� �������� ��������� � ���� ������������������ ����. 
/// </summary>
/// <param name="ctx">������������� ���������. </param>
/// <param name="name">������������� ���������. </param>
/// <param name="val">��������� �� �����. </param>
/// <param name="sz">����� ������. </param>
/// <returns>int ��� ������. </returns>
extern "C" PILOT_NT_API int ctxSetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* val, int sz);

/// <summary>
/// �������� � �������� �������� �������������� ���������. 
/// </summary>
/// <param name="ctx">������������� ���������. </param>
/// <param name="name">������������� ���������. </param>
/// <param name="val">�������� ���������. </param>
/// <returns>int ��� ������. </returns>
extern "C" PILOT_NT_API int ctxSetInt(CONTEXT_PTR ctx, EParameterName name, int val);

/// <summary> 
/// �������� � �������� �������� ���������� ���������. 
/// </summary>
/// <param name="ctx">������������� ���������. </param>
/// <param name="name">������������� ���������.</param>
///  <param name="str">��������� �� ������. </param>
///  <returns>int ��� ������</returns>
extern "C" PILOT_NT_API int ctxSetString(CONTEXT_PTR ctx, EParameterName name, const char* str);