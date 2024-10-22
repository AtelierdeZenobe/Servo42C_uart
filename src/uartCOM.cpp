#include "uartCOM.h"

#include <cstdint>
#include <cstdio>

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

bool UartCOM::Send(Message * messageOut,  Message &messageIn)
{
    bool success = true;
    uint8_t* message;
    size_t messageSize;
    size_t bytesSend = 0;

    if(messageOut->getMessage() != nullptr)
    {
        message = messageOut->getMessage();
        messageSize = messageOut->size();
    }
    else
    {
        success = false;
        setState(UART_ERROR);
        printMutex.lock();
        printf("Trying to send empty message !\n");
        printMutex.unlock();
    }
    
    // Send
    if(success)
    {
        bytesSend = m_servo42->write(message, messageSize);
        if(bytesSend != messageSize)
        {
            setState(UART_ERROR);
            success = false;
            printMutex.lock();
            printf("Sent %d bytes instead of %d bytes.\n", bytesSend, messageSize);
            printMutex.unlock();
        }
    }

    // Receive
    // TODO: clarify reception, async ?
    if(success)
    {
        setState(UART_RECEIVING);

        uint8_t buf[HEADER_SIZE + MAX_DATA_SIZE + CHECKSUM_SIZE];
        int bytes_read = 0;

        //TODO: avoid blocking loop.
        while(!m_servo42->readable())
        {}
        if (m_servo42->readable())
        {
            bytes_read = m_servo42->read(buf, sizeof(buf));
            if (bytes_read > 0)
            {
                buf[bytes_read] = '\0';
                std::vector<uint8_t> answerBuffer;

                for(size_t i = 1; i < bytes_read-1; ++i)
                {
                   //printf("%02x ", buf[i]);
                    answerBuffer.push_back(buf[i]);
                }

                messageIn = Message(buf[0],0x00,answerBuffer);
                messageIn.display();
            }
            else
            {
                printMutex.lock();
                printf("Readable but no byte received.\n");
                printMutex.unlock();
            }
        }
        else
        {
            printMutex.lock();
            printf("COULD NOT READ ANSWER\n");
            printMutex.unlock();
        }
        setState(UART_READY);
    }
    delete(messageOut);
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