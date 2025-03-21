#include <fstream> 
#include <sstream>

#include "Message.h"

Message::Message(uint8_t slaveAddress, uint8_t functionCode, const std::vector<uint8_t>& data)
        : m_slaveAddress(slaveAddress),
        m_functionCode(functionCode),
        m_data(data),
        m_state(MessageState::INIT)
{
    // Nothing to do
}

Message::Message(const std::vector<uint8_t>& datagram)
        : m_datagram(datagram),
        m_state(MessageState::INIT)
{
    //Nothing to do
}

uint8_t Message::computeChecksum()
{
    // Don't use m_datagram ! We use computeChecksum() to build it !
    uint8_t sum = m_slaveAddress;
    sum += m_functionCode;
    for(const auto &data : m_data)
    {
        sum += data;
    }
    uint8_t checkSum = sum & 0xFF;
    m_checksum = checkSum;
    return checkSum;
}

bool Message::isValid()
{
    return m_checksum == computeChecksum();
}

bool Message::setState(const MessageState& newState)
{
    bool success = false;
    // Only legal transition (outside of error) is INIT -> READY 
    switch(m_state)
    {
        case MessageState::INIT:
        {
            if(newState == MessageState::READY
                || newState == MessageState::ERROR)
            {
                success = true;
            }
            break;
        }
        case MessageState::READY:
        {
            if(newState == MessageState::ERROR)
            {
                success = true;
            }
            break;
        }
        case MessageState::ERROR:
        {
            break;
        }
        case MessageState::UNDEFINED:
        {
            break;
        }
        default:
        {
            //TODO: avoid printing directly
            printMutex.lock();
            std::cerr << "MessageState not handled" << std::endl;
            printMutex.unlock();
        }
    }
    // TODO: magic enum or custom one
    if(success)
    {
        printMutex.lock();
        LOG("Message transition: %02x -> %02x\n", m_state, newState);
        printMutex.unlock();
        m_state = newState;
    }
    else
    {
        printMutex.lock();
        LOG("Invalid transition: %02x -> %02x\n", m_state, newState);
        printMutex.unlock();
    }

    return success;
}

void Message::display() const
{
    printMutex.lock();
    if(m_state == MessageState::READY)
    {
        LOG("Message:\n");
        LOG("\tAddress: %02x\n", m_slaveAddress);
        LOG("\tFunction code: %02x\n", m_functionCode);
        LOG("\tData: ");
        for(const auto& d : m_data)
        {
            LOG("%02x ", d);
        }
        LOG("\n");
        LOG("\tDatagram: ");
        for(const auto& d : m_datagram)
        {
            //TODO std::cout with hex function
            LOG("%02x ", d);
        }
        LOG("\n");
    }
    else
    {
        std::cerr << "Cannot print message: not READY" << std::endl;
    }
    printMutex.unlock();
}

// TODO (unusable for now really)
void Message::print(const std::ostringstream& oss)
{
    printMutex.lock();
    std::cout << oss.str() << std::endl;
    printMutex.unlock();
}