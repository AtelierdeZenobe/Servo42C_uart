#include "mbed.h"
#include <iostream>
//#include <cstdio>
#include "uartCOM.h"
//#include <cstdint>

//TODO: Find why we can't print to the console
int main()
{
    UartCOM uartCOM = UartCOM();

    //message messageOut = {0xE0, 0x80, {0x00}, 0x60}; //CAL
    message messageOut = {0xE0, 0xFD, {0x0F, 0x00, 0x00, 0xFF, 0xFF}, 0x60}; //FORWARD
    message messageIn;

    uartCOM.Send(messageOut, messageIn);
}
