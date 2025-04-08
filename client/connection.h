#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <string>
#include <thread>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>   // inet_pton()
#include <string.h>
#include <optional>
#include <QObject>  // Include QObject for signals and slots

class connection : public QObject  // Inherit from QObject
{
    Q_OBJECT  // Enable Qt's meta-object features
private:
    static connection* currentConnection;  // Use raw pointer
    std::string username;
    int clientSocket;
    std::thread receiverThread;
    bool running;
    void messageReceiver();
public:
    connection();
    ~connection();
    void registerUser(std::string username);
    static connection& getConnection();
    static void destroyConnection();  // Method to clean up the singleton instance
    int getClientSocket();
    std::string getUsername();
    void startReceiving();

signals:
    void messageReceived(const QString& sender, const QString& message);
};

#endif // CONNECTION_H
