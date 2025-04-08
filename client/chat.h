#ifndef CHAT_H
#define CHAT_H

#include <QObject>
#include <QString>

class messagesApp;  // Forward declaration

class chat : public QObject {
    Q_OBJECT

public:
    chat();
    void sendMessage(std::string destinatary, std::string message);
    void setMessagesApp(messagesApp* app);  // New method

private slots:
    void renderMessage(const QString& senderName, const QString& message);

private:
    messagesApp* app;  // New member
};

#endif // CHAT_H
