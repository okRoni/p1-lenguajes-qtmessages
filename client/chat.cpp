#include "chat.h"
#include "connection.h"
#include <sys/socket.h>

chat::chat() {
    connection& net = connection::getConnection();
    QObject::connect(&net, &connection::messageReceived, this, &chat::renderMessage);  // Connect signal to slot
}

void chat::sendMessage(std::string destinatary, std::string message) {
    connection& net = connection::getConnection();  // Change to reference
    std::string sender = net.getUsername();

    message = sender + "->" + destinatary + ":" + message;

    send(net.getClientSocket(), message.c_str(), message.size(), 0);
}

void chat::renderMessage(const QString& senderName, const QString& message) {
    std::cout << "Message from" << senderName.toStdString() << ":" << message.toStdString() << "\n";
}
