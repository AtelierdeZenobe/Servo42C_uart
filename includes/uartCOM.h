#pragma once
#include <cstdint>
#include <iostream>
//#include <cstdint>
#include <memory>
#include <vector>
#include "mbed.h"

#include "message.h"

enum class uartSM : uint8_t
{
    UART_IDLE       = 0x00,
    UART_READY      = 0x01,
    UART_RECEIVING  = 0x02,
    UART_DONE       = 0x03, // TODO: temporary, to check timings
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
    * @param MessageIn The Message received (answer).
    * @param timeout The timeout period.
    * @return the
    */
    bool Send(Message * MessageOut, std::shared_ptr<Message>& MessageIn);

    /**
    * @brief Get the uartCOM state
    * @return the state
    */
    uartSM getState();

    private:
    /**
    * @brief Update the SM based on the new state request.
    * 
    * @param newState The state requested.
    * @return Wether the operation was successful.
    */
    bool setState(const uartSM &newState);

    uartSM m_state = uartSM::UART_IDLE;
    BufferedSerial* m_servo42;
};