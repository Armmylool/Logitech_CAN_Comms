#include "steering.h"
#include "Port.h"

#define wheel_address "06000001"

struct Keya_Command wheel_comms = {
    "230D200100000000",                                                 // Enable Command
    "230D200100000000",                                                 // Disable Command
    "230D2001"                                                                      // Header of Position Mode
};

void create_extended_packet(int16_t data, uint8_t* packet_buffer) {
    int16_t data16 = (int16_t)data;

    packet_buffer[0] = (data16 >> 8) & 0xFF;

    packet_buffer[1] = data16 & 0xFF;

    if (data < 0) {
        packet_buffer[2] = 0xFF;
        packet_buffer[3] = 0xFF;
    } else {
        packet_buffer[2] = 0x00;
        packet_buffer[3] = 0x00;
    }
}

void enableWheel() {
    CAN_Write(wheel_address, wheel_comms.ENABLE) ;
}

void disableWheel() {
    CAN_Write(wheel_address, wheel_comms.DISABLE) ;
}

void receiveDataFromG29(Sint16 data) {                                     // Scale value with 1.31068
    int16_t scaleData = - 1* (data / 1.31068 );
    uint8_t wheel_buffer[4]  ; 
    SDL_Log("Steering wheel scale data : %d", scaleData);
    create_extended_packet(scaleData, wheel_buffer) ;
    CAN_Write_integer(wheel_address, wheel_buffer,wheel_comms.POSITION) ;
}