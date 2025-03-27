#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <string>
#include <thread>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>   // inet_pton()
#include <string.h>

class connection
{
public:
    connection();
    void start(std::string username);
};

#endif // CONNECTION_H
