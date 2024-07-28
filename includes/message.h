#include <cstdint>
#include <iostream>
#include <vector>

class Message
{
    public:
    inline Message(){}; // For reception
    /**
    * @brief Constructor. Complete UART message to send.
    *
    * @param slaveAddress Motor address
    * @param functionCode Function code
    * @param data vector of data, can be empty.
    */
    Message(uint8_t slaveAddress, uint8_t functionCode, std::vector<uint8_t> data);
    inline bool isCheckSum(){return m_checkSum != NULL;}
    inline uint8_t* getMessage(){return m_message.data();}
    inline size_t size(){return m_message.size();}
    void display();

    private:
    /**
    * @brief Compute the checksum of the message
    * @return The checksum of the message
    */
    uint8_t computeCheckSum();

    /**
    * @brief Build the message to send
    * @return void
    */
    void buildMessage();

    uint8_t m_slaveAddress = 0xE0;
    uint8_t m_functionCode = 0x45;
    uint8_t m_checkSum;
    std::vector<uint8_t> m_data;
    std::vector<uint8_t> m_message;

};