#pragma once
#include "mbed.h"

#include <cstdint> // uint8_t
#include <sstream>
#include <vector> // vector
#include <string>

/**
* @brief Type for Message State Machine
*/
enum class MessageState : uint8_t
{
    UNDEFINED = 0xFF,
    INIT = 0x00,
    READY = 0x01,
    ERROR = 0xF0
};

/**
* @brief Message Class to handle in and out UART communication with motors.
*/
class Message
{
public:
    /**
    * @brief Print the Message content to the console.
    */
    void display();

protected:
    /**
    * @brief Message Constructor for MessageOut.
    * @param slaveAddress Motor address
    * @param functionCode Function code
    * @param data vector of data, can be empty.
    */
    Message(uint8_t slaveAddress, uint8_t functionCode, std::vector<uint8_t> data);

    /**
    * @brief Message Constructor for MessageIn.
    * @param datagram Raw uart data
    */
    Message(std::vector<uint8_t> datagram);

    /**
    * @brief Trigger a Message SM transition.
    * @param newState new state
    */
    bool setState(const MessageState& newState);

    // TODO: check the c++ standard to create a useful print function
    // I like the ostringstream with fold expression
    // Or logger with operator<< override
    /**
    * @brief Print to console with mutex.
    * @param message Message to print
    */
    void print(const std::ostringstream& message);

    /// @brief Motor's slave address
    uint8_t m_slaveAddress;
    /// @brief The instruction
    uint8_t m_functionCode;
    /// @brief Data associated to the instruction
    std::vector<uint8_t> m_data;

    /// @brief UART datagram to send
    std::vector<uint8_t> m_datagram;
    /// @brief Datagram's checksum
    uint8_t m_checkSum;

    /// @brief Mutex for console output
    Mutex m_printMutex;

    /// @brief Message state
    MessageState m_state;

private:
    /**
    * @brief Message default Constructor.
    * @note Should never be used directly.
    */
    Message();
};