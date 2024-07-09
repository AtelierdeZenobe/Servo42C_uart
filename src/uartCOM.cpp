#include "uartCOM.h"

#define HEADER_SIZE 2
#define CHEKSUM_SIZE 1

UartCOM::UartCOM()
{
    m_servo42 = new BufferedSerial(PC_6, PC_7);
    m_servo42->set_baud(9600);
    m_servo42->set_format(8, BufferedSerial::None, 1);
    setState(UART_READY);
}

bool UartCOM::Send(const message &messageOut,  message &messageIn)
{
    bool success = true;
    if (messageOut.data.empty())
    {
        success = false;
    }
    
    if(success)
    {
        //Send
        uint8_t checksum = computeCheckSum(messageOut);

        //TODO: clarify
        //TODO: does "BufferedSerial::write()" work with vector or is it c style only ?
        size_t commandSize = HEADER_SIZE + messageOut.data.size() + CHEKSUM_SIZE;
        uint8_t* command = new uint8_t[commandSize];

        command[0] = messageOut.slaveAddr;
        command[1] = messageOut.functionCode;
        std::copy(messageOut.data.begin(), messageOut.data.end(), command + 2);
        command[commandSize - 1] = checksum;

        m_servo42->write(command, sizeof(command));

        //TODO: wait for the answer
        setState(UART_RECEIVING);
        setState(UART_READY);
    }

    return success;
}

bool UartCOM::setState(const uartSM &newState)
{
    bool success = false;
    switch (newState) {
        case UART_IDLE:
            //Should never go back to Idle
            break;
        case UART_READY:
            if(m_state == UART_IDLE || m_state == UART_RECEIVING)
            {
                success = true;
            }
            break;
        case UART_RECEIVING:
            if(m_state == UART_READY)
            {
                success = true;
            }
        case UART_ERROR:
            //Not handled
            break;
        default:
            break;
    }

    if(success)
    {
        std::cout << "Switched from " << m_state << " to " << newState << std::endl;
        m_state = newState;
    }
    else
    {
        std::cout << "Could not switch from " << m_state << " to " << newState << std::endl;
    }
    return success;
}

uint8_t UartCOM::computeCheckSum(const message &message)
{
    uint8_t sum = message.slaveAddr;
    sum += message.functionCode;
    for(const auto &data : message.data)
    {
        sum += data;
    }

    uint8_t checkSum = sum & 0xFF;
    std::cout << "Checksum: " << checkSum << std::endl;
    return checkSum;
}