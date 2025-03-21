#pragma once
#include <cstdint>
#include <iostream>
//#include <cstdint>
#include <memory>
#include <vector>
#include "mbed.h"

#include "MessageIn.h"
#include "MessageOut.h"

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
    * @brief Constructor. Defaults to F446RE UART3
    *
    * @param TX Controller uart tx pin
    * @param RX Controller uart rx pin
    */
    UartCOM(PinName TX = PC_10, PinName RX = PC_11);

    /**
    * @brief Send a Message over UART and wait for an answer.
    * 
    * @param MessageOut The Message to send.
    * @param timeout The timeout period.
    */
    void Send(std::shared_ptr<MessageOut> MessageOut);

    private:
    /**
    * @brief Update the SM based on the new state request.
    * 
    * @param newState The state requested.
    * @return Wether the operation was successful.
    */
    bool setState(const uartSM &newState);

    uartSM m_state = UART_IDLE;
    BufferedSerial* m_servo42;
};