#include "mbed.h"
#include <map>
#include <utility>

static const std::map<uint8_t,std::pair<PinName, PinName>> UART_MAP =
{
    {0xE1, {PC_10, PC_11}}, // UART3
    {0xE2, {PA_0, PA_1}},  // UART4
    {0xE3, {PC_12, PD_2}}   // UART5
};

