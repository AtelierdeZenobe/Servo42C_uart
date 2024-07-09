#include "mbed.h"
#include <cstdio>
#include "uartCOM.h"
#include <cstdint>

//TODO: Find why we can't print to the console

int main() {

    UartCOM uartCOM = UartCOM();

    message messageOut = {0xE0, 0x80, {0x00}, 0x60};
    message messageIn;

    uartCOM.Send(messageOut, messageIn);
    
}
