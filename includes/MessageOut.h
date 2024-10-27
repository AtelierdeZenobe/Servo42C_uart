#include "Message.h"    
#include <cstdint>

/**
* @brief MessageOut Class to handle instructions to send to motors.
*/
class MessageOut : public Message
{
public:
    /**
    * @brief Message default Constructor.
    *
    * @param slaveAddress Motor address
    * @param functionCode Function code
    * @param data vector of data, can be empty.
    */
    MessageOut(uint8_t slaveAddress, uint8_t functionCode, std::vector<uint8_t> data);

private:
    /**
    * @brief Build the datagram to send
    */
    void buildMessage();
};