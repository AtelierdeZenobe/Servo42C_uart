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

MessageIn UartCOM::Send(std::shared_ptr<MessageOut> messageOut)
{
    bool success = true;
    uint8_t* message;
    size_t messageSize;
    size_t bytesSend = 0;

    ////////
    /// Send
    if(messageOut->isValid())
    {
        //TODO: state sending
        bytesSend = m_servo42->write(messageOut->getDatagram(), messageOut->getDatagramSize());
        if(bytesSend != messageSize)
        {
            setState(UART_ERROR);
            success = false;
            //printMutex.lock();
            printf("Sent %d bytes instead of %d bytes.\n", bytesSend, messageSize);
            //printMutex.unlock();
            success = false;
        }
    }
    else
    {
        printMutex.lock();
        std::cerr << "Cannot send: uartCOM invalid" << std::endl;
        printMutex.unlock();
        success = false;
    }

    //////////
    /// Receive
    //TODO: async ?
    std::vector<uint8_t> answer;
    if(success)
    {
        setState(UART_RECEIVING);

        //TODO: avoid blocking loop.
        int timeout = 0;
        while(!m_servo42->readable() && timeout++<0xFFFF)
        {
            //ThisThread::sleep_for(1ms);
            wait_us(50);
        }
        if (m_servo42->readable())
        {
            uint8_t buf[HEADER_SIZE + MAX_DATA_SIZE + CHECKSUM_SIZE];

            // It appears data may arrive slower than expected.
            // TODO: encapsulate the call to read untill the expected number of bytes are read ?

            thread_sleep_for(10); //This sleep somehow allows for read to receive all the data
            int bytes_read = m_servo42->read(buf, (HEADER_SIZE + MAX_DATA_SIZE + CHECKSUM_SIZE));
            answer = std::vector<uint8_t>(buf, buf + bytes_read);
        }
        else
        {
            std::cerr << "uartCOM not readable" << std::endl;
            setState(UART_ERROR);
        }
        setState(UART_READY);
    }
    return MessageIn(answer);
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
        printf("Could not switch from %02x to %02x\r\n", m_state, newState);
        printMutex.unlock();
    }
    return success;
}