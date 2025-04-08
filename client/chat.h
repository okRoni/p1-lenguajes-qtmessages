#ifndef CHAT_H
#define CHAT_H

#include <string>

class chat
{
public:
    chat();
    static void sendMessage(std::string recipient, std::string message);
    static void renderMessage(std::string senderName, std::string message);
};

#endif // CHAT_H
