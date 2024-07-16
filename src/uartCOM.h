#include <iostream>
//#include <cstdint>
#include <vector>
#include "mbed.h"

struct message
{
    uint8_t slaveAddr           = 0xE0;
    uint8_t functionCode        = 0x45;
    std::vector<uint8_t> data;
    uint8_t checkSum            = 0xFF;
};

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
    * @brief Send a message over UART and wait for an answer.
    * 
    * @param messageOut The message to send.
    * @param messageIn The message received (answer).
    * @param timeout The timeout period.
    * @return the
    */
    bool Send(const message &messageOut, message &messageIn);

    private:
    /**
    * @brief Send a message over UART and wait for an answer.
    * 
    * @param messageOut The message to send.
    * @param messageIn The message received (answer).
    * @param timeout The timeout period.
    * @return void
    */
    uint8_t computeCheckSum(const message &message);
    bool setState(const uartSM &newState);
    uartSM m_state = UART_IDLE;
    BufferedSerial* m_servo42;

};