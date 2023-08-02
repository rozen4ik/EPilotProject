#pragma once

#include "paramsln.h"
#include <windows.h>

#ifdef PILOT_NT_EXPORTS
#define PILOT_NT_API __declspec(dllexport)
#else
#define PILOT_NT_API __declspec(dllimport)
#endif


/** Идентификатор контекста операции */
typedef long CONTEXT_PTR;


/** Идентификаторы параметров, которые можно получить или передать через контекст операции */
typedef enum 
{
    PAR_LLT_ID = 1,    ///< [out] Номер программы лояльности в который попала карта, целое. Параметр возвращается при вызове функций ::card_authorize15 и ::ReadCardContext
    PAR_PAN = 2,    ///< [out] Маскированный номер карты, строка. Параметр возвращается при вызове функций ::ReadCardContext
    PAR_HASH = 3,    ///< [out] Хэш. Строка. Параметр возвращается функцией ::ReadCardContext
    PAR_CARD_TYPE = 4,    ///< [in/out] Тип карты. Перечисление ::CardTypes. Параметр используется функциями ::ReadCardContext и ::card_authorize15 в качестве входного для управления чтением VAS и ::ReadCardContext в качестве выходного параметра. 
    PAR_OWN_CARD = 5,    ///< [out] Признак "Карта выпущена Сбербанком". Целое. Параметр возвращается функцией ::ReadCardContext
    PAR_VAS_LENGTH = 6,    ///< [out] Длина строки с xml данными VAS без учета завершающего ноля. Целое. Параметр возвращается функцией ::ReadCardContext и ::card_authorize15
    PAR_VAS_STRING = 7,    ///< [out] Строка с xml данными VAS. Строка. Параметр возвращается функцией ::ReadCardContext и ::card_authorize15
    PAR_ENCRYPTED_DATA = 8,    ///< [in/out] Шифрованный номер карты и срок действия. Бинарное значение. Возвращается функцией ::card_authorize15 и для нее же может использоваться в качестве входного параметра. Если шифрованные данные передаются через аргумент track2, то данные должны быть дополнены символом 'E' слева и после следует бинарное значение. Не реализован для протокола ТТК. @see ::auth_answer8
    PAR_LLT_DATA = 9,    ///< [out] Считанные с карты данные лояльности. Возвращается функцией ::ReadCardContext
    PAR_AMOUNT_OTHER = 10,   ///< [in] Дополнительная сумма транзакции (значение EMV тега 9F03 или SmartVista тега DF50)
    PAR_REALPAN = 11,   ///< [out] 
    PAR_EXPIRY_DATE = 12,   ///< [out] срок действия карты
    PAR_BASKET_ID = 13,   ///< [in] Строка. BasketId - идентификатор корзины товаров для оплаты или возврата с электронным сертификатом.
    PAR_EXTRAHASHTYPE = 14,   ///< [out] Тип Хэша N -hash256. 
    PAR_EXTRAHASH = 15,   ///< [out] Хэш. Строка. Параметр возвращается функцией ::ReadCardContext
    PAR_PAR = 16    ///< [out]
} EParameterName;



/**
 * @defgroup FINANCIAL Финансовые операции pilot_nt.dll
   @ingroup DOCUMENT_NAME
 */
 /**@{*/

 /** Типы операций */
