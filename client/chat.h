#ifndef CHAT_H
#define CHAT_H

#include <string>

class chat
{
public:
    chat();
    static void sendMessage(std::string destinatary, std::string message);
};

#endif // CHAT_H
