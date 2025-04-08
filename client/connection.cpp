#include "connection.h"

const int PORT = 12345;
const std::string SERVER_IP = "127.0.0.1";  // Dirección local para pruebas

std::optional<connection> connection::currentConnection;

connection::connection() {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
}

void connection::registerUser(std::string username) {
    this->username = username;
    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP.c_str(), &server_address.sin_addr);

    if (connect(clientSocket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Error connecting to server.\n";
    }

    send(clientSocket, this->username.c_str(), this->username.size(), 0);
}

connection& connection::getConnection() {
    if(not connection::currentConnection.has_value()) {
        connection::currentConnection = connection();
    }
    return connection::currentConnection.value();
}

int connection::getClientSocket() {
    return this->clientSocket;
}

std::string connection::getUsername() {
    return this->username;
}