typedef enum 
{
    OP_PURCHASE = 1,   ///< Оплата покупки
    OP_CASH = 2,   ///< Выдача наличных (только ВСП)
    OP_RETURN = 3,   ///< Возврат либо отмена покупки
    OP_BALANCE = 4,   ///< Баланс (только ВСП)
    OP_FUNDS = 6,   ///< Безнал.перевод (только ВСП)
    OP_PIL_OT_TOTALS = 7, ///< Сверка итогов
    OP_CANCEL = 8,   ///< Отмена покупки. Библиотека не разделяет отмену и возврат и всегда выполняет возврат. Для выполнения только отмены, в файле pinpad.ini должен быть указан параметр PilotNtSeparateCancelAndRefund=1
    OP_ROLLBACK = 13, ///< Аварийная отмена
    OP_SUSPEND = 15, ///< Приастоновка транзакции
    OP_COMMIT = 16, ///< Фиксация транзакции
    OP_READ_CARD = 20, ///< Ожидание карты
    OP_ADD_AUTH = 42,  ///< Добавочная предавторизация
    OP_CANC_AUTH = 43,  ///< Отмена предавторизации
    OP_PREAUTH = 51,  ///< Предавторизация
    OP_COMPLETION = 52,  ///< Завершение расчета
    OP_CASHIN = 53,  ///< Взнос наличных (только ВСП)
    OP_CASHIN_COMP = 54,  ///< Подтверждение взноса (только ВСП)
    OP_PILOT_START = 55,  ///< Операция зарезервирована и не должна использоваться
    OP_PILOT_STATUS = 56,  ///< Операция зарезервирована и не должна использоваться
    OP_PILOT_STOP = 57,  ///< Операция зарезервирована и не должна использоваться
    OP_SETPIN = 58,  ///< Установка ПИНа (только ВСП)
    OP_CHANGEPIN = 59,  ///< Смена ПИНа (только ВСП)
    OP_SEND_TICKETS = 60,  ///< Передача номеров билетов (Аэрофлот)
    OP_PUR_CASHBACK = 61,  ///< Оплата покупки с выдачей наличных
    OP_CASH_ON_TRADE = 62,  ///< Выдача наличных в торговых точках
    OP_RFU_CANCEL = 63,///< Отмена покупки. !!!Номер операции конфликтует с таким же кодом операции U2!!! !!!Не использовать!!! Операция удалена в версии 31.00 и в будущем будет синхронизирована с U2
    OP_PAY_EL_CERT = 88, ///< Оплата с использованием ЭС НСПК
    OP_CANCEL_EL_CERT = 89 ///< Отмена оплаты с использованием ЭС НСПК
} OperationTypes;

typedef enum
{
    ERROR_CODE = 0, ///< Результат выполнения транзакции 2 байт    
    CODE_AUTH = 1, ///< Код авторизации 7 байт
    RRN = 2, ///< Ссылочный номер RRN 13 байт
    NUMBER_OPERATION_DAY = 3, ///< Порядковый номер операции за день 5 байт
    NUMBER_CARD = 4, ///< Номер карты 20 байт
    CARD_DATE = 5, ///< Срок действия карты 6 байт
    TEXT_MESSAGE_ERROR = 6, ///< Текстовое сообщение об ошибке 32 байт
    DATE_OPERATION = 7, ///< Дата операции 4 байт
    TIME_OPERATION = 8, ///< Время операции 4 байт
    BANK_AFFILIATION = 9, ///< Принадлежность карты Сбербанку 1 байт
    NUMBER_TERMINAL = 10, ///< Номер терминала 9 байт
    NAME_CARD = 11, ///< Название карты 32 байт
    HASH_CARD = 12, ///< SHA1 (номер карты) Хеш от номера карты 20 байт
    ECRYPTED_DATA = 13, ///< Шифрованные данные 32 байт
    CARD_ID = 14 ///< CardID 1 байт
} ResponseRCardContext;

#define OpetationTypes OperationTypes //fix spelling error with backward compatibility

struct payment_info_item 
{
    DWORD  dwTag;                    /**< Тег платежной системы */
    char   Value[MAX_PAYMENT_ITEM];  /**< Значение тэга платежной системы. 128 байт. ::MAX_PAYMENT_ITEM */
    BYTE   Flags;                    /**< must be 0x40 for immediate sending    */
    void* pNextItem;                /**< ?? */
};

/**
 * Основные параметры операции.
 * Структура, используемая для описания операции и получения результатов выполнения операции.
 */
 /** @struct auth_answer
  *  @brief Основные параметры операции.
  */
typedef struct 
{
    int TType;             /**< [in] тип операции. см ::OpetationTypes */
    unsigned long Amount;  /**< [in] сумма в копейках                    */
    char RCode[3];         /**< [out] двузначный код результата авторизации. Значение 00 означает успех операции, 01-98 - полученные от хоста ошибки авторизации, 99 - все остальные ошибки, в том числе возникшие локально и не свзанные с ответами хоста.*/
    char AMessage[16];     /**< [out] текст результата авторизации       */
    int  CType;            /**< [in,out] тип карты                       */
    char* Check;           /**< [out] образ чека, должен освобождаться GlobalFree в вызывающей программе */
} auth_answer;

/** @struct auth_answer14
 *  @brief Расширение card_authorize13 возможностью указать информацию о товаре.
 */
