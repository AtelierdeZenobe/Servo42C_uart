#include "Message.h"    

/**
* @brief MessageIn Class to handle motors' answers.
*/
class MessageIn : public Message
{
public:
    /**
    * @brief Message default Constructor.
    *
    * @param datagram Raw answer data
    */
    MessageIn(const std::vector<uint8_t>& datagram);

private:
    /**
    * @brief Initialize the member variables based on raw datagram.
    * @return Wether the operation was successful.
    */
    bool readDatagram();
};