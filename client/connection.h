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

class connection
{
private:
    int clientSocket;
    static std::optional<connection> currentConnection;
public:
    std::string username;
    connection();
    void registerUser(std::string username);
    static connection getConnection();
    int getClientSocket();
};

#endif // CONNECTION_H
