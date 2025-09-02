#include <Port.h>

#define false FALSE
#define CAN_RATE_500K 500000
#define CAN_RATE_1M   1000000

HANDLE hSerial ;
char writeBuffer[64] ; 
DWORD bytesWritten, bytesRead;

HANDLE Serial_Begin(const char *Port, uint32_t baudrate, uint8_t byte_size, uint8_t parity, uint8_t stop_bits) {
     hSerial = CreateFile(Port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);

    if (hSerial == INVALID_HANDLE_VALUE) {
        printf("Error opening COM port. Error Code: %ld\n", GetLastError());
        return INVALID_HANDLE_VALUE;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        printf("Error getting COM state. Error Code: %ld\n", GetLastError());
        CloseHandle(hSerial);
        return INVALID_HANDLE_VALUE;
    }

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = MAXDWORD; 
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        printf("Error setting timeouts. Error Code: %ld\n", GetLastError());
        CloseHandle(hSerial);
        return INVALID_HANDLE_VALUE;
    }

    if (!FlushFileBuffers(hSerial)) {
            printf("Error flushing serial port buffer. Error Code: %ld\n", GetLastError());
    } else {
            printf("Flushed serial port buffer successfully.\n");
    }

    printf("Serial port initialized with Baudrate: %d\n", baudrate);
    return hSerial;
}

boolean CAN_Begin(int bitrate) {
    switch (bitrate) {
        case CAN_RATE_500K :
            printf("CAN 500k bps beginned\n") ;
            sprintf(writeBuffer, "S6\r"); // Set bitrate to 500k
            break ;
        case CAN_RATE_1M :
            printf("CAN 1M bps beginned\n") ;
             sprintf(writeBuffer, "S8\r"); // Set bitrate to 1M
            break;
        default :
             printf("Error: Unsupported bitrate %d\n", bitrate);
            return false ;
    }
    if (!WriteFile(hSerial, writeBuffer, strlen(writeBuffer), &bytesWritten, NULL)){
        return false;
    }
    // Open Channel
    sprintf(writeBuffer, "O\r");
    if (!WriteFile(hSerial, writeBuffer, strlen(writeBuffer), &bytesWritten, NULL)) {
        return FALSE;
    }
    printf("CAN channel opened successfully!\n");
    return TRUE;
}

boolean CAN_Write(const char *addr, const char *data) {
    sprintf(writeBuffer, "T%s8%s\r", addr, data ) ;
    if (!WriteFile(hSerial, writeBuffer, strlen(writeBuffer), &bytesWritten, NULL)) {
        return FALSE;
    }
    else {
        printf("CAN writed successfully!\n") ;
        return TRUE ;
    }
}

boolean CAN_Write_integer(const char* addr, uint8_t* packet_buffer) {
    char data_as_string[9];
    for (int i = 0; i < 4; i++) {
        sprintf(&data_as_string[i * 2], "%02X", packet_buffer[i]);
    }
    return CAN_Write(addr, data_as_string);
}
