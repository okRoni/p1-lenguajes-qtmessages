#include "connection.h"

const int PORT = 12345;
const std::string SERVER_IP = "127.0.0.1";  // Dirección local para pruebas

connection::connection() {}

void connection::start(std::string username) {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP.c_str(), &server_address.sin_addr);

    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Error al conectar al servidor.\n";
    }

    send(client_socket, username.c_str(), username.size(), 0);
}