typedef struct 
{
    auth_answer   ans;               /**< [in, out]  Основные параметры операции. См. ::auth_answer */
    char   AuthCode[MAX_AUTHCODE];  /**< [out] Код авторизации. 7 байт.              */
    char   CardID[CARD_ID_LEN];      /**< [out] Идентификатор карты. 25 байт. Для международных карт все символы, кроме первых 6 и последних 4, будут заменены символами ‘*’.*/
    int    ErrorCode;                /**< [out] Код ошибки.                                         */
    char   TransDate[TRANSDATE_LEN]; /**< [out] Дата и время операции                               */
    int    TransNumber;              /**< [out] Номер операции за опер. день, см. номер на чеке     */
    int    SberOwnCard;              /**< [out] Флаг принадлежности карты Сбербанку                 */
    char   Hash[CARD_HASH_LEN];      /**< [in, out] хеш SHA1 от номера карты, в формате ASCII с нулевым байтом в конце. 40 байт.*/
    char   Track3[CARD_TRACK3_LEN];  /**< [out] третья дорожка карты. В PCI DSS решениях содержит данные с карты лояльности*/
    DWORD  RequestID;                /**< [in,out] Уникальный номер операции. Только PCI DSS решения.*/
    DWORD  Department;              /**< [in] Порядковый номер отдела от 0 до 14-ти, включительно.
                                              При установке номера отдела в 0xFFFFFFFF, номер отдела
                                              будет запрошен через интерфейс терминала после вставки карты.
                                              Если номер отдела будет указан вне настроенного диапазона,
                                              то терминал вернет код ошибки 4191. */
    char   RRN[MAX_REFNUM];          /**< [in,out] Номер ссылки операции, присвоенный хостом. Используется
                                                  для операций возврат, множественной авторизации и завершения расчета.
                                                  Содержит уникальный 12-значный ссылочный номер.
                                                  При предавторизации это поле является выходным
                                                  (его заполняет библиотека pilot_nt.dll), а при
                                                  завершении расчета – входным (значение должно
                                                  быть заполнено вызывающей программой; оно должно
                                                  совпадать со значением, возвращенным при предавторизации).*/
    DWORD  CurrencyCode;             /**< [in] Международный код валюты (810, 643, 840, 978 и т.д.) */
    char   CardEntryMode;            /**< [out] Способ чтения карты ('D'-магн.полоса, 'M'-ручной ввод, 'C'-чип, 'E'-бесконтакт EMV, 'R'-бесконтакт magstripe, 'F'-fallback, 'Q'-QR код, 'P'- Bio)*/
    char   CardName[MAX_CARD_NAME_LEN]; /**< [out] Название типа карты */
    char   AID[MAX_AID_ASCII_LEN];   /**< [out] Application ID чиповой карты (в виде ASCIIZ-строки). Не используется для PCI-DSS решений*/
    char   FullErrorText[MAX_FULL_ERROR_TEXT]; /**< [out] Полный текст сообщения об ошибке*/
    DWORD  GoodsPrice;                /**< [in] Цена за единицу товара, коп (34.99->3499)*/
    DWORD  GoodsVolume;               /**< [in] Количество товара, в тыс. долях (30.234->30234)*/
    char   GoodsCode[MAX_GOODS_CODE + 1]; /**< [in] Код товара во внешней системе.*/
    char   GoodsName[MAX_GOODS_NAME]; /**< [in] Наименование товара во внешней системе. Внимание! В структуре auth_answer14 название товара на один символ короче чем в gate.dll TGoodsData. Зафиксируем эту ошибку как стандарт*/
} auth_answer14;

/// <summary>
/// Проверка готовности пинпада 
/// Функция проверяет наличие пинпада. При успешном выполнении возвращает 0 (пинпад подключен), при неудачном – код ошибки (пинпад не подключен или неисправен). 
/// </summary>
/// <returns>int Код ошибки. </returns>
extern "C" PILOT_NT_API int TestPinpad();

/// <summary>
/// Закрытие дня.  
/// </summary>/// <param name="auth_answer">Поля TType, Amount, CType заполнять не нужно.</param>/// <returns>int Код ошибки.</returns>
extern "C" PILOT_NT_API int close_day(auth_answer* auth_answer);

/// <summary>
/// Выполнение операций по картам 
/// Функция дополнительно принимает входной и выходной контексты операции. 
/// Во входном контексте в библиотеку передаются дополнительные параметры операции в выходном контексте возвращается расширенный результат операции. 
/// Параметры, которые могут переданы или получены через контекст операции перечислены в EParameterName. 
/// </summary>
/// <param name="track2">данные дорожки карты с магнитной полосой. Если NULL, то будет предложено считать карту.</param>
/// <param name="auth_answer">см. auth_answer14 </param>
/// <param name="payinfo">Информация для платежной системы. Должен быть равен NULL, если терминал не настроен специальным образом для передачи дополнительных параметров на хост.</param>
/// <param name="dataInt">см. ctxAlloc</param>
/// <param name="dataOut">см. ctxAlloc</param>
/// <returns>int Код ошибки. </returns>
extern "C" PILOT_NT_API int card_authorize15(const char* track2, auth_answer14* auth_answer, payment_info_item* payinfo, CONTEXT_PTR dataIn, CONTEXT_PTR dataOut);

