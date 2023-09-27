#include "pch.h"
#include "ComPort.h"

ComPort::ComPort()
{
    std::string line;
    std::string comPort;
    std::string speed;
    size_t index = 0;

    wchar_t wPathToDll[MAX_PATH];

    GetModuleFileName(GetThisDllHandle(), wPathToDll, MAX_PATH);

    std::wstring wStringPathToDll = wPathToDll;
    std::string pathToDll(wStringPathToDll.begin(), wStringPathToDll.end());
    std::string del = "pilot_nt.dll";
    std::string::size_type pos = pathToDll.find("pilot_nt.dll");
    pathToDll.erase(pos, del.size());
    pathToDll += "pinpad.ini";

    std::cout << "Путь к dll" << std::endl;
    std::cout << pathToDll << std::endl;

    std::string rusPathToDll = pathToDll;
    OemToCharBuffA(pathToDll.c_str(), &rusPathToDll[0], pathToDll.size());

    std::ifstream in(pathToDll);

    if (in.is_open())
    {
        while (std::getline(in, line))
        {
            if (line.find("ComPort") != std::string::npos)
            {
                index = line.find("=") + 1;
                for (int i = index; i < line.size(); i++)
                    comPort += line[i];
            }
            else if (line.find("Speed") != std::string::npos)
            {
                index = line.find("=") + 1;
                for (int i = index; i < line.size(); i++)
                    speed += line[i];
            }
        }
    }

    in.close();

    if (std::stoi(comPort) <= 9)
        settings["ComPort"] = "COM" + comPort;
    else
        settings["ComPort"] = "\\\\.\\COM" + comPort;
    settings["Speed"] = speed;
    Logger(settings["ComPort"]);
    Logger(settings["Speed"]);
}

void ComPort::OpenPort()
{
    std::cout << "Данные из ини файла получены" << std::endl;
    std::wstring nPort(settings["ComPort"].begin(), settings["ComPort"].end());
    int speed = std::stoi(settings["Speed"]);
    LPCTSTR sPortName = nPort.c_str();

    hSerial = ::CreateFile(sPortName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (hSerial == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            Logger("Невозможно открыть последовательный порт");
            //std::cout << "Невозможно открыть последовательный порт.\n";
        }
        Logger("some other error occurred.");
        //exit(EXIT_FAILURE);
        //std::cout << "some other error occurred.\n";
    }

    SetCommMask(hSerial, EV_RXCHAR);
    SetupComm(hSerial, 3000, 3000);

    COMMTIMEOUTS CommTimeOuts;
    CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
    CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
    CommTimeOuts.ReadTotalTimeoutConstant = 3000;
    CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
    CommTimeOuts.WriteTotalTimeoutConstant = 3000;

    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(dcb);

    if (!GetCommState(hSerial, &dcb))
    {
        Logger("getting state error");
        //std::cout << "getting state error\n";
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

    if (!SetCommState(hSerial, &dcb))
    {
        Logger("error setting serial port state");
        //std::cout << "error setting serial port state\n";
    }
}

void ComPort::ReadPort(std::string& data)
{
    DWORD iSize;
    char sReceivedChar = '\0';

    while (true)
    {
        BOOL iRet = ReadFile(hSerial, &sReceivedChar, 1, &iSize, 0);  // получаем 1 байт
        if (iSize > 0)   // если что-то принято, выводим
        {
            if (sReceivedChar == '\u0003')
            {
                data += sReceivedChar;
                break;
            }
            data += sReceivedChar;
        }
        //else if (sReceivedChar == '\u0003')
        //    break;
    }
}

void ComPort::WritePort(char* data, const int size)
{
    DWORD dwSize = sizeof(*data);
    DWORD dwBytesWritten;

    while (*data != '\0')
    {
        //std::cout << *data;
        BOOL iRet = WriteFile(hSerial, data, dwSize, &dwBytesWritten, NULL);
        data++;
    }
    
    std::cout << dwSize << " - " << dwBytesWritten << std::endl;
}

void ComPort::ClosePort()
{
    CloseHandle(hSerial);
}

void ComPort::IOPort(const std::string& inData, std::string& outData)
{
    OpenPort();
    std::cout << "Отправлено в порт" << std::endl;
    Logger("-> " + inData);
    std::cout << inData << std::endl;
    PurgeComm(hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR);
    while (outData == "")
    {        
        WritePort(const_cast<char*>(inData.data()), inData.length());
        Logger("->");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        ReadPort(outData);
        Logger("<-");
        std::cout << "Получено из порта" << std::endl;
    }
    std::cout << outData << std::endl;
    Logger("<- " + outData);
    std::cout << std::endl;
    ClosePort();
}
