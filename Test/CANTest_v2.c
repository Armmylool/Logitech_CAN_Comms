#include <stdio.h>
#include <Port.h>

#define CAN_bps 1000000

int main() {
    Serial_Begin("\\\\.\\COM11", 115200, 8,  NOPARITY, 1) ;
    CAN_Begin(CAN_bps) ;
    CAN_Write("06000001","0102030405060708") ;
}