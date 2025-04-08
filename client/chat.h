#ifndef CHAT_H
#define CHAT_H

#include <string>
#include <QObject>  // Include QObject for signals and slots

class chat : public QObject  // Inherit from QObject
{
    Q_OBJECT  // Enable Qt's meta-object features
public:
    chat();
    void sendMessage(std::string recipient, std::string message);

public slots:
    void renderMessage(const QString& senderName, const QString& message);  // Slot for handling messages
};

#endif // CHAT_H
