#include <cstdint>
#include <iostream>
//#include <cstdint>
#include <vector>
#include "mbed.h"

#include "message.h"

enum uartSM
{
    UART_IDLE       = 0x00,
    UART_READY      = 0x01,
    UART_RECEIVING  = 0x02,
    UART_ERROR      = 0xFF
};

class UartCOM
{
    public:
    /**
    * @brief Constructor.
    */
    UartCOM();

    /**
    * @brief Send a Message over UART and wait for an answer.
    * 
    * @param MessageOut The Message to send.
    * @param MessageIn The Message received (answer).
    * @param timeout The timeout period.
    * @return the
    */
    bool Send(Message &MessageOut, Message &MessageIn);


    bool setState(const uartSM &newState);
    uartSM m_state = UART_IDLE;
    BufferedSerial* m_servo42;

};