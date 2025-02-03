#include "uartCOM.h"

#include <cstdint>
#include <cstdio>
#include <memory>

#define HEADER_SIZE 2
#define MAX_DATA_SIZE 5
#define CHECKSUM_SIZE 1

UartCOM::UartCOM(PinName TX, PinName RX)
{
    m_servo42 = new BufferedSerial(TX, RX); // TX RX STM
    m_servo42->set_baud(9600);
    m_servo42->set_format(8, BufferedSerial::None, 1);
    setState(UART_READY);
    
    // Attempt to prevent serial stuck
    m_servo42->sync(); // Flush serial
    m_servo42->set_blocking(false);
}

void UartCOM::Send(std::shared_ptr<MessageOut> messageOut)
{
    size_t bytesSend = 0;

    if (!messageOut || !m_servo42) {
        printMutex.lock();
        std::cerr << "Invalid pointers" << std::endl;
        printMutex.unlock();
        return;
    }

    // messageOut->display();

    ////////
    /// Send
    if(messageOut->isValid())
    {
        //TODO: state sending
        bytesSend = m_servo42->write(messageOut->getDatagram(), messageOut->getDatagramSize());
        if(bytesSend != messageOut->getDatagramSize())
        {
            setState(UART_ERROR);
            printMutex.lock();
            LOG("Sent %d bytes instead of %d bytes.\n", bytesSend, messageOut->getDatagramSize());
            printMutex.unlock();
        }
    }
    else
    {
        printMutex.lock();
        std::cerr << "Cannot send: uartCOM invalid" << std::endl;
        printMutex.unlock();
    }
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
        m_state = newState;
    }
    else
    {
        printMutex.lock();
        LOG("Could not switch from %02x to %02x\r\n", m_state, newState);
        printMutex.unlock();
    }
    return success;
}