#include "pch.h"
#include "com_port.h"
#include <iostream>
#include <vector>


void open_port(HANDLE* hSerial)
{
	LPCTSTR sPortName = L"COM2";
	*hSerial = ::CreateFile(sPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (*hSerial == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            std::cout << "serial port does not exist.\n";
        }
        std::cout << "some other error occurred.\n";
    }

    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(dcb);

    if (!GetCommState(*hSerial, &dcb))
    {
        std::cout << "getting state error\n";
    }
    dcb.BaudRate = 115200;
    dcb.fBinary = 1;
    dcb.fParity = 1;
    dcb.fOutxCtsFlow = 0;
    dcb.fOutxDsrFlow = 0;
    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fDsrSensitivity = 0;
    dcb.fTXContinueOnXoff = 0;
    dcb.fOutX = 0;
    dcb.fInX = 0;
    dcb.fErrorChar = 0;
    dcb.fNull = 0;
    dcb.fRtsControl = RTS_CONTROL_ENABLE;
    dcb.fAbortOnError = 0;
    dcb.ByteSize = 8;
    dcb.Parity = 0;
    dcb.StopBits = 0;
    dcb.EvtChar = 0;
    dcb.EofChar = 0;
    
    if (!SetCommState(*hSerial, &dcb))
    {
        std::cout << "error setting serial port state\n";
    }
}

void read_port(HANDLE* hSerial, std::string& data)
{
    DWORD iSize;
    char sReceivedChar;
    while (true)
    {
        BOOL iRet = ReadFile(*hSerial, &sReceivedChar, 1, &iSize, 0);  // получаем 1 байт
        if (iSize > 0)   // если что-то принято, выводим
        {
            data += sReceivedChar;
        }                   
        else if (sReceivedChar == '\u0003' || iSize == 0)
            break;
    }
}

void write_port(HANDLE* hSerial, char* data, const int size)
{   
    std::string str = "";

    for (int i = 0; i < size; i++)
    {
        std::cout << data[i];
        str += data[i];
    }     

    std::cout << std::endl;

    const int length = str.length();
    char* char_array = new char[size + 1];
    strcpy(char_array, data);

    DWORD dwSize = sizeof(*data);
    DWORD dwBytesWritten;

    while (*data != '\0')
    {
        std::cout << *data;
        BOOL iRet = WriteFile(*hSerial, data, dwSize, &dwBytesWritten, NULL);
        data++;
    }    

    std::cout << std::endl;
}

void close_port(HANDLE* hSerial)
{
    CloseHandle(*hSerial);
}
