#include "chat.h"
#include "connection.h"
#include <sys/socket.h>

chat::chat() {}

void chat::sendMessage(std::string destinatary, std::string message) {
    connection& net = connection::getConnection();  // Change to reference
    std::string sender = net.getUsername();

    message = sender + "->" + destinatary + ":" + message;

    send(net.getClientSocket(), message.c_str(), message.size(), 0);
}