/// <summary>
/// Создать контекст операции. Функция создает пустой контекст операции. 
/// </summary>
/// <returns>CONTEXT_PTR Идентификатор контекста операции или 0, если произошла ошибка </returns>
extern "C" PILOT_NT_API CONTEXT_PTR ctxAlloc();

/// <summary>
/// Отчистить контекст. Функция удаляет все параметры из контекста. 
/// </summary>
/// <param name="ctx">Идентификатор контекста. </param>
/// <returns></returns>
extern "C" PILOT_NT_API void ctxClear(CONTEXT_PTR ctx);

/// <summary>
///Удалить контекст операции 
/// </summary>
/// <param name="ctx">Идентификатор контекста.</param>
/// <returns></returns>
extern "C" PILOT_NT_API void ctxFree(CONTEXT_PTR ctx);

/// <summary>
/// Считать из контекста значение переменной в виде последовательности байт. 
/// Для строковой переменной будет значение будет возвращено без изменения, 
/// для целочисленной переменной функция вернет последовательность из четырех байт с прямым порядком байт. 
/// Если размер буфера недостачен, в переменную pVal будет скопировано MAXSIZE байт, а вызов завершится ошибкой ERR_CTX_GET.
/// </summary>
/// <param name="ctx">Идентификатор контекста. </param>
/// <param name="name">Идентификатор параметра. </param>
/// <param name="pVal">Указатель на буфер результата. </param>
/// <param name="pOutSz">Количество байт, скопированных в буфер.</param>
/// <param name="MAXSIZE">Максимально возможное количество байт. </param>
/// <returns>int Код ошибки. ERR_OK - переменная считана успешно. ERR_CTX_GET - ошибка получения переменной. </returns>
extern "C" PILOT_NT_API int ctxGetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* pVal, int* pOutSz, int MAXSIZE);

/// <summary>
/// Считать из контекста значение переменной в виде целого числа. 
/// Для строковой переменной будет выполнено преобразование строки в число, 
/// для последовательности байт функция преобразует первые четыре байта последовательности в целое число с прямым порядком байт. 
/// При возвращении ошибки значение *pVal не изменяется 
/// </summary>
/// <param name="ctx">Идентификатор контекста. </param>
/// <param name="name">Идентификатор параметра. </param>
/// <param name="pVal">Указатель на число.</param>
/// <returns>int Код ошибки. ERR_OK - переменная считана успешно. ERR_CTX_GET - ошибка получения переменной. </returns>
extern "C" PILOT_NT_API int ctxGetInt(CONTEXT_PTR ctx, EParameterName name, int* pVal);

/// <summary>
/// Считать из контекста значение переменной в виде строки. 
/// Для целочисленной переменной вы получите ее строковое представление, 
/// а для последовательности байт функция вернет hex строку. 
/// Если размер буфера недостачен, в переменную pVal будет скопировано sz-1 байт + завершающий ноль, а вызов завершится ошибкой ERR_CTX_GET. 
/// </summary>
/// <param name="ctx">Идентификатор контекста. </param>
/// <param name="name">Идентификатор параметра. </param>
/// <param name="str">Указатель на строку. </param>
/// <param name="sz">Максимально возможная длина строки. </param>
/// <returns>int Код ошибки. </returns>
extern "C" PILOT_NT_API int ctxGetString(CONTEXT_PTR ctx, EParameterName name, char* str, int sz);

/// <summary>
/// Записать в контекст значение параметра в виде последовательности байт. 
/// </summary>
/// <param name="ctx">Идентификатор контекста. </param>
/// <param name="name">Идентификатор параметра. </param>
/// <param name="val">Указатель на буфер. </param>
/// <param name="sz">Длина буфера. </param>
/// <returns>int Код ошибки. </returns>
extern "C" PILOT_NT_API int ctxSetBinary(CONTEXT_PTR ctx, EParameterName name, unsigned char* val, int sz);

