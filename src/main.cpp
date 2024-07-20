#include "mbed.h"
#include <cstdint>
#include <iostream>
//#include <cstdio>
#include "uartCOM.h"
//#include <cstdint>

//TODO: Find why we can't print to the console
// For now: -> Switch USB
int main()
{
    UartCOM uartCOM = UartCOM();

    //message messageOut = {0xE0, 0x80, {0x00}}; //CAL
    //message messageOut = {0xE0, 0xFD, {0x88, 0x0F, 0x00, 0xFF, 0x0F}}; 
    //message messageOut = {0xE0, 0xF6, {0xFF}}; // Run at constant speed

    Message messageOut = Message(0xE0, 0xFD, {0x88, 0x0F, 0x00, 0xFF, 0x0F}); //FORWARD
    Message messageIn;
    uartCOM.Send(messageOut, messageIn);

}
