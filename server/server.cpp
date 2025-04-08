#include <iostream>
#include <string>
#include <map>
#include <thread>
#include <mutex>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

std::map<std::string, int> clientSockets; // map of currently connected clients
std::mutex clients_mutex;
const int PORT = 12345;

int sendMessage(const std::string& sender, const std::string& recipient, const std::string& content) {
    // check if sender and recipient are valid
    if (clientSockets.count(sender) == 0) {
        std::cerr << "Sender not found: " << sender << "\n";
        return -1;
    }
    if (clientSockets.count(recipient) == 0) {
        std::cerr << "Recipient not found: " << recipient << "\n";
        return -1;
    }

    // send message
    int senderSocket = clientSockets[sender];
    int recipientSocket = clientSockets[recipient];
    std::lock_guard<std::mutex> lock(clients_mutex);
    send(recipientSocket, content.c_str(), content.size(), 0);

    return 0;
}

int processMessage(const std::string& message) {
    size_t contactSeparator = message.find("->"); // separator bwtwn sender and recipient
    size_t messageSeparator = message.find(":"); // separator bwtwn contacts and message
    // ommit invalid messages
    if (contactSeparator == std::string::npos || messageSeparator == std::string::npos) {
        std::cerr << "Invalid message format: " << message << "\n";
        return -1;
    }
    std::string sender = message.substr(0, contactSeparator);
    std::string recipient = message.substr(contactSeparator + 2, messageSeparator - contactSeparator - 2); 
    std::string content = message.substr(messageSeparator + 1);
    std::cout << "Message of " << sender << " to " << recipient << ":\n" << content << "\n\n";

    // check if sender and recipient are valid
    return 0;
}

void handleClient(int client_socket) {
    char buffer[1024] = {0};
    // recieve username (first thing the client sends)
    read(client_socket, buffer, 1024);
    std::string username(buffer);

    { // register client
        std::lock_guard<std::mutex> lock(clients_mutex);
        clientSockets[username] = client_socket;
        std::cout << username << " connected.\n";
    }

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_read = read(client_socket, buffer, 1024);
        if (bytes_read <= 0) break;  // desconected client
        
        // process message
        std::string message(buffer);
        if (processMessage(message) == -1) {
            std::cerr << "Error processing message: " << message << "\n";
            continue;
        }
    }

    { // delete client of currently connected clients map
        std::lock_guard<std::mutex> lock(clients_mutex);
        clientSockets.erase(username);
        std::cout << username << " disconnected.\n";
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

    std::cout << "Listening in port  " << PORT << "...\n";

    while (true) {
        int client_socket = accept(server_fd, nullptr, nullptr);
        std::thread(handleClient, client_socket).detach();
    }

    close(server_fd);
    return 0;
}
