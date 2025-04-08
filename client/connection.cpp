#include "connection.h"
#include <QString>  // Include QString for Qt signals

const int PORT = 12345;
const std::string SERVER_IP = "127.0.0.1";  // Dirección local para pruebas

connection* connection::currentConnection = nullptr; 

connection::connection() {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    running = false;
}

connection::~connection() {
    running = false;
    if (receiverThread.joinable()) {
        receiverThread.join();
    }
    close(clientSocket);
}

void connection::startReceiving() {
    running = true;
    receiverThread = std::thread(&connection::messageReceiver, this);
}

void connection::registerUser(std::string username) {
    this->username = username;
    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP.c_str(), &server_address.sin_addr);

    if (::connect(clientSocket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Error connecting to server.\n";
    }

    send(clientSocket, this->username.c_str(), this->username.size(), 0);
    startReceiving();  // Start the message receiver thread after registering
}

void connection::messageReceiver() {
    char buffer[1024];
    while (running) {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) break;

        std::string message(buffer);
        size_t separator = message.find("->");
        if (separator != std::string::npos) {
            std::string sender = message.substr(0, separator);
            std::string content = message.substr(separator + 2);

            emit messageReceived(QString::fromStdString(sender), QString::fromStdString(content));
        }
    }
}

connection& connection::getConnection() {
    if (!currentConnection) {
        currentConnection = new connection();  // Allocate memory for the singleton instance
    }
    return *currentConnection;
}

void connection::destroyConnection() {
    delete currentConnection;  // Free the allocated memory
    currentConnection = nullptr;
}

int connection::getClientSocket() {
    return this->clientSocket;
}

std::string connection::getUsername() {
    return this->username;
}
