#pragma once
#include "mbed.h"
#include "common.h"

#include <cstdint> // uint8_t
#include <vector>
#include <iostream>
#include <sstream>
#include <string> 

static Mutex printMutex;

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
    /// @brief Datagram getter to c-style
    inline const uint8_t* getData() const {return m_data.data();}
    /// @brief Datagram getter to c-style
    inline const uint8_t* getDatagram() const {return m_datagram.data();}
    /// @brief Datagram size getter to c-style
    inline const size_t getDatagramSize() const {return m_datagram.size();}

    /**
    * @brief Check the validity of the raw message, based on checksum and datagram
    * @return The validity of the message
    */
    bool isValid();

    /**
    * @brief Print the Message content to the console.
    */
    void display() const;

    //TODO weird public static member variable
    /// @brief Mutex for console output
    static Mutex m_printMutex;

protected:
    /**
    * @brief Message Constructor for MessageOut.
    * @param slaveAddress Motor address
    * @param functionCode Function code
    * @param data vector of data, can be empty.
    */
    Message(uint8_t slaveAddress, uint8_t functionCode, const std::vector<uint8_t>& data);

    /**
    * @brief Message Constructor for MessageIn.
    * @param datagram Raw uart data
    */
    Message(const std::vector<uint8_t>& datagram);

    /**
    * @brief Trigger a Message SM transition.
    * @param newState new state
    * @return Wether the transition was triggered
    */
    bool setState(const MessageState& newState);

    /**
    * @brief Compute the datagram checksum
    * @return The checksum
    */
    uint8_t computeChecksum();

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
    /// @brief Function code (instruction)
    uint8_t m_functionCode;
    /// @brief Data asociated to the function code
    std::vector<uint8_t> m_data;
    /// @brief Datagram's checksum
    uint8_t m_checksum;

    /// @brief UART datagram to send
    std::vector<uint8_t> m_datagram;

    /// @brief Message state
    MessageState m_state;

private:
    /**
    * @brief Message default Constructor.
    * @note Should never be used directly.
    */
    Message();
};