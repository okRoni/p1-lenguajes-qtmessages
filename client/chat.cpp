#include "chat.h"
#include "connection.h"
#include <sys/socket.h>

chat::chat() {}

void chat::sendMessage(std::string destinatary, std::string message) {
    connection cone = connection::getConnection();
    std::string sender = cone.username;

    message = sender + "->" + destinatary + ":" + message;

    send(cone.getClientSocket(), message.c_str(), message.size(), 0);
}
