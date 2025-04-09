#ifndef CHAT_H
#define CHAT_H

#include <QObject>
#include <QString>
#include <vector>

class messagesApp;  // Forward declaration

class chat : public QObject {
    Q_OBJECT

public:
    chat();
    void sendMessage(std::string destinatary, std::string message);
    void setMessagesApp(messagesApp* app);  // New method
    void loadContactMessages(const std::string& contact);

private slots:
    void handleIncomingMessage(const QString& senderName, const QString& message);

private:
    messagesApp* app;  // New member
    std::vector<std::string> contacts;
    void renderMessage(const QString& senderName, const QString& message);
};

#endif // CHAT_H
