#include "message.h"
#include <cstdint>

Message::Message(uint8_t slaveAddress, uint8_t functionCode, std::vector<uint8_t> data)
        : m_slaveAddress(slaveAddress),
        m_functionCode(functionCode),
        m_data(data)
{
    buildMessage();
}

void Message::buildMessage()
{
    m_message.clear();
    m_message.push_back(m_slaveAddress);
    m_message.push_back(m_functionCode);
    m_message.insert(m_message.end(),m_data.begin(), m_data.end());
    m_message.push_back(computeCheckSum());
}

uint8_t Message::computeCheckSum()
{
    uint8_t sum = m_slaveAddress;
    sum += m_functionCode;
    for(const auto &data : m_data)
    {
        sum += data;
    }

    uint8_t checkSum = sum & 0xFF;

    return checkSum;
}

void Message::display()
{
    printf("Sent: ");
    for(const auto& d : m_message)
    {
        printf("%02x ", d);
    }
    printf("\n");
}