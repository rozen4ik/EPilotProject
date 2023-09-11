#pragma once

#include "pilot_nt.h"
#include "cmd_msb.h"
#include "PacketProcessing.h"
#include "base64.h"
#include <iostream>
#include <unordered_map>
#include "ComPort.h"

class PilotService
{
public:
	std::unordered_map<CONTEXT_PTR, auth_answer14> dContext;
	std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>> DTagExtraData;
	std::unordered_map<ResponseRCardContext, std::vector<unsigned char>> resRecCard;
	std::unordered_map<std::string, int>runCardAuth;
	std::string str;
	std::string rStr;


	int TestPinpad(); ///< Проверка готовности пинпада
	int close_day(auth_answer* auth_answer, std::string& check); ///< Закрытие дня. 
	int card_authorize(const char* track2, auth_answer* auth_answer); ///< Выполнение операций по картам
	int card_authorize15(const char* track2, auth_answer14& auth_answer, payment_info_item& payinfo, CONTEXT_PTR dataIn, CONTEXT_PTR dataOut, std::string& check); ///< Выполнение операций по картам
	CONTEXT_PTR ctxAlloc(std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context); ///< Создать контекст операции. Функция создает пустой контекст операции. 
	void ctxClear(CONTEXT_PTR ctx); ///< Отчистить контекст. Функция удаляет все параметры из контекста.
	void ctxFree(CONTEXT_PTR ctx, std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context); ///< Удалить контекст операции 
	int ctxGetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* pVal, int* pOutSz, int MAXSIZE, std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context); ///< Считать из контекста значение переменной в виде последовательности байт.
	int ctxGetInt(CONTEXT_PTR ctx, EParameterName name, int* pVal, std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context); ///< Считать из контекста значение переменной в виде целого числа. 
	int ctxGetString(CONTEXT_PTR ctx, EParameterName name, char* str, int sz, std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context); ///< Считать из контекста значение переменной в виде строки. 
	int ctxSetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* val, int sz, std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context); ///< Записать в контекст значение параметра в виде последовательности байт. 
	int ctxSetInt(CONTEXT_PTR ctx, EParameterName name, int val, std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context); ///< Записать в контекст значение целочисленного параметра. 
	int ctxSetString(CONTEXT_PTR ctx, EParameterName name, const char* str, std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context); ///< Записать в контекст значение строкового параметра. 
	int ReadCardContext(CONTEXT_PTR dataOut, std::unordered_map<CONTEXT_PTR, std::vector<std::unordered_map<std::string, std::vector<unsigned char>>>>& map_context); ///< Чтение карты 
	int CommitTrx(DWORD dwAmount, char* pAuthCode); ///< Функция возвращает последнюю успешную транзакцию в «нормальное» состояние. После этого транзакция будет включена в отчет и спроцессирована как успешная. Перевести ее снова в «подвешенное» состояние будет уже нельзя.
	int RollbackTrx(DWORD dwAmount, char* pAuthCode); ///< Функция вызывает немедленную отмену последней успешной операции 
	int SuspendTrx(DWORD dwAmount, char* pAuthCode); ///< Функция переводит последнюю успешную транзакцию в «подвешенное» состояние. Если транзакция находится в этом состоянии, то при следующем сеансе связи с банком она будет отменена.. 
	int AbortTransaction(); ///< Функция прерывает работу функций card_authorizeX() 
	void Done(); ///< Деинициализация библиотеки pilot_nt.dll. 

private:

};

