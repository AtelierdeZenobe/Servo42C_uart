#include "MessageIn.h"

MessageIn::MessageIn(std::vector<uint8_t> datagram)
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
    if(m_datagram.size() > 3)
    {
        m_slaveAddress = m_datagram[0];
        m_functionCode = m_datagram[1];
        m_data = std::vector<uint8_t>(m_datagram.begin() + 2, m_datagram.end() - 1);
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

