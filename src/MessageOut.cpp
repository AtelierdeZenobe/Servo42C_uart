#include "MessageOut.h"

MessageOut::MessageOut(uint8_t slaveAddress, uint8_t functionCode, std::vector<uint8_t> data)
        : Message(slaveAddress, functionCode, data)
{
    buildMessage();
    setState(MessageState::READY);
}

void MessageOut::buildMessage()
{
    m_datagram.clear();
    m_datagram.push_back(m_slaveAddress);
    m_datagram.push_back(m_functionCode);
    m_datagram.insert(m_datagram.end(),m_data.begin(), m_data.end());
    m_datagram.push_back(computeChecksum());
}

