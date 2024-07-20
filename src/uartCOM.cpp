#include "uartCOM.h"

#include <cstdint>
#include <cstdio>

#define HEADER_SIZE 2
#define MAX_DATA_SIZE 5
#define CHECKSUM_SIZE 1

UartCOM::UartCOM()
{
    m_servo42 = new BufferedSerial(PC_6, PC_7);
    m_servo42->set_baud(9600);
    m_servo42->set_format(8, BufferedSerial::None, 1);
    setState(UART_READY);
}

bool UartCOM::Send(Message &messageOut,  Message &messageIn)
{
    bool success = true;
    uint8_t* message;
    size_t messageSize;
    size_t bytesSend = 0;

    if(messageOut.getMessage() != nullptr)
    {
        message = messageOut.getMessage();
        messageSize = messageOut.size();
    }
    else
    {
        success = false;
        setState(UART_ERROR);
        printf("Trying to send empty message !\n");
    }
    
    // Send
    if(success)
    {
        bytesSend = m_servo42->write(message, messageSize);
        if(bytesSend == messageSize)
        {
            messageOut.display();
        }
        else
        {
            setState(UART_ERROR);
            success = false;
            printf("Sent %d bytes instead of %d bytes.\n", bytesSend, messageSize);
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
                printf("Received: ");
                for(size_t i = 0; i < bytes_read; ++i)
                {
                    printf("%02x ", buf[i]);
                }
                printf("\n");
            }
            else
            {
                printf("Readable but no byte received.\n");
            }
        }
        else {
            printf("COULD NOT READ ANSWER\n");
        }
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
        printf("Switched from %02x to %02x\r\n", m_state, newState);
        m_state = newState;
    }
    else
    {
        printf("Could not switch from %02x to %02x\r\n", m_state, newState);
    }
    return success;
}