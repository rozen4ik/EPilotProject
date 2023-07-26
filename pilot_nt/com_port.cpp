#include "pch.h"
#include "com_port.h"
#include <iostream>
#include <vector>
#include <fstream>


void open_port(HANDLE* hSerial)
{
	LPCTSTR sPortName = L"COM2";
    int speed = 115200;
    //read_ini_file(sPortName, speed);

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
    dcb.BaudRate = speed;
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
    char sReceivedChar = '\0';
    while (sReceivedChar != '\u0003')
    {
        BOOL iRet = ReadFile(*hSerial, &sReceivedChar, 1, &iSize, 0);  // �������� 1 ����
        if (iSize > 0)   // ���� ���-�� �������, �������
        {
            data += sReceivedChar; 
        }                   
        else if (sReceivedChar == '\u0003')
            break;
    }
}

void write_port(HANDLE* hSerial, char* data, const int size)
{   
    DWORD dwSize = sizeof(*data);
    DWORD dwBytesWritten;

    while (*data != '\0')
    {
        //std::cout << *data;
        BOOL iRet = WriteFile(*hSerial, data, dwSize, &dwBytesWritten, NULL);
        data++;
    }    
}

void close_port(HANDLE* hSerial)
{
    CloseHandle(*hSerial);
}

void read_ini_file(LPCTSTR& sPortName, int& speedPort)
{
    std::ifstream in("pinpad.ini");
    std::string line;
    std::string speed = "";
    int cout = 0;

    if (in.is_open())
    {
        while (std::getline(in, line))
        {
            size_t position = line.find("ComPort=");
            if (position != std::string::npos)
            {
                position = line.find("=");
                for (int i = position + 1; i < line.length(); i++)
                    sPortName += line[i];
                cout++;
            }
            else
            {
                std::cout << "Net porta" << std::endl;
            }

            position = line.find("Speed=");
            if (position != std::string::npos)
            {
                position = line.find("=");
                for (int i = position + 1; i < line.length(); i++)
                {
                    speed += line[i];
                }
                speedPort = std::stoi(speed);
                cout++;
            }
            else
            {
                std::cout << "Net porta" << std::endl;
            }


            if (cout == 2) break;
        }
    }

    in.close();
}
