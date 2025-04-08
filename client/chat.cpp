#include "chat.h"
#include "connection.h"
#include "messagesapp.h"
#include "ui_messagesapp.h"
#include <sys/socket.h>

chat::chat() {
    connection& net = connection::getConnection();
    QObject::connect(&net, &connection::messageReceived, this, &chat::renderMessage, Qt::QueuedConnection);
}

void chat::sendMessage(std::string destinatary, std::string message) {
    connection& net = connection::getConnection();  // Change to reference
    std::string sender = net.getUsername();

    message = sender + "->" + destinatary + ":" + message;

    send(net.getClientSocket(), message.c_str(), message.size(), 0);
}

void chat::setMessagesApp(messagesApp* app) {
    this->app = app;
}

void chat::renderMessage(const QString& senderName, const QString& message) {
    if (app) {
        QString messageText = "\n" + senderName + ":" + message;
        app->getUi()->chatText->append(messageText);
    }
}
