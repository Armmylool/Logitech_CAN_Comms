#include <stdint.h>
#include <stdio.h>
#include <windows.h>

/**
 * @brief  Serial Port Begin  @param Port Port of USB  @param baudrate Baudrate of USB(bps)  @param byte_size byte setting of USB
 * @param parity ODD or EVEN or NONE @param stop_bits Stop bit setting of USB */
HANDLE Serial_Begin(const char *Port, uint32_t baudrate, uint8_t byte_size, uint8_t parity, uint8_t stop_bits) ;

/** @brief  CAN Setting and Start
 *  @param bitrate  bitrate of CAN device or BUS
 * @warning Serial_Begin must be begin first
*/
boolean CAN_Begin(int bitrate) ;

/** @brief CAN write Data 
 *  @param addr the Extended Address
 *  @param data 8 byte data
*/
boolean CAN_Write(const char *addr, const char *data) ;

boolean CAN_Write_integer(const char* addr, uint8_t* packet_buffer) ;
