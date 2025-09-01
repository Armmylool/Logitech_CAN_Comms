#include <windows.h>
#include <stdio.h>

int main() {
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};
    char szPortName[] = "\\\\.\\COM11"; // แก้ไขเป็น COM Port ของคุณ
    char writeBuffer[32];
    char readBuffer[64] = {0};
    DWORD bytesWritten, bytesRead;

    // 1. เปิด Serial Port
    hSerial = CreateFile(szPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hSerial == INVALID_HANDLE_VALUE) {
        printf("Error opening serial port\n");
        return 1;
    }

    // 2. ตั้งค่า Serial Port (Baud rate, etc.)
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) { /* error getting state */ }
    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if(!SetCommState(hSerial, &dcbSerialParams)){ /* error setting state */ }

    // 3. ตั้งค่า Timeouts
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if(!SetCommTimeouts(hSerial, &timeouts)){ /* error setting timeouts */ }

    // 4. ส่งคำสั่งเพื่อตั้งค่าและเปิด CAN channel
    sprintf(writeBuffer, "S8\r"); // Set bitrate to 500k
    WriteFile(hSerial, writeBuffer, strlen(writeBuffer), &bytesWritten, NULL);

    sprintf(writeBuffer, "O\r"); // Open channel
    WriteFile(hSerial, writeBuffer, strlen(writeBuffer), &bytesWritten, NULL);

    // 5. ส่ง CAN frame (ID: 123, DLC: 8, Data: 01 02 03 04 05 06 07 08)
    sprintf(writeBuffer, "t12380102030405060708\r");
    WriteFile(hSerial, writeBuffer, strlen(writeBuffer), &bytesWritten, NULL);
    printf("Sent CAN frame.\n");

    // 6. วนลูปเพื่ออ่านข้อมูล
    while(1) {
        ReadFile(hSerial, readBuffer, sizeof(readBuffer)-1, &bytesRead, NULL);
        if (bytesRead > 0) {
            readBuffer[bytesRead] = '\0';
            printf("Received: %s\n", readBuffer);
        }
    }

    // 7. ปิดการเชื่อมต่อ
    CloseHandle(hSerial);
    return 0;
} 