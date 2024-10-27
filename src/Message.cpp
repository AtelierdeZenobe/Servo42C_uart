#include <fstream>
#include <iostream> 
#include <sstream>

#include "Message.h"

Message::Message(uint8_t slaveAddress, uint8_t functionCode, std::vector<uint8_t> data)
        : m_slaveAddress(slaveAddress),
        m_functionCode(functionCode),
        m_data(data)
{
    Message();
}

Message::Message(std::vector<uint8_t> datagram)
        : m_datagram(datagram)
{
    Message();
}

Message::Message() : m_state(MessageState::INIT)
{
    // Nothing to do
}

uint8_t Message::computeChecksum()
{
    uint8_t sum = 0;
    for(const auto &data : m_datagram)
    {
        sum += data;
    }

    uint8_t checkSum = sum & 0xFF;

    return checkSum;
}

bool Message::isValid()
{
    return m_checkSum == computeChecksum();
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
            m_printMutex.lock();
            std::cerr << "Message not handled" << std::endl;
            m_printMutex.unlock();
        }
    }
    // TODO: magic enum or custom one
    if(success)
    {
        m_printMutex.lock();
        std::cout << static_cast<uint8_t>(m_state) << "->" << static_cast<uint8_t>(newState) << std::endl;
        m_printMutex.unlock();
        m_state = newState;
    }
    else
    {
        m_printMutex.lock();
        std::cerr << "Invalid transition" << static_cast<uint8_t>(m_state) << "->" << static_cast<uint8_t>(newState) << std::endl;
        m_printMutex.unlock();
    }

    return success;
}

void Message::display()
{
    m_printMutex.lock();
    if(m_state == MessageState::READY)
    {
        std::cout << "Message:" << std::endl;
        std::cout << "\tAddress: " << m_slaveAddress << std::endl;
        std::cout << "\tFunction code: " << m_functionCode << std::endl;
        std::cout << "\tData: ";
        for(const auto& d : m_data)
        {
            //TODO std::cout with hex function
            printf("%02x ", d);
        }
        std::cout << std::endl;
    }
    else
    {
        std::cerr << "Cannot print message: not READY" < std::endl;
    }
    m_printMutex.unlock();
}

// TODO (unusable for now really)
void Message::print(const std::ostringstream& oss)
{
    m_printMutex.lock();
    std::cout << oss.str() << std::endl;
    m_printMutex.unlock();
}