/// <summary>
/// Записать в контекст значение целочисленного параметра. 
/// </summary>
/// <param name="ctx">Идентификатор контекста. </param>
/// <param name="name">Идентификатор параметра. </param>
/// <param name="val">Значение параметра. </param>
/// <returns>int Код ошибки. </returns>
extern "C" PILOT_NT_API int ctxSetInt(CONTEXT_PTR ctx, EParameterName name, int val);

/// <summary> 
/// Записать в контекст значение строкового параметра. 
/// </summary>
/// <param name="ctx">Идентификатор контекста. </param>
/// <param name="name">Идентификатор параметра.</param>
///  <param name="str">Указатель на строку. </param>
///  <returns>int Код ошибки</returns>
extern "C" PILOT_NT_API int ctxSetString(CONTEXT_PTR ctx, EParameterName name, const char* str);

/// <summary>
/// Чтение карты Если требуется чтение нестандартных карт, например карт лояльности на базе Mifare или Google VAS, в контекст операции должен быть передан соответсвующий тип карты.
/// </summary>
/// <param name="dataOut">Контекст операции, в которую копируются pan, хэш, тип карты, признак "Карта выпущена Сбербанком", номер программы лояльности, признаки считывания VAS и т.п.</param>
/// <returns>int Код ошибки. </returns>
extern "C" PILOT_NT_API int ReadCardContext(CONTEXT_PTR dataOut);

/// <summary>
/// Функция возвращает последнюю успешную транзакцию в «нормальное» состояние. 
/// После этого транзакция будет включена в отчет и спроцессирована как успешная. 
/// Перевести ее снова в «подвешенное» состояние будет уже нельзя. 
/// Функция сверяет переданные извне параметры (сумму и код авторизации) со значениями в последней успешной операции, которая была проведена через библиотеку. 
/// Если хотя бы один параметр не совпадает, функция возвращает код ошибки 4140 и не выполняет никаких действий. 
/// </summary>
/// <param name="dwAmount">Сумма операции (в копейках)</param>
/// <param name="pAuthCode">Код авторизации.</param>
/// <returns>int Код ошибки. </returns>
extern "C" PILOT_NT_API int CommitTrx(DWORD dwAmount, char* pAuthCode);

/// <summary>
/// Функция вызывает немедленную отмену последней успешной операции 
/// Операция может быть предварительно возможно, ранее переведенную в «подвешенное» состояние, хотя это и не обязательно). 
/// Если транзакция уже была возвращена в «нормальное» состояние функцией CommitTrx(), то функция RollbackTrx() завершится с кодом ошибки 4141, 
/// не выполняя никаких действий. Функция сверяет переданные извне параметры (сумму и код авторизации) со значениями в последней успешной операции, 
/// которая была проведена через библиотеку. Если хотя бы одинпараметр не совпадает, функция возвращает код ошибки 4140 и не выполняет никаких действий.
///</summary>
///<param name="dwAmount">Сумма операции (в копейках) </param> 
/// <param name="pAuthCode">Код авторизации.</param>
/// <returns>int Код ошибки. </returns>
extern "C" PILOT_NT_API int RollbackTrx(DWORD dwAmount, char* pAuthCode);

/// <summary>
/// Функция переводит последнюю успешную транзакцию в «подвешенное» состояние. 
/// Если транзакция находится в этом состоянии, то при следующем сеансе связи с банком она будет отменена.
/// Функция сверяет переданные извне параметры (сумму и код авторизации) со значениями в последней успешной операции, 
/// которая была проведена через библиотеку. Если хотя бы один параметр не совпадает, функция возвращает код ошибки 4140 и не выполняет никаких действий.
///</summary>
/// <param name="dwAmount">Сумма операции (в копейках) </param>
/// <param name="pAuthCode">Код авторизации. </param>
/// <returns>int Код ошибки. </returns>
extern "C" PILOT_NT_API int SuspendTrx(DWORD dwAmount, char* pAuthCode);

/// <summary>
/// Функция прерывает работу функций card_authorizeX() 
/// Внешнее ПО может вызвать эту функцию из отдельного треда, чтобы досрочно прекратить выполнение любой из функций card_authorize…(). 
/// При этом функция card_authorize…() завершится с кодом ошибки 2000 (операция прервана). 
/// </summary> 
/// <returns>int Код ошибки. </returns>
extern "C" PILOT_NT_API int AbortTransaction();

/// <summary>
/// Выполняется отключение от библиотеки gate.dll 
/// </summary>
/// <returns></returns>
extern "C" PILOT_NT_API void Done();