#include "chat.h"
#include "connection.h"
#include "messagesapp.h"
#include "ui_messagesapp.h"
#include <sys/socket.h>
#include <fstream>
#include <QMessageBox>

chat::chat() {
    connection& net = connection::getConnection();
    QObject::connect(&net, &connection::messageReceived, this, &chat::handleIncomingMessage, Qt::QueuedConnection);
}

void saveMessage(const std::string& sender, const std::string& message) {
    std::ofstream file("messages.txt", std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open messages file for writing.\n";
        return;
    }
    file << sender << "->" << message << std::endl;
    file.close();
}

void chat::renderMessage(const QString& senderName, const QString& message) {
    std::string senderStr = senderName.toStdString();
    if ("me|" == senderStr.substr(0, 3)) {
        senderStr = "<span style='color:#218a91'><b>you</b></span>";
    } else {
        senderStr = "<span style='color:#215391'><b>" + senderStr + "</b></span>";
    }
    QString messageText = "\n" + QString::fromStdString(senderStr) + ": " + message;
    app->getUi()->chatText->append(messageText);
}

void chat::sendMessage(std::string destinatary, std::string message) {
    // check if contact is new or not
    // if new, add it to list and click it
    if (std::find(contacts.begin(), contacts.end(), destinatary) == contacts.end()) {
        contacts.push_back(destinatary);
        app->getUi()->contactsList->addItem(QString::fromStdString(destinatary));
        app->getUi()->contactsList->setCurrentItem(app->getUi()->contactsList->item(app->getUi()->contactsList->count() - 1));
        app->getUi()->contactText->setPlainText(QString::fromStdString(destinatary));
        app->getUi()->contactText->setEnabled(false);
    } // if already exist and not selected, show error
    else if (app->getUi()->contactsList->currentItem() &&
             app->getUi()->contactsList->currentItem()->text() != QString::fromStdString(destinatary)) {
        QMessageBox::critical(app, "Error", "A chat with the specified contact already exists. Please select it from the list.");
        return;
    }

    connection& net = connection::getConnection();  // Change to reference
    std::string sender = net.getUsername();

    std::string fullMessage = sender + "->" + destinatary + ":" + message;

    send(net.getClientSocket(), fullMessage.c_str(), fullMessage.size(), 0);

    std::string localSender = "me|" + destinatary;
    
    saveMessage(localSender, message);
    renderMessage(QString::fromStdString(localSender), QString::fromStdString(message));
    app->getUi()->messageInput->clear();

}

void chat::setMessagesApp(messagesApp* app) {
    this->app = app;
}

void chat::loadContactMessages(const std::string& contact) {
    std::ifstream file("messages.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open messages file for reading.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::string::size_type separator = line.find("->");
        if (separator == std::string::npos) continue;  // skip invalid lines
        std::string sender = line.substr(0, separator);
        std::string message = line.substr(separator + 2);
        if (sender.find(contact) != std::string::npos) {
            renderMessage(QString::fromStdString(sender), QString::fromStdString(message));
        }
    }

    file.close();
}

void chat::handleIncomingMessage(const QString& senderName, const QString& message) {
    // check for contact
    std::string contact;
    std::string senderStr = senderName.toStdString();
    if ("me|" == senderStr.substr(0, 3)) {
        contact = senderStr.substr(3);
    } else {
        contact = senderStr;
    }

    // check if contact is in the list
    if (std::find(contacts.begin(), contacts.end(), contact) == contacts.end()) {
        contacts.push_back(contact);
        // update contact list in the UI
        app->getUi()->contactsList->addItem(QString::fromStdString(contact));
    }

    // save message
    saveMessage(senderStr, message.toStdString());

    // render message if contact is selected
    if (app->getUi()->contactsList->currentItem() &&
        app->getUi()->contactsList->currentItem()->text() == QString::fromStdString(contact)) {
        renderMessage(senderName, message);
    }
}
