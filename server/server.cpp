#include <iostream>
#include <string>
#include <map>
#include <thread>
#include <mutex>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

std::map<std::string, int> clients;  // Almacena los clientes por nombre de usuario
std::mutex clients_mutex;
const int PORT = 12345;

void handle_client(int client_socket) {
    char buffer[1024] = {0};
    // Recibir nombre de usuario
    read(client_socket, buffer, 1024);
    std::string username(buffer);

    { // Sección crítica para registrar el usuario
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients[username] = client_socket;
        std::cout << username << " conectado.\n";
    }

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_read = read(client_socket, buffer, 1024);
        if (bytes_read <= 0) break;  // Cliente desconectado

        std::string message(buffer);
        size_t sep = message.find(':');
        if (sep == std::string::npos) continue;

        std::string recipient = message.substr(0, sep);
        std::string content = message.substr(sep + 1);

        std::lock_guard<std::mutex> lock(clients_mutex);
        if (clients.count(recipient)) {
            int recipient_socket = clients[recipient];
            send(recipient_socket, content.c_str(), content.size(), 0);
        } else {
            std::string error_msg = "Usuario no encontrado\n";
            send(client_socket, error_msg.c_str(), error_msg.size(), 0);
        }
    }

    { // Eliminar cliente al desconectarse
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(username);
        std::cout << username << " desconectado.\n";
    }
    close(client_socket);
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);

    std::cout << "Servidor escuchando en el puerto " << PORT << "...\n";

    while (true) {
        int client_socket = accept(server_fd, nullptr, nullptr);
        std::thread(handle_client, client_socket).detach();
    }

    close(server_fd);
    return 0;
}
