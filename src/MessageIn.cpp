#include "MessageIn.h"

MessageIn::MessageIn(const std::vector<uint8_t>& datagram)
        : Message(datagram)
{
    readDatagram();
    if(isValid())
    {
        setState(MessageState::READY);
    }
    else
    {
        setState(MessageState::ERROR);
    }
}

bool MessageIn::readDatagram()
{
    bool success = true;
    if(m_datagram.size() > 2)
    {
        auto it = m_datagram.begin();
        m_slaveAddress = *it++;
        m_functionCode = 0x00; // Does not influence checksum
        m_data = std::vector<uint8_t>(it, m_datagram.end() - 1);
        m_checksum = m_datagram.back();
    }
    else
    {
        //TODO
        printMutex.lock();
        std::cerr << "MessageIn: datagram too short" << std::endl;
        printMutex.unlock();
        success = false;
    }

    return success;
}

