#include <iostream>
#include <string>
#include <thread>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>   // inet_pton()
#include <string.h>


const int PORT = 12345;
const std::string SERVER_IP = "127.0.0.1";  // Dirección local para pruebas

void receive_messages(int socket) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = read(socket, buffer, 1024);
        if (bytes_received <= 0) {
            std::cout << "Desconectado del servidor.\n";
            break;
        }
        std::cout << "Mensaje: " << buffer << std::endl;
    }
}

int main() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP.c_str(), &server_address.sin_addr);

    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Error al conectar al servidor.\n";
        return 1;
    }

    std::string username;
    std::cout << "Introduce tu nombre de usuario: ";
    std::getline(std::cin, username);
    send(client_socket, username.c_str(), username.size(), 0);

    std::thread(receive_messages, client_socket).detach();

    std::string message;
    while (true) {
        std::getline(std::cin, message);
        if (message == "salir") break;
        send(client_socket, message.c_str(), message.size(), 0);
    }

    close(client_socket);
    return 0;
}
