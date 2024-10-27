#include "Message.h"    

/**
* @brief MessageIn Class to handle motors' answers.
*/
class MessageIn : Message
{
public:
    /**
    * @brief Message default Constructor.
    *
    * @param datagram Raw answer data
    */
    MessageIn(std::vector<uint8_t> datagram);

private:
    /**
    * @brief Initialize the member variables based on raw datagram.
    * @return Wether the operation was successful.
    */
    bool readDatagram();
};